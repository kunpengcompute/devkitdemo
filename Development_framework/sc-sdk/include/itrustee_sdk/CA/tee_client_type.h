/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: data type and structure definition according to GP
 */

#ifndef _TEE_CLIENT_TYPE_H_
#define _TEE_CLIENT_TYPE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <semaphore.h>
#include "tee_client_list.h"
#include "tee_client_constants.h"

typedef enum TEEC_ReturnCode TEEC_Result;

typedef struct {
    uint32_t timeLow;
    uint16_t timeMid;
    uint16_t timeHiAndVersion;
    uint8_t clockSeqAndNode[8];
} TEEC_UUID;

typedef struct {
    int32_t fd;
    uint8_t *ta_path;
    struct ListNode session_list;
    struct ListNode shrd_mem_list;
    union {
        struct {
            void *buffer;
            sem_t buffer_barrier;
        } share_buffer;
        uint64_t imp;          /* for adapt */
    };
} TEEC_Context;

typedef struct {
    uint32_t session_id;
    TEEC_UUID service_id;
    uint32_t ops_cnt;
    union {
        struct ListNode head;
        uint64_t imp;          /* for adapt */
    };
    TEEC_Context *context;
} TEEC_Session;

typedef struct {
    void *buffer;
    uint32_t size;
    uint32_t flags;         /* reference to TEEC_SharedMemCtl */
    uint32_t ops_cnt;
    bool is_allocated;      /* identify whether the memory is registered or allocated */
    union {
        struct ListNode head;
        void* imp;          /* for adapt, imp is not used by system CA, only for vendor CA */
    };
    TEEC_Context *context;
} TEEC_SharedMemory;

/*
 * the corresponding param types are
 * TEEC_MEMREF_TEMP_INPUT/TEEC_MEMREF_TEMP_OUTPUT/TEEC_MEMREF_TEMP_INOUT
 */
typedef struct {
    void *buffer;
    uint32_t size;
} TEEC_TempMemoryReference;

/*
 * the corresponding param types are
 * TEEC_MEMREF_WHOLE/TEEC_MEMREF_PARTIAL_INPUT
 * TEEC_MEMREF_PARTIAL_OUTPUT/TEEC_MEMREF_PARTIAL_INOUT
 */
typedef struct {
    TEEC_SharedMemory *parent;
    uint32_t size;
    uint32_t offset;
} TEEC_RegisteredMemoryReference;

/*
 * the corresponding param types are
 * TEEC_VALUE_INPUT/TEEC_VALUE_OUTPUT/TEEC_VALUE_INOUT
 */
typedef struct {
    uint32_t a;
    uint32_t b;
} TEEC_Value;

typedef struct {
    int ion_share_fd;
    uint32_t ion_size;
} TEEC_IonReference;

typedef union {
    TEEC_TempMemoryReference tmpref;
    TEEC_RegisteredMemoryReference memref;
    TEEC_Value value;
    TEEC_IonReference ionref;
} TEEC_Parameter;

typedef struct {
    uint32_t event_type;    /* Tui event type */
    uint32_t value;         /* return value, is keycode if tui event is getKeycode */
    uint32_t notch;         /* notch size of the screen for tui */
    uint32_t width;         /* width of foldable screen */
    uint32_t height;        /* height of foldable screen */
    uint32_t fold_state;    /* state of foldable screen */
    uint32_t display_state; /* one state of folded state */
    uint32_t phy_width;     /* real width of the mobile */
    uint32_t phy_height;    /* real height of the mobile */
} TEEC_TUI_Parameter;

typedef struct {
    uint32_t started;     /* 0 means cancel this operation, others mean to perform this operation */
    uint32_t paramTypes;  /* use TEEC_PARAM_TYPES to construct this value */
    TEEC_Parameter params[TEEC_PARAM_NUM];
    TEEC_Session *session;
    bool cancel_flag;
} TEEC_Operation;

#endif
