#ifndef HPC_UTILS_H
#define HPC_UTILS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <omp.h>
#include <sched.h>
#include <string.h>

#define SEC2NS  1000000000

typedef struct Point_t {
    double x;
    double y;
}Point;

static inline uint64_t GetTime_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * SEC2NS + ts.tv_nsec;
}

static inline uint64_t xorshf96()
{
    static uint64_t x=123456789;
    static uint64_t y=362436069;
    static uint64_t z=521288629;
    uint64_t t;
    
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
    
    t = x;
    x = y;
    y = z;
    
    z = t ^ x ^ y;
    return z;
}

static inline uint64_t my_random()
{
    return xorshf96();
}

static inline double random_double(uint32_t range)
{
    return (my_random() % range) * (double)(1.0);
}

static inline Point *InitPointsVector(int len)
{
    Point *pointsVector = (Point*)malloc(len * sizeof(Point));
    if (pointsVector == NULL) {
        printf("ERROR: Memory Allocate Failed!\n");
        exit(1);
    }
    int i;
    #pragma omp parallel for
    for (i = 0; i < len; i++) {
        pointsVector[i].x = random_double(len);
        pointsVector[i].y = random_double(len);
    }
    return pointsVector;
}

static inline double ComputPointDistance(Point *pointA, Point *pointB)
{
    double horizontal, vertical, pointDis;
    horizontal = pointA->x - pointB->x;
    vertical = pointA->y - pointB->y;
    pointDis = sqrt((horizontal * horizontal) + (vertical + vertical));
    return pointDis;
}

void set_thread_affinity(uint16_t core_id)
{
    cpu_set_t cpu_mask;
    cpu_set_t get;
    
    CPU_ZERO(&cpu_mask);
    CPU_SET(core_id, &cpu_mask);
    
    if (sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask) == -1)
    {
        printf("set cpu affinity failed...\n");
        exit(1);
    }
    
    if (sched_setaffinity(0, sizeof(get), &get) == -1)
    {
        printf("get affinity failed...\n");
        exit(1);
    }
}

#endif