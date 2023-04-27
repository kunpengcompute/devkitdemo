/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: do cert management on kunpeng.
 */
#include <stdlib.h>
#include <tee_log.h>
#include <permsrv_api_cert.h>
#include <permsrv_api_legacy.h>
#include <tee_ext_api.h>
#include <securec.h>
#include <time.h>
#include <cert_config.h>

#include "cert_logger.h"

enum {
    SAVE_CERT_CMD   = 1,
    SEARCH_CERT_CMD = 2,
    DEL_CERT_CMD    = 3,
    SEARCH_LOG_CMD  = 4,
    SEND_CRL_CMD    = 5
};

#define ACTION_CRT_EXPORT "export"
#define ACTION_CRT_IMPORT "cert_import"
#define ACTION_CRL_IMPORT "crl_import"
#define ACTION_CRT_REMOVE "remove"
#define ACTION_CRT_UNDEFINED "undefined"
#define MAX_BUFFER_LEN 8192
#define MAX_LOG_BUFFER_LEN 10000
#define BASE_YEAR 1900
#define BASE_MON 1

static void log_action(const char *action, TEE_Result result)
{
    /* format result */
    char *suc = result == TEE_SUCCESS ? "true" : "false";
    /* get system time */
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    struct tm *lt = localtime(&time.tv_sec);
    if (lt == NULL) {
        tloge("get UTC time failed\n");
        return;
    }
    /* format log entry: "[yyyy/mm/dd HH:MM:SS] ACTION: xxx, SUCCESS: true/false " */
    char buf[MAX_LOG_LINE_LEN];
    if (snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
                  "[UTC:%04d/%02d/%02d %02d:%02d:%02d] ACTION: %s, SUCCESS: %s.\n",
                  lt->tm_year + BASE_YEAR, lt->tm_mon + BASE_MON, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec,
                  action, suc) < 0) {
        tloge("format log entry failed\n");
        return;
    }
    /* write into log file on ssa */
    if (cert_log_write(buf) != TEE_SUCCESS)
        tloge("write to log failed\n");
}

/* ----------------------------------------------------------------------------
 *   Trusted Application Entry Points
 * ----------------------------------------------------------------------------
 */

static TEE_Result cert_verify_and_send(uint32_t param_types, TEE_Param params[4])
{
    TEE_Result ret;
    const char *pubkey = g_root_public_key;
    uint32_t pubkey_len = sizeof(g_root_public_key) / sizeof(char);
    if (!check_param_type(param_types,
        TEE_PARAM_TYPE_MEMREF_INPUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE)) {
        tloge("Bad expected parameter types, 0x%x.\n", param_types);
        return TEE_ERROR_BAD_PARAMETERS;
    }

    if (params[0].memref.size == 0 || params[0].memref.size > MAX_BUFFER_LEN || params[0].memref.buffer == NULL) {
        tloge("Bad expected parameter.\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    ret = ta_signing_cert_import(params[0].memref.buffer, params[0].memref.size, pubkey, pubkey_len);
    if (ret != TEE_SUCCESS)
        tloge("cert store failed\n");
    return ret;
}

static TEE_Result crl_send_service(uint32_t param_types, TEE_Param params[4])
{
    TEE_Result ret;
    if (!check_param_type(param_types,
        TEE_PARAM_TYPE_MEMREF_INPUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE)) {
        tloge("Bad expected parameter types, 0x%x.\n", param_types);
        return TEE_ERROR_BAD_PARAMETERS;
    }

    if (params[0].memref.size == 0 || params[0].memref.size > MAX_BUFFER_LEN || params[0].memref.buffer == NULL) {
        tloge("Bad expected parameter.\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    ret = TEE_EXT_crl_cert_process(params[0].memref.buffer, params[0].memref.size);
    if (ret != TEE_SUCCESS)
        tloge("crl send failed\n");
    return ret;
}

static TEE_Result cert_search_service(uint32_t param_types, uint32_t cmd_id, TEE_Param params[4])
{
    TEE_Result ret = TEE_SUCCESS;
    uint32_t limit = params[0].memref.size;
    uint32_t len = 0;
    uint8_t *dst = NULL;
    if (!check_param_type(param_types,
        TEE_PARAM_TYPE_MEMREF_INOUT,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE)) {
        tloge("Bad expected parameter types, 0x%x.\n", param_types);
        return TEE_ERROR_BAD_PARAMETERS;
    }

    if (params[0].memref.size == 0 || params[0].memref.size > MAX_LOG_BUFFER_LEN || params[0].memref.buffer == NULL) {
        tloge("Bad expected parameter.\n");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    dst = (uint8_t *)malloc(params[0].memref.size);
    if (dst == NULL) {
        tloge("malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }

    switch (cmd_id) {
    case SEARCH_CERT_CMD:
        ret = ta_signing_cert_export((uint8_t *)dst, &len, limit);
        break;
    case SEARCH_LOG_CMD:
        ret = cert_log_read((char *)dst, limit, &len);
        if (len < limit) {
            dst[len++] = '\0';
        } else {
            dst[limit - 1] = '\0';
            len = limit;
        }
        break;
    default:
        break;
    }
    if (memcpy_s(params[0].memref.buffer, limit, dst, len) != EOK) {
        free(dst);
        dst = NULL;
        return TEE_ERROR_SECURITY;
    }

    params[0].memref.size = len;
    if (ret != TEE_SUCCESS)
        tloge("cert search failed\n");
    free(dst);
    dst = NULL;
    return ret;
}

static TEE_Result cert_delete_service(uint32_t param_types)
{
    TEE_Result ret;
    if (!check_param_type(param_types,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE,
        TEE_PARAM_TYPE_NONE)) {
        tloge("Bad expected parameter types, 0x%x.\n", param_types);
        return TEE_ERROR_BAD_PARAMETERS;
    }
    ret = ta_signing_cert_destroy();
    if (ret != TEE_SUCCESS)
        tloge("cert delete failed\n");
    return ret;
}

/**
 *  Function TA_CreateEntryPoint
 *  Description:
 *    The function TA_CreateEntryPoint is the Trusted Application's constructor,
 *    which the Framework calls when it creates a new instance of the Trusted Application.
 */
TEE_Result TA_CreateEntryPoint(void)
{
    TEE_Result ret = addcaller_ca_exec(CERT_MANAGER_DEPLOY_PATH, CERT_MANAGER_DEPLOY_USER);
    if (ret != TEE_SUCCESS)
        tloge("TA_CreateEntryPoint: AddCaller_CA_exec failed.\n");
    return ret;
}

/**
 *  Function TA_OpenSessionEntryPoint
 *  Description:
 *    The Framework calls the function TA_OpenSessionEntryPoint
 *    when a client requests to open a session with the Trusted Application.
 *    The open session request may result in a new Trusted Application instance
 *    being created.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes,
    TEE_Param params[4], void** sessionContext)
{
    /* -Wunused-parameter */
    (void)paramTypes;
    /* -Wunused-parameter */
    (void)params;
    /* -Wunused-parameter */
    (void)sessionContext;
    SLogTrace("---- TA_OpenSessionEntryPoint -------- ");
    return TEE_SUCCESS;
}

/**
 *  Function TA_InvokeCommandEntryPoint
 *  Description:
 *    The Framework calls this function when the client invokes a command
 *    within the given session.
 */
TEE_Result TA_InvokeCommandEntryPoint(void* sessionContext, uint32_t cmd_id,
    uint32_t paramTypes, TEE_Param params[4])
{
    /* -Wunused-parameter */
    (void)sessionContext;
    TEE_Result ret;
    char *action = NULL;
    switch (cmd_id) {
    case SAVE_CERT_CMD:
        action = ACTION_CRT_IMPORT;
        ret = cert_verify_and_send(paramTypes, params);
        if (ret != TEE_SUCCESS)
            tloge("certificate restoring failed\n");
        break;
    case SEND_CRL_CMD:
        action = ACTION_CRL_IMPORT;
        ret = crl_send_service(paramTypes, params);
        if (ret != TEE_SUCCESS)
            tloge("crl restoring failed\n");
        break;
    case SEARCH_CERT_CMD:
    /* fall through: to be handled with the same function as SEARCH_LOG_CMD case */
    case SEARCH_LOG_CMD:
        action = ACTION_CRT_EXPORT;
        ret = cert_search_service(paramTypes, cmd_id, params);
        if (ret != TEE_SUCCESS)
            tloge("certificate searching failed\n");
        break;
    case DEL_CERT_CMD:
        action = ACTION_CRT_REMOVE;
        ret = cert_delete_service(paramTypes);
        if (ret != TEE_SUCCESS)
            tloge("certificate delete failed\n");
        break;
    default:
        action = ACTION_CRT_UNDEFINED;
        ret = TEE_ERROR_BAD_PARAMETERS;
        break;
    }
    log_action(action, ret);
    return ret;
}

/**
 *  Function TA_CloseSessionEntryPoint
 *  Description:
 *    The Framework calls this function to close a client session.
 *    During the call to this function the implementation can use
 *    any session functions.
 */
void TA_CloseSessionEntryPoint(void* sessionContext)
{
    /* -Wunused-parameter */
    (void)sessionContext;
    SLogTrace("---- TA_CloseSessionEntryPoint ----- ");
}

/**
 *  Function TA_DestroyEntryPoint
 *  Description:
 *    The function TA_DestroyEntryPoint is the Trusted Application's destructor,
 *    which the Framework calls when the instance is being destroyed.
 */
void TA_DestroyEntryPoint(void)
{
    SLogTrace("---- TA_DestroyEntryPoint ---- ");
}
