/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef DAS_H
#define DAS_H
#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif

enum DAS_RESULT {
    RETURN_DAS_FULL = -2,
    RETURN_DAS_ERROR = -1,
    RETURN_DAS_OK = 0,
    RETURN_DAS_EMPTY = 1,
    RETURN_DAS_DELETING = 2,
    RETURN_DAS_REJECT = 3,
};

enum DasLogLvl {
    DAS_LOGLVL_ERR = 0,
    DAS_LOGLVL_WAR,
    DAS_LOGLVL_INF,
    DAS_LOGLVL_DBG,
};

typedef struct DasKvParam {
    uint64_t offset;
    uint64_t len;
    uint64_t timeStamp;
    uint32_t lunID;
} DasKvParam;

typedef struct DasOPS {
    void (*SubmitDasPrefetch)(struct DasKvParam* params);
    void (*logFunc)(void *logger, enum DasLogLvl level, const char *format, ...);
} DasOPS;

/* Param to create DAS */
typedef struct DasModuleParam {
    struct DasOPS *ops;
    void *logger;
    uint64_t cacheLineSize;
} DasModuleParam;

int32_t Rcache_CeateDasModule(DasModuleParam *createInstanceParam);

void Rcache_ExitDasModule(void);

int32_t Rcache_PutDasInfo(DasKvParam *params);
#ifdef __cplusplus
}
#endif

#endif
