
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "sm3.h"

#define HASH_DIGEST_SIZE 32

int KAEDriverSM3Digest(unsigned char *in, unsigned int bufSize, unsigned char *digest);

void PrintDigest(const unsigned char *digest)
{
    unsigned int i;
    for (i = 0; i < HASH_DIGEST_SIZE; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");
}

int GetFileSize(char *filePath, int *fileSize)
{
    struct stat statbuf;
    int ret = stat(filePath, &statbuf);
    if (ret != 0) {
        return ret;
    }
    *fileSize = statbuf.st_size;
    return 0;
}

int ReadAll(char *filePath, char *buf, size_t fileSize)
{
    FILE *fp = fopen(filePath, "rb");
    if (fp == NULL) {
        return 1;
    }
    if (fread(buf, 1, fileSize, fp) != fileSize) {
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

void TestSM3(unsigned char *in, unsigned int inputFileSize)
{
    printf("Test SM3 digest. Result: ");
    unsigned char digest[HASH_DIGEST_SIZE];
    SM3_CTX c;
    sm3_init(&c);
    sm3_update(&c, in, inputFileSize);
    sm3_final(digest, &c);
    PrintDigest(digest);
}

void TestWDSM3(unsigned char *in, unsigned int inputFileSize)
{
    printf("Test WD SM3 digest. Result: ");
    unsigned char digest[HASH_DIGEST_SIZE];
    KAEDriverSM3Digest(in, inputFileSize, digest);
    PrintDigest(digest);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Invalid Input. Input format: ./sm3_demo filepath\n");
        return 1;
    }

    char *inputFilePath = argv[1];
    unsigned int inputFileSize = 0;
    if (GetFileSize(inputFilePath, &inputFileSize) != 0) {
        printf("Failed to read input file.\n");
        return 1;
    }
    unsigned char *in = calloc(1, inputFileSize);
    if (in == NULL) {
        printf("Failed to calloc memory.\n");
        return 1;
    }
    ReadAll(inputFilePath, in, inputFileSize);

    struct timeval start, end;
    gettimeofday(&start, NULL);
    TestSM3(in, inputFileSize);
    gettimeofday(&end, NULL);
    long timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("SM3 cost time: %f s\n", timeuse / 1000000.0);

    gettimeofday(&start, NULL);
    TestWDSM3(in, inputFileSize);
    gettimeofday(&end, NULL);
    timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("WD SM3 cost time: %f s\n", timeuse / 1000000.0);

    free(in);
    return 0;
}
