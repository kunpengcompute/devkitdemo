#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ksal/ksal_erasure_code.h>

int main(int argc, char **argv)
{
    struct HecParam param = {4, 2, 4096, 8192, 2, {3, 4}};
    uint8_t tmp[20][8192];
    uint8_t *ptrs[20];
    uint8_t *bptrs[20];
    for (size_t i = 0; i < param.dataNum + param.parityNum; ++i) {
        for (size_t j = 0; j < param.blockSize; j++) {
                tmp[i][j] = (uint8_t)rand() & 0xff;
        }
        ptrs[i] = tmp[i];
        bptrs[i] = tmp[i];
    }
    KsalEcEncode(ptrs, ptrs + param.dataNum, &param);
    bptrs[3] = tmp[18];
    bptrs[4] = tmp[19];
    KsalEcDecode(bptrs, bptrs + param.dataNum, &param);
    if (memcmp(bptrs[3], ptrs[3], param.blockSize) == 0 &&
        memcmp(bptrs[4], ptrs[4], param.blockSize) == 0) {
        printf("decode succ!!\r\n");
    } else {
        printf("decode fail!!\r\n");
    }
    return 0;
}
