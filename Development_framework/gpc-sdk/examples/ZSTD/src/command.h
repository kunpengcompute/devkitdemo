#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>    
#include <stdio.h>   
#include <string.h>   
#include <errno.h>
#include <sys/stat.h>
#include <queue>
#include <vector>
#include "zstd/zstd.h"

#define CHECK(cond, ...)                        \
    do {                                        \
        if (!(cond)) {                          \
            fprintf(stderr,                     \
                    "%s:%d CHECK(%s) failed: ", \
                    __FILE__,                   \
                    __LINE__,                   \
                    #cond);                     \
            fprintf(stderr, "" __VA_ARGS__);    \
            fprintf(stderr, "\n");              \
            exit(1);                            \
        }                                       \
    } while (0)

#define CHECK_ZSTD(fn, ...)                                      \
    do {                                                         \
        size_t const err = (fn);                                 \
        CHECK(!ZSTD_isError(err), "%s", ZSTD_getErrorName(err)); \
    } while (0)

class TestZSTD {
public:
    TestZSTD(int num, const char **str);
    ~TestZSTD();
    char* CreateOutFileName(const char *filename) const;
    // Simple Compress
    void Compress();
    // Simple Decompress
    void Decompress();
    // Multiple Compress
    void MultipleCompress();
    // Multiple Decompress
    void MultipleDecompress();
    void DictionaryCompress(const char *inFileName, const char *outFileName, const ZSTD_CDict* dictPtr);
    void MultipleDictionaryCompress();

    void ResetInFileNames(std::vector<const char *> newArgs);
private:
    TestZSTD(TestZSTD &t) = delete;
    TestZSTD& operator=(const TestZSTD&t) = delete;
    std::queue<const char *> inFileNames;
    // Define the returned error code from utility functions
    enum error_type {
        ERROR_fsize = 1,
        ERROR_fopen = 2,
        ERROR_fclose = 3,
        ERROR_fread = 4,
        ERROR_fwrite = 5,
        ERROR_loadFile = 6,
        ERROR_malloc = 7,
    };
    // Get the size of a given file path
    size_t Fsize(const char *fileName) const;

    // Open a file using given file path and open option.
    FILE* Fopen(const char *fileName, const char *mode) const;

    // Close an opened file using given FILE pointer
    void Fclose(FILE *file) const;

    // Read sizeToRead bytes from a given file, storing them at the location given by buffer.
    size_t Fread(void *buffer, size_t sizeToRead, FILE *file) const;

    // Write sizeToWrite bytes to a file pointed to by file, obtaining them from a location given by buffer.
    size_t Fwrite(const void *buffer, size_t sizeToWrite, FILE *file) const;

    // Allocate memory
    void* Malloc(size_t size) const;

    // load file into buffer (memory)
    size_t LoadFile(const char *fileName, void *buffer, size_t bufferSize) const;

    // allocate memory buffer and then load file into it
    void* MallocAndLoadFile(const char *fileName, size_t *bufferSize) const;

    // Save buffSize bytes to a given file path, obtaining them from a location pointed to by buff
    void SaveFile(const char *fileName, const void *buff, size_t buffSize) const;
    
    ZSTD_CDict* CreateCDict(const char* dictFileName, int cLevel);
};

#endif
