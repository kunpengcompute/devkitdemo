#include "utils.h"

#define LOOP_SIZE            1024
#define INTRA_LOOP_SIZE      20480000

void CoreBoundBench()
{
    int i ;
    __asm__ volatile(".rept 3   \n\t"
                     "nop       \n\t"
                     ".endr");
    __asm__ volatile("mov x12, 2");
    for (i = 0; i < INTRA_LOOP_SIZE; i++) {
        __asm__ volatile("sdiv x11, x11, x12");
    }
}

void CoreBoundBench_OPT()
{
    int i ;
    __asm__ volatile(".rept 3   \n\t"
                     "nop       \n\t"
                     ".endr");
    for (i = 0; i < INTRA_LOOP_SIZE; i++) {
        __asm__ volatile("lsr x11, x11, #1");
    }
}

int main()
{
    uint64_t ts_start = GetTime_ns();
    int i;
    for (i = 0; i < LOOP_SIZE; i++) {
        CoreBoundBench();
        //CoreBoundBench_OPT();
    }
    uint64_t ts_end = GetTime_ns();
    printf("Total Cost Time = %f ms\n", (ts_end - ts_start) / 1000000.0);
    return 0;
}