/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: vsl struc enum definition header.
 * Author: zky
 * Create: 2022-06-30
 */
#ifndef VSL_TYPES_H
#define VSL_TYPES_H
#include <stdint.h>
#include <arm_neon.h>

typedef enum {
    VSL_OK,
    VSL_ILLEGAL_PARM,
    VSL_INVALID_TYPE,
    VSL_WARNING,
    VSL_NULL_PTR
} KmlVslResult;
typedef enum {
    WICHMANN_HILL,
    MARSAGLIA_MULTICARRY,
    SUPER_DUPER,
    MERSENNE_TWISTER,
    LECUYER_CMRG
} RNGtype;
typedef enum {
    BUGGY_KINDERMAN_RAMAGE,
    AHRENS_DIETER,
    BOX_MULLER,
    INVERSION,
    KINDERMAN_RAMAGE
} N01type;

typedef struct VslPolicy {
    uint32_t dummy[628]; // allow for optimizing compilers to read over bound
    RNGtype kind;
    N01type normalMethod;
    double BMNormKeep; // to keep box muller last rand
    uint32_t seed;
    void (*init)(struct VslPolicy *policy, uint32_t seed, RNGtype kind);
    void (*init_vec)(struct VslPolicy *policy, uint32_t seed, RNGtype kind);
    double (*UnifRand)(struct VslPolicy *policy);
    void (*UnifRandVec)(struct VslPolicy *policy);
    void (*fix)(struct VslPolicy *policy, RNGtype RNG_kind);
    void (*fix_vec)(struct VslPolicy *policy, RNGtype RNG_kind);
    uint32_t mti; // for MT only /* mti==MT_LEN+1 means mt[MT_LEN] is not initialized */
    uint32x4_t dummyVec[628];
    float64x2_t dst[2];
    float64x2_t normDst[2];
} VslPolicy;
#endif