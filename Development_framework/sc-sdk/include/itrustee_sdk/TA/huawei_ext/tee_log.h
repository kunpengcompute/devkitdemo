/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: Reference of TEE log api and internal definitions
 */

#ifndef __TEE_LOG_H
#define __TEE_LOG_H

#include "tee_defines.h"
#include "tee_log_legacy.h"

#define TA_LOG_LEVEL_ERROR   0
#define TA_LOG_LEVEL_WARNING 1
#define TA_LOG_LEVEL_INFO    2
#define TA_LOG_LEVEL_DEBUG   3
#define TA_LOG_LEVEL_VERBO   4

#define TA_LOG_LEVEL_DEFAULT  TA_LOG_LEVEL_INFO
// TA_LOG_LEVEL can be redefined by TA developers
#ifndef TA_LOG_LEVEL
#define TA_LOG_LEVEL TA_LOG_LEVEL_DEFAULT
#endif

#define TAG_VERB  "[VERB]"
#define TAG_DEBUG "[DEBUG]"
#define TAG_INFO  "[INFO]"
#define TAG_WARN  "[WARN]"
#define TAG_ERROR "[ERROR]"

#define DEBUG_TAG   "[DEBUG]"
#define INFO_TAG    "[INFO]"
#define WARNING_TAG "[WARN]"
#define ERROR_TAG   "[ERROR]"
typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN  = 1,
    LOG_LEVEL_INFO  = 2,
    LOG_LEVEL_DEBUG = 3,
    LOG_LEVEL_VERBO = 4,
    LOG_LEVEL_ON    = 5,
} LOG_LEVEL;

void uart_cprintf(const char *fmt, ...);
void uart_printf_func(const char *fmt, ...);

void tee_print(LOG_LEVEL log_level, const char *fmt, ...);
void tee_print_driver(LOG_LEVEL log_level, const char *log_tag, const char *fmt, ...);
extern const char *g_debug_prefix;

/* tlogv */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_VERBO)
#ifdef DRIVER_LOG_TAG
#define tlogv(fmt, args...) \
    tee_print_driver(LOG_LEVEL_VERBO, DRIVER_LOG_TAG, "%s %d:" fmt "", TAG_VERB, __LINE__, ##args)
#else
#define tlogv(fmt, args...) tee_print(LOG_LEVEL_VERBO, "%s %d:" fmt "", TAG_VERB, __LINE__, ##args)
#endif /* DRIVER_LOG_TAG */
#else
#define tlogv(fmt, args...) \
    do {                    \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_VERBO */

/* tlogd */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_DEBUG)
#ifdef DRIVER_LOG_TAG
#define tlogd(fmt, args...) \
    tee_print_driver(LOG_LEVEL_DEBUG, DRIVER_LOG_TAG, "%s %d:" fmt "", TAG_DEBUG, __LINE__, ##args)
#else
#define tlogd(fmt, args...) tee_print(LOG_LEVEL_DEBUG, "%s %d:" fmt "", TAG_DEBUG, __LINE__, ##args)
#endif /* DRIVER_LOG_TAG */
#else
#define tlogd(fmt, args...) \
    do {                    \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_DEBUG */

/* tlogi */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_INFO)
#ifdef DRIVER_LOG_TAG
#define tlogi(fmt, args...) \
    tee_print_driver(LOG_LEVEL_INFO, DRIVER_LOG_TAG, "%s %d:" fmt "", TAG_INFO, __LINE__, ##args)
#else
#define tlogi(fmt, args...) tee_print(LOG_LEVEL_INFO, "%s %d:" fmt "", TAG_INFO, __LINE__, ##args)
#endif /* DRIVER_LOG_TAG */
#else
#define tlogi(fmt, args...) \
    do {                    \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_INFO */

/* tlogw */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_WARNING)
#ifdef DRIVER_LOG_TAG
#define tlogw(fmt, args...) \
    tee_print_driver(LOG_LEVEL_WARN, DRIVER_LOG_TAG, "%s %d:" fmt "", TAG_WARN, __LINE__, ##args)
#else
#define tlogw(fmt, args...) tee_print(LOG_LEVEL_WARN, "%s %d:" fmt "", TAG_WARN, __LINE__, ##args)
#endif /* DRIVER_LOG_TAG */
#else
#define tlogw(fmt, args...) \
    do {                    \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_WARNING */

/* tloge */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_ERROR) // Always meet this condition
#ifndef TLOGE_NO_TIMESTAMP
#ifdef DRIVER_LOG_TAG
#define tloge(fmt, args...) \
    tee_print_driver(LOG_LEVEL_ERROR, DRIVER_LOG_TAG, "%s %d:" fmt " ", TAG_ERROR, __LINE__, ##args)
#else
#define tloge(fmt, args...) tee_print(LOG_LEVEL_ERROR, "%s %d:" fmt " ", TAG_ERROR, __LINE__, ##args)
#endif /* DRIVER_LOG_TAG */
#else
#define tloge(fmt, args...) printf("[%s] %s %d:" fmt " ", g_debug_prefix, TAG_ERROR, __LINE__, ##args)
#endif /* TLOGE_NO_TIMESTAMP */
#else
#define tloge(fmt, args...) \
    do {                    \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_ERROR */

void ta_print(LOG_LEVEL log_level, const char *fmt, ...);

/* ta_logv */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_VERBO)
#define ta_logv(fmt, args...) ta_print(LOG_LEVEL_VERBO, "%s %d: " fmt "\n", TAG_VERB, __LINE__, ##args)
#else
#define ta_logv(fmt, args...) \
    do {                      \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_VERBO */

/* ta_logd */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_DEBUG)
#define ta_logd(fmt, args...) ta_print(LOG_LEVEL_DEBUG, "%s %d: " fmt "\n", TAG_DEBUG, __LINE__, ##args)
#else
#define ta_logd(fmt, args...) \
    do {                      \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_DEBUG */

/* ta_logi */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_INFO)
#define ta_logi(fmt, args...) ta_print(LOG_LEVEL_INFO, "%s %d: " fmt "\n", TAG_INFO, __LINE__, ##args)
#else
#define ta_logi(fmt, args...) \
    do {                      \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_INFO */

/* ta_logw */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_WARNING)
#define ta_logw(fmt, args...) ta_print(LOG_LEVEL_WARN, "%s %d: " fmt "\n", TAG_WARN, __LINE__, ##args)
#else
#define ta_logw(fmt, args...) \
    do {                      \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_WARNING */

/* ta_loge */
#if (TA_LOG_LEVEL >= TA_LOG_LEVEL_ERROR) // Always meet this condition
#define ta_loge(fmt, args...) ta_print(LOG_LEVEL_ERROR, "%s %d: " fmt "\n", TAG_ERROR, __LINE__, ##args)
#else
#define ta_loge(fmt, args...) \
    do {                      \
    } while (0)
#endif /* TA_LOG_LEVEL >= TA_LOG_LEVEL_ERROR */

/*
in debug version,   users can dynamically modify the loglevel,
in release version, users have to modify the level by compile
*/
#ifndef DEBUG_VERSION
#ifdef TA_DEBUG
#define ta_debug(fmt, args...) uart_printf_func("%s %s: " fmt "", DEBUG_TAG, __FUNCTION__, ##args)
#else
#define ta_debug(fmt, args...)
#endif /* TA_DEBUG */
#else
#define ta_debug(fmt, args...)                                                   \
    do {                                                                         \
        uint32_t level;                                                          \
        level = get_value();                                                     \
        if (level >= LEVEL_DEBUG) {                                              \
            uart_printf_func("%s %s: " fmt "", DEBUG_TAG, __FUNCTION__, ##args); \
        }                                                                        \
    } while (0)

#define ta_warning(fmt, args...)                                                 \
    do {                                                                         \
        uint32_t level;                                                          \
        level = get_value();                                                     \
        if (level >= LEVEL_WARNING) {                                            \
            uart_printf_func("%s %s: " fmt "", DEBUG_TAG, __FUNCTION__, ##args); \
        }                                                                        \
    } while (0)

#endif /* DEBUG_VERSION */

#define ta_info(fmt, args...)  uart_printf_func("%s: " fmt "", INFO_TAG, ##args)
#define ta_error(fmt, args...) uart_printf_func("%s: " fmt " ", ERROR_TAG, ##args)

/* Log level for SLogx */
#define TRACE_S   "[TRACE]"
#define WARNING_S "[WARN]"
#define ERROR_S   "[ERROR]"

#define TA_SLOG_LEVEL_ERROR   0
#define TA_SLOG_LEVEL_WARNING 1
#define TA_SLOG_LEVEL_TRACE   2

#define TA_SLOG_LEVEL_DEFAULT  TA_SLOG_LEVEL_WARNING
// TA_SLOG_LEVEL can be redefined by TA developers
#ifndef TA_SLOG_LEVEL
#define TA_SLOG_LEVEL TA_SLOG_LEVEL_DEFAULT
#endif

/* SLogTrace */
#if (TA_SLOG_LEVEL >= TA_SLOG_LEVEL_TRACE)
#define SLogTrace(fmt, args...) tee_print(LOG_LEVEL_DEBUG, "%s: " fmt "\n", TRACE_S, ##args)
#else
#define SLogTrace(fmt, args...) \
    do {                        \
    } while (0)
#endif

/* SLogWarning */
#if (TA_SLOG_LEVEL >= TA_SLOG_LEVEL_WARNING)
#define SLogWarning(fmt, args...) tee_print(LOG_LEVEL_WARN, "%s: " fmt "\n", WARNING_S, ##args)
#else
#define SLogWarning(fmt, args...) \
    do {                          \
    } while (0)
#endif

/* SLogError */
#if (TA_SLOG_LEVEL >= TA_SLOG_LEVEL_ERROR)
#define SLogError(fmt, args...) tee_print(LOG_LEVEL_ERROR, "%s: " fmt "\n", ERROR_S, ##args)
#else
#define SLogError(fmt, args...)   \
    do {                          \
    } while (0)
#endif

/* SAssert, Assert api for tee log, note: should call Panic to deal, here just return */
#define SAssert(exp)                                                                         \
    do {                                                                                     \
        if (!(exp)) {                                                                        \
            tee_print(LOG_LEVEL_ERROR, "Assertion [%s] Failed: File %s, Line %d\n", #exp, __FILE__, __LINE__); \
            return 0xFFFF0001;                                                               \
        }                                                                                    \
    } while (0);

/*
 * Output log to tee log file.
 *
 * @param fmt [IN]    Printf log's format config style.
 *
 * @return void
 */
void slog(const char *fmt, ...);
uint32_t get_value(void);
#define TEE_LogPrintf(fmt, args...) slog(fmt, ##args)
#endif /* __TEE_LOG_H */
