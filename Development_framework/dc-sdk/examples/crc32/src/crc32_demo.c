#include <stdio.h>
#include <stdlib.h>
#include <ksal/ksal_crc.h>

int main(int argc, char **argv)
{
    uint8_t buf[4096];
    for (size_t i = 0; i < sizeof(buf); ++i) {
        buf[i] = (int8_t)i & 0xff;
    }
    uint32_t crc = KsalCrc32c(0x123, buf, sizeof(buf));
    printf("crc = 0x%x\r\n", crc);
    return 0;
}
