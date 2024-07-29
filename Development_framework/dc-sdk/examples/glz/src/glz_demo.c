#include <stdio.h>
#include <stdlib.h>
#include "glz.h"

#define GLZ_DEFAULT_LEVEL 1

extern int glz_compress_default();
extern int glz_decompress_default();
extern void glz_compress_delete();
extern glz_encoder *glz_compress_initial();

int main() {
    FILE *inputFile;
    char *testInput;
    int testCompressSize;

    inputFile = fopen("../dickens", "r");
    if (inputFile == NULL) {
        printf("Error opening file!\n");
        return -1;
    }
    
    /* 获取待压缩文件大小 */
    fseek(inputFile, 0, SEEK_END);
    testCompressSize = ftell(inputFile);
    fseek(inputFile, 0 , SEEK_SET);
    
    testInput = (char*)malloc(testCompressSize);
    if (testInput == NULL) {
        printf("Error allocating memory!\n");
        fclose(inputFile);
        return -1;
    }

    /* 读取文件内容到buffer */
    fread(testInput, testCompressSize, 1, inputFile); 
    
    glz_encoder* glz_stream = NULL;
    glz_stream = glz_compress_initial((long unsigned)testCompressSize);
    if (glz_stream == NULL) {
        free(testInput);
        printf("glz_stream init fail!\n");
        return -1;
    }
    
    /* TEST glz_level 0 compress */
    encode_model glz_mode = GLZ_FAST;
    char* testCompressed = (char*)malloc(testCompressSize);
    if (testCompressed == NULL) {
        free(testInput);
        glz_compress_delete(&glz_stream);
        printf("Error allocating memory!\n");
        return -1;
    }
    int cs = glz_compress_default(glz_stream, testCompressed, testCompressSize,
    testInput, testCompressSize, glz_mode, GLZ_DEFAULT_LEVEL);

    if (cs <= 0){ 
        glz_compress_delete(&glz_stream);
        free(testInput);
        free(testCompressed);   
        printf("TEST glz level 0 compress failed!\n");
	    return -1;
    }
    printf("TEST glz level 0 compress success!\norigin size=%d,after decompress=%d,compress ratio=%.2f\n", 
        testCompressSize, cs, testCompressSize * 1.0 / cs);

    /* TEST glz_level 0 decompress */
    char* testDecompressed = (char*)malloc(testCompressSize);
    if (testDecompressed == NULL) {
        free(testInput);
        free(testCompressed); 
        glz_compress_delete(&glz_stream);
        printf("Error allocating memory!\n");
        return -1;
    }
    int dcs = glz_decompress_default(glz_stream, testDecompressed, testCompressSize,
    testCompressed, testCompressSize, glz_mode);
    if (dcs <= 0){ 
        glz_compress_delete(&glz_stream);
        free(testInput);
        free(testCompressed);
        free(testDecompressed);
        printf("TEST glz level 0 decompress failed!\n");
	    return -1;
    }
    printf("TEST glz level 0 decompress success!\norigin size=%d,after decompress=%d\n", cs, dcs);

    /* TEST glz_level 1 compress */
    glz_mode = GLZ_HIGH_CR;
    char* testCompressed1 = (char*)malloc(testCompressSize);
    if (testCompressed1 == NULL) {
        free(testInput);
        free(testCompressed);
        free(testDecompressed); 
        glz_compress_delete(&glz_stream);
        printf("Error allocating memory!\n");
        return -1;
    }
    int cs_1 = glz_compress_default(glz_stream, testCompressed1, testCompressSize,
    testInput, testCompressSize, glz_mode, GLZ_DEFAULT_LEVEL);

    if (cs <= 0){ 
        glz_compress_delete(&glz_stream);
        free(testInput);
        free(testCompressed);
        free(testDecompressed);
        free(testCompressed1);   
        printf("TEST glz level 1 compress failed!\n");
	    return -1;
    }
    printf("TEST glz level 1 compress success!\norigin size=%d,after decompress=%d,compress ratio=%.2f\n", 
        testCompressSize, cs_1, testCompressSize * 1.0 / cs_1);

    /* TEST glz_level 1 decompress */
    char* testDecompressed1 = (char*)malloc(testCompressSize);
    if (testDecompressed1 == NULL) {
        free(testInput);
        free(testCompressed);
        free(testDecompressed);
        free(testCompressed1);  
        glz_compress_delete(&glz_stream);
        printf("Error allocating memory!\n");
        return -1;
    }
    int dcs_1 = glz_decompress_default(glz_stream, testDecompressed1, testCompressSize,
    testCompressed1, testCompressSize, glz_mode);
    if (dcs <= 0){ 
        glz_compress_delete(&glz_stream);
        free(testInput);
        free(testCompressed);
        free(testDecompressed);
        free(testCompressed1);
        free(testDecompressed1);  
        printf("TEST glz level 1 decompress failed!\n");
	    return -1;
    }
    printf("TEST glz level 1 decompress success!\norigin size=%d,after decompress=%d\n", cs_1, dcs_1);

    glz_compress_delete(&glz_stream);
    free(testInput);
    free(testCompressed);
    free(testDecompressed);
    free(testCompressed1);
    free(testDecompressed1);  
    return 0;
}

