/*
 * Copyright 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: KAEzip Demo
 * Create: 2022-07-26
 */
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

#define NAME_LEN 100
#define SHOUT_OPTION "hcxzf:"
#define HEAD_BLOCK 1
#define CONTENT_BLOCK 2
#define BLOCKSIZE 512
#define BLOCKSIZE2 8192
#define GZIP 1
#define GZ_FILE_SIZE 1000
#define GZ_SUFFIX ".gz"
#define SUFFIX_LEN strlen(GZ_SUFFIX)
#define REGTYPE '0' /* regular file */
#define DIRTYPE '5'  /* directory */
#define DZ_BUF_SIZE 16388

/* Bits used in the mode field, values in octal. */
#define KAE_SUID 04000 /* set UID on execution */
#define KAE_SGID 02000 /* set GID on execution */
#define KAE_SVTX 01000 /* reserved */

/* file permissions */
#define KAE_UREAD 00400 /* read by owner */
#define KAE_UWRITE 00200 /* write by owner */
#define KAE_UEXEC 00100 /* execute/search by owner */
#define KAE_GREAD 00040 /* read by group */
#define KAE_GWRITE 00020 /* write by group */
#define KAE_GEXEC 00010 /* execute/search by group */
#define KAE_OREAD 00004 /* read by other */
#define KAE_OWRITE 00002 /* write by other */
#define KAE_OEXEC 00001 /* execute/search by other */
struct header {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
};
union block {
    char buffer[BLOCKSIZE];
    struct header header;
};

/* global variables */
static const char *g_program_name;
static char *g_archive_name;

static _Bool g_create_flag;
static _Bool g_extract_flag;
static int g_use_compress_program;
static int g_content_block_count;

FILE *g_archive_ptr = NULL;
struct stat *g_stat_buffer = NULL;
static char *g_archive_name_tot;
static int g_archive_fd;
static char g_new_filename[NAME_LEN];

static int g_now_block;
FILE *g_archive_ptr;
FILE *g_file_ptr;

static const struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"create", no_argument, NULL, 'c'},
    {"extract", no_argument, NULL, 'x'},
    {"gzip", no_argument, NULL, 'z'},
    {"file", required_argument, NULL, 'f'},
    {0, 0, 0, 0}
};

static void usage()
{
    printf("usage:\n");
    printf("-c --create          create an archive.\n");
    printf("-x --extract         extract an archive.\n");
    printf("-z --gzip            use compress program gzip.\n");
    printf("-f --file=ARCHIVE    archive name.\n");
    printf("-h --help            help doc.\n");
}

static int set_archive_name(char *arg)
{
    if (!strlen(arg)) {
        return 1;
    }
    int arglen = strlen(arg);
    g_archive_name = (char*)malloc(arglen + 1);
    memset(g_archive_name, 0, arglen + 1);
    /* g_archive_name init */
    if (g_archive_name == NULL) {
        printf("kaezip: memory allocation failed");
        return 1;
    }
    strcpy(g_archive_name, arg);
    printf("archive name is: %s\n", g_archive_name);
    return 0;
}

int compress_file_to_gzip(char *input_name, char *output_name)
{
    gzFile zfile;
    FILE *fp;
    char *new_output_name = NULL;
    if (NULL == output_name) {
        char def_output_name[PATH_MAX] = {0};
        snprintf(def_output_name, sizeof(def_output_name), "%s", input_name);
        new_output_name = def_output_name;
    } else {
        new_output_name = (char*)malloc(NAME_LEN);
        strcpy(new_output_name, output_name);
    }
    if (input_name == NULL) {
        perror("kaezip: file name can not be empty\n");
        free(new_output_name);
        return 1;
    }
    fp = fopen(input_name, "rb");
    if (!fp) {
        perror("kaezip: open file failed\n");
        free(new_output_name);
        return 1;
    }
    zfile = gzopen(new_output_name, "wb");
    if (zfile == NULL) {
        perror("kaezip: open file failed\n");
        free(new_output_name);
        fclose(fp);
        return 1;
    }
    char *s = (char*)malloc(BLOCKSIZE*sizeof(char));
    while (1) {
        /* res is the number of blocks(512) */
        int res = fread(s, BLOCKSIZE, 1, fp);
        gzwrite(zfile, s, 512);
        if (res <= 0) {
            break;
        }
    }
    fclose(fp);
    remove(input_name);
    gzclose(zfile);
    free(new_output_name);
    free(s);
    return 0;
}

static mode_t get_mode(mode_t v)
{
    /* Calculate the mode information written to the tar file header */
    mode_t u;

    u = ((v & S_ISUID ? KAE_SUID : 0) | (v & S_ISGID ? KAE_SGID : 0) |
         (v & S_ISVTX ? KAE_SVTX : 0) | (v & S_IRUSR ? KAE_UREAD : 0) |
         (v & S_IWUSR ? KAE_UWRITE : 0) | (v & S_IXUSR ? KAE_UEXEC : 0) |
         (v & S_IRGRP ? KAE_GREAD : 0) | (v & S_IWGRP ? KAE_GWRITE : 0) |
         (v & S_IXGRP ? KAE_GEXEC : 0) | (v & S_IROTH ? KAE_OREAD : 0) |
         (v & S_IWOTH ? KAE_OWRITE : 0) | (v & S_IXOTH ? KAE_OEXEC : 0));
    return u;
}

static void get_uname(union block *header_buffer)
{
    strcpy(header_buffer->header.uname, "");
}

static void get_gname(union block *header_buffer)
{
    strcpy(header_buffer->header.gname, "");
}

static void to_octal(unsigned int value, char *ptr, size_t size)
{
    if (size == 0) {
        printf("size can not be 0");
        return;
    }
    size--;
    do {
        ptr[--size] = '0' + (value & 7);
        value >>= 3;
    } while(size);
}

static void set_typeflag(union block *header_buffer)
{
    /* Set the typeflag information of the tar file */
    if (S_ISREG(g_stat_buffer->st_mode)) {
        header_buffer->header.typeflag = REGTYPE;
    } else if (S_ISDIR(g_stat_buffer->st_mode)) {
        header_buffer->header.typeflag = DIRTYPE;
    }
}

static void set_chksum(union block *header_buffer)
{
    /* Set chksum of tar file */
    int sum = 0;
    char *p = NULL;

    memset(header_buffer->header.chksum, ' ', 8);
    p = header_buffer->buffer;
    int i;
    for (i = sizeof(*header_buffer); i-- != 0;) {
        sum += 0xFF & *p++;
    }
    to_octal(sum, header_buffer->header.chksum, 7);
}

int gz_uncompress(gzFile in, FILE *out)
{
    char buf[DZ_BUF_SIZE];
    int len;
    int err;
    for(;;)
    {
        len = gzread(in, buf, sizeof(buf));
        if (len < 0) {
            perror(gzerror(in, &err));
            return 1;
        }
        if (len == 0) {
            break;
        }
        if ((int)fwrite(buf, 1, (unsigned)len, out) != len) {
            perror("failed fwrite\n");
            return 1;
        }
    }
    return 0;
}

static int write_emptyblock()
{
    /*
     * Write two empty blocks to the end of the tar file,
     * modified in order to make the archive file the same format
     * as the standard tar file
     */
    union block *empty_buffer;

    empty_buffer = (union block *)malloc(sizeof(union block));
    if (empty_buffer == NULL) {
        printf("kaezip: memory allocation failed\n");
        return 1;
    }
    memset(empty_buffer, 0, BLOCKSIZE);
    write(g_archive_fd, empty_buffer, BLOCKSIZE);
    write(g_archive_fd, empty_buffer, BLOCKSIZE);
    free(empty_buffer);
    return 0;
}

static void write_header_to_archive(char *archive, char *file)
{
    union block header_buffer;
    int filename_len = strlen(file); /* length of filename */

    memset(&header_buffer, 0, BLOCKSIZE);
    get_uname(&header_buffer);
    get_gname(&header_buffer);
    strncpy(header_buffer.header.name, file, filename_len);
    /* add add info from g_stat_buffer to header */
    to_octal(get_mode(g_stat_buffer->st_mode), header_buffer.header.mode, 8); /* mode */
    to_octal(g_stat_buffer->st_uid, header_buffer.header.uid, 8);             /* UID */
    to_octal(g_stat_buffer->st_gid, header_buffer.header.gid, 8);             /* GID */
    to_octal(g_stat_buffer->st_size, header_buffer.header.size, 12);          /* SIZE */
    to_octal(g_stat_buffer->st_mtim.tv_sec, header_buffer.header.mtime, 12);  /* mtime */
    set_typeflag(&header_buffer);                                             /* typeflag */
    strcpy(header_buffer.header.magic, "ustar  ");                            /* magic */
    /* Finally calculate chksum */
    set_chksum(&header_buffer);
    /* write header_buffer to file */
    write(g_archive_fd, &header_buffer, BLOCKSIZE);
}

static int write_contents_to_archive(char *file)
{
    FILE *file_ptr = NULL;
    union block *file_buffer = NULL;

    file_ptr = fopen(file, "rb");
    if (file_ptr == NULL) {
        printf("kaezip: openfile %s failed!\n", file);
        return 1;
    }
    file_buffer = (union block *)malloc(BLOCKSIZE2);
    if (file_buffer == NULL) {
        printf("kaezip: memory allocation failed\n");
        fclose(file_ptr);
        return 1;
    }

    int block_count = g_stat_buffer->st_size / BLOCKSIZE2;
    int i;
    for(i=0; i<block_count; i++)
    {
        fread(file_buffer, BLOCKSIZE2, 1, file_ptr);
        /* res is the number of blocks */
        write(g_archive_fd, file_buffer, BLOCKSIZE2);
    }
    if(g_stat_buffer->st_size % BLOCKSIZE2 != 0) {
        while (1)
        {
            int res = fread(file_buffer, BLOCKSIZE, 1, file_ptr);
            /* res is the number of blocks */
            if(res) {
                write(g_archive_fd, file_buffer, BLOCKSIZE);
            }
            if (res <= 0) {
                if (g_stat_buffer->st_size % BLOCKSIZE == 0) {
                    break;
                }
                write(g_archive_fd, file_buffer, BLOCKSIZE);
                break;
            }
        }
    }
    free(file_buffer);
    fclose(file_ptr);
    file_ptr = NULL;
    return 0;
}

static int write_to_archive(char *archive, char *file)
{
    int res;
    if(g_stat_buffer != NULL) {
        memset(g_stat_buffer, 0, sizeof(struct stat));
    }
    if ((res = stat(file, g_stat_buffer)) != 0) {
        printf("kaezip: stat file %s failed for the reason of %s\n", file, strerror(errno));
        return 1;
    }
    int file_name_len = strlen(file);
    /* Output the file name currently being processed */
    printf("%s\n", file);
    if (S_ISREG(g_stat_buffer->st_mode)) {
        /* Determine if file is a file */
        write_header_to_archive(archive, file);
        if(write_contents_to_archive(file) != 0) {
            return 1;
        }
        return 0;
    } else if (S_ISDIR(g_stat_buffer->st_mode)) {
        /* Determine if file is a directory */
        if (file_name_len != 0 && file[file_name_len - 1] != '/') {
            file[file_name_len++] = '/';
            file[file_name_len] = '\0';
        }
        write_header_to_archive(archive, file);
        DIR *dir = opendir(file);
        if (dir == NULL) {
            printf("open direction %s failed!\n", file);
            return 1;
        }
        struct dirent *now_filename;
        while ((now_filename = readdir(dir)) != NULL) {
            /* Traverse all files in this directory */
            if (strcmp(now_filename->d_name, ".") == 0 ||
                strcmp(now_filename->d_name, "..") == 0) {
                /* skip . and .. directories */
                continue;
            }
            if(strlen(now_filename->d_name) > NAME_LEN) {
                printf("kaezip: filename is too long\n");
                return 1;
            }
            char new_filename[NAME_LEN];
            memset(new_filename, 0, NAME_LEN*sizeof(char));
            strcat(new_filename, file);
            strcat(new_filename, now_filename->d_name);
            if(write_to_archive(archive, new_filename) != 0) {
                return 1;
            }
            /* recursively process files in subdirectories */
        }
        now_filename = NULL;
        closedir(dir);
    } else {
        printf("error: %s is not a file or a directory.\n", file);
        return 1;
    }
    return 0;
}

static void free_all()
{
    if(g_archive_fd != -1) close(g_archive_fd);
    if(g_archive_name_tot != NULL) free(g_archive_name_tot);
    if(g_stat_buffer != NULL) free(g_stat_buffer);
    return;
}

int create(char **argv, char *archive_name)
{
    int archive = creat(archive_name, S_IWUSR | S_IWGRP |
                                      S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH);
    if(archive == -1) {
        printf("kaezip: archive create failed\n");
        return 1;
    }
    /* create archive */
    g_archive_fd = open(archive_name, O_RDWR);
    if(g_archive_fd == -1) {
        printf("kaezip: archive open failed\n");
        free_all();
        return 1;
    }
    memset(g_new_filename, 0, NAME_LEN * sizeof(char));
    char *file_name = NULL;
    g_archive_name_tot = (char *)malloc(NAME_LEN * sizeof(char));
    memset(g_archive_name_tot, 0, NAME_LEN * sizeof(char));
    if (g_archive_name_tot == NULL) {
        printf("kaezip: memory allocation failed\n");
        free_all();
        return 1;
    }
    strcpy(g_archive_name_tot, archive_name);
    g_stat_buffer = (struct stat *)malloc(sizeof(struct stat));
    if (g_stat_buffer == NULL) {
        printf("kaezip: memory allocation failed\n");
        free_all();
        return 1;
    }
    memset(g_stat_buffer, 0, sizeof(struct stat));
    file_name = (char *)malloc(sizeof(char) * NAME_LEN);
    if (file_name == NULL) {
        printf("kaezip: memory allocation failed\n");
        free_all();
        return 1;
    }
    memset(file_name, 0, sizeof(char) * NAME_LEN);
    int filename_position = optind;
    if(strlen(argv[filename_position]) > NAME_LEN)
    {
        printf("kaezip: file name is too long\n");
        free(file_name);
        free_all();
        return 1;
    }
    strcpy(file_name, argv[filename_position]);

    /* Open archive in added mode */
    if(write_to_archive(archive_name, file_name) != 0) {
        free(file_name);
        free_all();
        return 1;
    }
    if(write_emptyblock() != 0) {
        free(file_name);
        free_all();
        return 1;
    }
    free(file_name);
    free_all();
    return 0;
}

static int main_create(char **argv)
{
    if (g_use_compress_program) {
        char *suffix = strrchr(g_archive_name, '.');
        if (suffix == NULL) {
            printf("kaezip: Nonstandard archive name\n");
            return 1;
        }
        *suffix = '\0';
    }
    int create_res = create(argv, g_archive_name);
    if (create_res != 0) {
        return 1;
    }
    if (g_use_compress_program == 0) {
        return 0;
    }
    char *outputname = (char *)malloc(NAME_LEN * sizeof(char));
    if (outputname == NULL) {
        printf("kaezip: memory allocation failed\n");
        return 1;
    }
    memset(outputname, 0, NAME_LEN * sizeof(char));
    strcat(outputname, g_archive_name);
    strcat(outputname, ".gz");
    compress_file_to_gzip(g_archive_name, outputname);
    free(outputname);
    return 0;
}

static unsigned int octal_to_decemal(int octal_number)
{
    int decimal_number = 0, i = 1, j = 0;
    while (octal_number != 0) {
        j = octal_number % 10;
        decimal_number += i * j;
        i *= 8;
        octal_number /= 10;
    }
    return decimal_number;
}

static void extract_head_block(union block *archive_buffer, unsigned int *file_size)
{
    int archive_name_len = strlen(archive_buffer->header.name);

    if (archive_name_len == 0) {
        return;
    }
    if (archive_buffer->header.name[archive_name_len - 1] == '/') {
        mkdir(archive_buffer->header.name, 0777);
        g_content_block_count = 0;
    } else {
        g_file_ptr = fopen(archive_buffer->header.name, "w");
        *file_size = octal_to_decemal(atoi(archive_buffer->header.size));
        g_content_block_count = *file_size / BLOCKSIZE +1;
    }
}

static void extract_content_block(union block *archive_buffer, unsigned int *file_size) {
    if (g_content_block_count == 1) {
        // last block
        fwrite(archive_buffer, *file_size % BLOCKSIZE, 1, g_file_ptr);
    } else {
        fwrite(archive_buffer, BLOCKSIZE, 1, g_file_ptr);
    }
    g_content_block_count--;
    if (g_content_block_count == 0) {
        // Close file when complete
        fclose(g_file_ptr);
    }
}

void extract(char *archive_name)
{
    g_archive_ptr = fopen(archive_name, "rb");
    if (g_archive_ptr == NULL) {
        printf("open archive %s failed!", archive_name);
        return;
    }
    union block archive_buffer;
    g_now_block = HEAD_BLOCK;
    g_content_block_count = 0;
    // file size
    unsigned int file_size;
    int res;
    while (1) {
        memset(archive_buffer.buffer, 0, sizeof(union block));
        res = fread(&archive_buffer, BLOCKSIZE, 1, g_archive_ptr);
        if (res <= 0) {
            break;
        }
        if (g_content_block_count == 0) {
            g_now_block = HEAD_BLOCK;
        } else {
            g_now_block = CONTENT_BLOCK;
        }
        if (g_now_block == HEAD_BLOCK) {
            // In HEAD_BLOCK
            extract_head_block(&archive_buffer, &file_size);
        } else if (g_now_block == CONTENT_BLOCK) {
            // In CONTENT_BLOCK
            extract_content_block(&archive_buffer, &file_size);
        }
    }
    fclose(g_archive_ptr);
}

int decompress_gzip_to_file(char *file)
{
    char buf[GZ_FILE_SIZE];
    char *infile, *outfile;
    FILE *out;
    gzFile in;
    unsigned len = strlen(file);

    if (len + strlen(GZ_SUFFIX) >= sizeof(buf)) {
        fprintf(stderr, "kaezip: filename too long\n");
        return 1;
    }
    snprintf(buf, sizeof(buf), "%s", file);
    if (len > SUFFIX_LEN && strcmp(file + len - SUFFIX_LEN, GZ_SUFFIX) == 0) {
        infile = file;
        outfile = buf;
        outfile[len - 3] = '\0';
    } else {
        outfile = file;
        infile = buf;
        snprintf(buf + len, sizeof(buf) - len, "%s", GZ_SUFFIX);
    }
    in = gzopen(infile, "rb");
    if (in == NULL) {
        fprintf(stderr, "kaezip: can't gzopen %s\n", infile);
        return 1;
    }
    out = fopen(outfile, "wb");
    if (out == NULL) {
        perror(file);
        return 1;
    }
    if (gz_uncompress(in, out) != 0) {
        return 1;
    }
    if (fclose(out)) {
        perror("failed fclose\n");
        return 1;
    }
    if (gzclose(in) != Z_OK) {
        perror("failed gzclose\n");
        return 1;
    }
    return 0;
}

static int main_extract()
{
    if (g_use_compress_program) {
        decompress_gzip_to_file(g_archive_name);
        char *suffix = strrchr(g_archive_name, '.');
        if (suffix == NULL) {
            printf("kaezip: Nonstandard archive name\n");
            return 1;
        }
        *suffix = '\0';
    }
    extract(g_archive_name);
    if (g_use_compress_program) {
        remove(g_archive_name);
    }
    return 0;
}

int main(int argc, char **argv)
{
    int long_index;
    char option_char;

    g_program_name = argv[0];
    while ((option_char = getopt_long(argc, argv, SHOUT_OPTION, long_options, &long_index)) != -1) {
        if (option_char == 255) {
            break;
        }
        
        switch (option_char) {
            case 255:
                break;
            case 'h':
                usage();
                return 0;
            case 'c':
                g_create_flag = 1;
                break;
            case 'x':
                g_extract_flag = 1;
                break;
            case 'z':
                g_use_compress_program = 1;
                break;
            case 'f':
                if (set_archive_name(optarg) != 0) {
                    return 0;
                }
                break;
            default:
                printf("kaezip: please try kaezip --help\n");
                return 0;
        }
    }
    
    if (g_archive_name == NULL) {
        printf("kaezip: please input archive name\n");
        return 0;
    }
    if (g_create_flag) {
        if (main_create(argv) != 0) {
            return 0;
        }
    }
    if (g_extract_flag) {
        if (main_extract() != 0) {
            return 0;
        }
    }
    free(g_archive_name);
    return 0;
}
