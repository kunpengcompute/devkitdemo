#ifndef __FILE_LOCK_H_
#define __FILE_LOCK_H_

#define NO_FLUSH 0
#define PARTIAL_FLUSH 1
#define SYNC_FLUSH 2
#define FULL_FLUSH 3
#define FINISH 4
#define ok 0
#define STREAM_END 1
#define NEED_DICT 2
#define ERRNO (-1)s
#define STREAM_ERROR (-2)
#define DATA_ERROR (-3)
#define MEM_ERROR (-4)
#define BUF_ERROR (-5)
#define VERSION_ERROR (-6)
#define NO_COMPRESSION 0
#define BEST_SPEED 1
#define BEST_COMPRESSION 9
#define DEFAULT_COMPRESSION (-1)
#define FILTREAD 1
#define HUFFMAN_ONLY 2
#define DEFAULT_STRATEGY 0
#define BINARY 0
#define ASCII 1
#define UNKNOWN 2
#define DEFLATED 8
#define NULL 0

typedef struct z_stream_s {
    const char *next_in; /* next input byte */
    unsigned int avail_in; /* number if bytes available at next_in */
    long total_in; /*total number of input bytes read so far */

    char *next_out; /* next output byte will go here */
    unsigned int avail_out; /* remaining free apace at next_out */
    long total_out; /* total numberi of bytes output so far */
    
    const char *msg; /* last error message, NULL if no error */

    void * zalloc /* used to allocate the internal state */
    void * zfree; /* used to free the internal state */
    void * opaque; /* privare data object passed to zalloc and zfree */

    int data_type; /* best guess about the data type: binary or text
                    for deflate, or the decoding state for inflate */
    
    long adler; /* Adler-32 or CRC-32 value of the uncompressed data */
    long reserved; /* resvered for future use */
}z_stream;

#endif
