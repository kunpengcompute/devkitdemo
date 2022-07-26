#include "command.h"

TestZSTD::TestZSTD(int num, const char **str) : inFileNames(std::queue<const char *>()) {
    for (int i = 1; i < num; i++) {
        inFileNames.push(str[i]);
    }
}

TestZSTD::~TestZSTD() {}

size_t TestZSTD::Fsize(const char *fileName) const {
    struct stat statBuf;
    size_t fileSize;
    if (stat(fileName, &statBuf) != 0) {
        perror(fileName);
        exit(ERROR_fsize);
    }
    fileSize = statBuf.st_size;
    return fileSize;
}

FILE *TestZSTD::Fopen(const char *fileName, const char *mode) const {
    FILE *const inFile = fopen(fileName, mode);
    if (inFile)
        return inFile;
    exit(ERROR_fopen);
}

void TestZSTD::Fclose(FILE *file) const {
    if (!fclose(file)) {
        return;
    };
    exit(ERROR_fclose);
}

size_t TestZSTD::Fread(void *buffer, size_t sizeToRead, FILE *file) const {
    size_t const readSize = fread(buffer, 1, sizeToRead, file);
    if (readSize == sizeToRead) {
        return readSize;
    }
    if (feof(file)) {
        return readSize;
    }
    exit(ERROR_fread);
}

size_t TestZSTD::Fwrite(const void *buffer, size_t sizeToWrite, FILE *file) const {
    size_t const writtenSize = fwrite(buffer, 1, sizeToWrite, file);
    if (writtenSize == sizeToWrite) {
        return sizeToWrite;
    }
    exit(ERROR_fwrite);
}

void *TestZSTD::Malloc(size_t size) const {
    void *const buff = malloc(size);
    if (buff) {
        return buff;
    }
    exit(ERROR_malloc);
}

size_t TestZSTD::LoadFile(const char *fileName, void *buffer, size_t bufferSize) const {
    size_t const fileSize = Fsize(fileName);
    CHECK(fileSize <= bufferSize, "File too large!");

    FILE *const inFile = fopen(fileName, "rb");

    size_t const readSize = fread(buffer, 1, fileSize, inFile);

    if (readSize != (size_t)fileSize) {
        exit(ERROR_fread);
    }
    fclose(inFile);
    return fileSize;
}

void *TestZSTD::MallocAndLoadFile(const char *fileName, size_t *bufferSize) const {
    size_t const fileSize = Fsize(fileName);
    *bufferSize = fileSize;
    void *const buffer = Malloc(*bufferSize);
    LoadFile(fileName, buffer, *bufferSize);
    return buffer;
}

void TestZSTD::SaveFile(const char *fileName, const void *buff, size_t buffSize) const {
    FILE *const oFile = Fopen(fileName, "wb");
    size_t const wSize = fwrite(buff, 1, buffSize, oFile);
    if (wSize != (size_t)buffSize) {
        exit(ERROR_fwrite);
    }
    if (fclose(oFile)) {
        exit(ERROR_fclose);
    }
}

char *TestZSTD::CreateOutFileName(const char *filename) const {
    size_t const inL = strlen(filename);
    size_t const outL = inL + 5;
    void *const outSpace = Malloc(outL);
    memset(outSpace, 0, outL);
    strcat((char *)outSpace, filename);
    strcat((char *)outSpace, ".zst");
    return (char *)outSpace;
}

ZSTD_CDict* TestZSTD::CreateCDict(const char* dictFileName, int cLevel) {
    size_t dictSize;
    printf("loading dictionary %s \n", dictFileName);
    void* const dictBuffer = MallocAndLoadFile(dictFileName, &dictSize);
    ZSTD_CDict* const cdict = ZSTD_createCDict(dictBuffer, dictSize, cLevel);
    CHECK(cdict != NULL, "ZSTD_createCDict() failed!");
    free(dictBuffer);
    return cdict;
}

void TestZSTD::Compress() {
    const char *inFileName = inFileNames.front();
    const char *outFileName = CreateOutFileName(inFileName);
    inFileNames.pop();
    size_t fSize;
    void *const fBuff = MallocAndLoadFile(inFileName, &fSize);
    size_t const cBuffSize = ZSTD_compressBound(fSize);
    void *const cBuff = Malloc(cBuffSize);

    size_t const cSize = ZSTD_compress(cBuff, cBuffSize, fBuff, fSize, 1);
    CHECK_ZSTD(cSize);
    SaveFile(outFileName, cBuff, cSize);

    /* success */
    printf("%25s : %6u -> %7u - %s \n", inFileName, static_cast<unsigned>(fSize), 
            static_cast<unsigned>(cSize), outFileName);

    free(fBuff);
    free(cBuff);
    free(const_cast<char *>(outFileName));
    return;
}

void TestZSTD::Decompress() {
    size_t cSize;
    const char *inFileName = inFileNames.front();
    void* const cBuff = MallocAndLoadFile(inFileName, &cSize);
    inFileNames.pop();
    unsigned long long const rSize = ZSTD_getFrameContentSize(cBuff, cSize);
    CHECK(rSize != ZSTD_CONTENTSIZE_ERROR, "%s: not compressed by zstd!", inFileName);
    CHECK(rSize != ZSTD_CONTENTSIZE_UNKNOWN, "%s: original size unknown!", inFileName);

    void* const rBuff = Malloc((size_t)rSize);

    size_t const dSize = ZSTD_decompress(rBuff, rSize, cBuff, cSize);
    CHECK_ZSTD(dSize);
    /* When zstd knows the content size, it will error if it doesn't match. */
    CHECK(dSize == rSize, "Impossible because zstd will check this condition!");

    /* success */
    printf("%25s : %6u -> %7u \n", inFileName, static_cast<unsigned>(cSize), 
            static_cast<unsigned>(rSize));

    free(rBuff);
    free(cBuff);
    return ;
}

void TestZSTD::MultipleCompress() {
    while (!inFileNames.empty()) {
        this->Compress();
    }
    return ;
}

void TestZSTD::MultipleDecompress() {
    while (!inFileNames.empty()) {
        this->Decompress();
    }
    return ;
}

void TestZSTD::DictionaryCompress(const char *inFileName, const char *outFileName, const ZSTD_CDict* dictPtr) {
    size_t fSize;
    void* const fBuff = MallocAndLoadFile(inFileName, &fSize);
    size_t const cBuffSize = ZSTD_compressBound(fSize);
    void* const cBuff = Malloc(cBuffSize);
    ZSTD_CCtx* const cctx = ZSTD_createCCtx();
    CHECK(cctx != NULL, "ZSTD_createCCtx() failed!");
    size_t const cSize = ZSTD_compress_usingCDict(cctx, cBuff, cBuffSize, fBuff, fSize, dictPtr);
    CHECK_ZSTD(cSize);
    SaveFile(outFileName, cBuff, cSize);
    printf("%25s : %6u -> %7u - %s \n", inFileName, static_cast<unsigned>(fSize), 
            static_cast<unsigned>(cSize), outFileName);
    ZSTD_freeCCtx(cctx);
    free(fBuff);
    free(cBuff);
    free(const_cast<char *>(outFileName));
    return;
}

void TestZSTD::MultipleDictionaryCompress() {
    const char* const dictName = inFileNames.back();
    ZSTD_CDict* const dictPtr = CreateCDict(dictName, 3);

    while (inFileNames.size() > 1) {
        const char* inFileName = inFileNames.front();
        inFileNames.pop();
        char* const outFileName = CreateOutFileName(inFileName);
        DictionaryCompress(inFileName, outFileName, dictPtr);
    }

    ZSTD_freeCDict(dictPtr);
    return;
}

void TestZSTD::ResetInFileNames(std::vector<const char *> newArgs) {
    while (!inFileNames.empty()) {
        inFileNames.pop();
    }
    for (int i = 0; i < newArgs.size(); i++) {
        inFileNames.push(newArgs[i]);
    }
    return;
}