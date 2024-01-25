/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2023. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: static definitions of client API
 */

#ifndef _TEE_CLIENT_CONSTANTS_H_
#define _TEE_CLIENT_CONSTANTS_H_

enum TEEC_ReturnCode {
    TEEC_SUCCESS = 0x0,                      /* success */
    TEEC_ERROR_INVALID_CMD,                  /* invalid command */
    TEEC_ERROR_SERVICE_NOT_EXIST,            /* target service is not exist */
    TEEC_ERROR_SESSION_NOT_EXIST,            /* session between client and service is not exist */
    TEEC_ERROR_SESSION_MAXIMUM,              /* exceed max num of sessions */
    TEEC_ERROR_REGISTER_EXIST_SERVICE,       /* cannot register the service which already exist */
    TEEC_ERROR_TAGET_DEAD_FATAL,             /* system error occurs in TEE */
    TEEC_ERROR_READ_DATA,                    /* failed to read data in file */
    TEEC_ERROR_WRITE_DATA,                   /* failed to write data to file */
    TEEC_ERROR_TRUNCATE_OBJECT,              /* data is truncated */
    TEEC_ERROR_SEEK_DATA,                    /* failed to seek data in file */
    TEEC_ERROR_FSYNC_DATA,                   /* failed to sync data in file */
    TEEC_ERROR_RENAME_OBJECT,                /* failed to rename file */
    TEEC_ERROR_TRUSTED_APP_LOAD_ERROR,       /* failed to load Trusted Application */
    TEEC_ERROR_GENERIC = 0xFFFF0000,         /* generic error occurs */
    TEEC_ERROR_ACCESS_DENIED = 0xFFFF0001,   /* permission check failed, in initilize context or
                                                open session or invoke commnad */
    TEEC_ERROR_CANCEL = 0xFFFF0002,          /* operation is already canceled */
    TEEC_ERROR_ACCESS_CONFLICT = 0xFFFF0003, /* confilct occurs in concurrent access to data,
                                                error occurs in file operaions generally */
    TEEC_ERROR_EXCESS_DATA = 0xFFFF0004,     /* exceed max data to be handled by system */
    TEEC_ERROR_BAD_FORMAT = 0xFFFF0005,      /* data format is invalid, Trusted Application cannot
                                                handle it */
    TEEC_ERROR_BAD_PARAMETERS = 0xFFFF0006,  /* invalid parameters */
    TEEC_ERROR_BAD_STATE = 0xFFFF0007,       /* operation failed in current state, when try to access
                                                storage without initilize storage service */
    TEEC_ERROR_ITEM_NOT_FOUND = 0xFFFF0008,  /* cannot find target item */
    TEEC_ERROR_NOT_IMPLEMENTED = 0xFFFF0009, /* request operation is not implemented */
    TEEC_ERROR_NOT_SUPPORTED = 0xFFFF000A,   /* request operation is not supported */
    TEEC_ERROR_NO_DATA = 0xFFFF000B,         /* no data present for current operation */
    TEEC_ERROR_OUT_OF_MEMORY = 0xFFFF000C,   /* system resource if out of use */
    TEEC_ERROR_BUSY = 0xFFFF000D,            /* system is too busy to handle current operation */
    TEEC_ERROR_COMMUNICATION = 0xFFFF000E,   /* error occurs when client try to communicate
                                                with Trusted Application */
    TEEC_ERROR_SECURITY = 0xFFFF000F,        /* security error occurs */
    TEEC_ERROR_SHORT_BUFFER = 0xFFFF0010,    /* out buffer is not enough for current request */
    TEEC_ERROR_MAC_INVALID = 0xFFFF3071,     /* MAC value check failed */
    TEEC_ERROR_TARGET_DEAD = 0xFFFF3024,     /* Trusted Application is crashed */
    TEEC_FAIL              = 0xFFFF5002,     /* common error */
    TEEC_ERROR_EXTERNAL_CANCEL   = 0xFFFF0011,  /* used by adapt only, event caused User Interface operation aborted */
    TEEC_ERROR_OVERFLOW          = 0xFFFF300F,  /* used by adapt only */
    TEEC_ERROR_STORAGE_NO_SPACE  = 0xFFFF3041,  /* used by adapt only */
    TEEC_ERROR_SIGNATURE_INVALID = 0xFFFF3072,  /* used by adapt only */
    TEEC_ERROR_TIME_NOT_SET      = 0xFFFF5000,  /* used by adapt only */
    TEEC_ERROR_TIME_NEEDS_RESET  = 0xFFFF5001,  /* used by adapt only */
    TEEC_ERROR_IPC_OVERFLOW      = 0xFFFF9114   /* ipc overflow */
};

enum TEEC_ReturnCodeOrigin {
    TEEC_ORIGIN_API = 0x1,         /* error occurs in handling client API */
    TEEC_ORIGIN_COMMS = 0x2,       /* error occurs in communicating between REE and TEE */
    TEEC_ORIGIN_TEE = 0x3,         /* error occurs in TEE */
    TEEC_ORIGIN_TRUSTED_APP = 0x4, /* error occurs in Trusted Application */
};

enum TEEC_SharedMemCtl {
    TEEC_MEM_INPUT = 0x1,  /* input type of memroy */
    TEEC_MEM_OUTPUT = 0x2, /* output type of memory */
    TEEC_MEM_INOUT = 0x3,  /* memory is used as both input and output */
    TEEC_MEM_SHARED_INOUT = 0x4,  /* no copy shared memory */
};

enum TEEC_ParamType {
    TEEC_NONE = 0x0,  /* unused parameter */
    TEEC_VALUE_INPUT = 0x01,  /* input type of value, refer TEEC_Value */
    TEEC_VALUE_OUTPUT = 0x02, /* output type of value, refer TEEC_Value */
    TEEC_VALUE_INOUT = 0x03,  /* value is used as both input and output, refer TEEC_Value */
    TEEC_MEMREF_TEMP_INPUT = 0x05,  /* input type of temp memory reference, refer TEEC_TempMemoryReference */
    TEEC_MEMREF_TEMP_OUTPUT = 0x06, /* output type of temp memory reference, refer TEEC_TempMemoryReference */
    TEEC_MEMREF_TEMP_INOUT = 0x07,  /* temp memory reference used as both input and output,
                                       refer TEEC_TempMemoryReference */
    TEEC_ION_INPUT = 0x08,  /* input type of icon memory reference, refer TEEC_IonReference */
    TEEC_ION_SGLIST_INPUT = 0x09, /* input type of ion memory block reference, refer TEEC_IonSglistReference */
    TEEC_MEMREF_SHARED_INOUT = 0x0a, /* no copy mem */
    TEEC_MEMREF_WHOLE = 0xc, /* use whole memory block, refer TEEC_RegisteredMemoryReference */
    TEEC_MEMREF_PARTIAL_INPUT = 0xd, /* input type of memory reference, refer TEEC_RegisteredMemoryReference */
    TEEC_MEMREF_PARTIAL_OUTPUT = 0xe, /* output type of memory reference, refer TEEC_RegisteredMemoryReference */
    TEEC_MEMREF_PARTIAL_INOUT = 0xf /* memory reference used as both input and output,
                                        refer TEEC_RegisteredMemoryReference */
};

/****************************************************
 *      Session Login Methods
 ****************************************************/
enum TEEC_LoginMethod {
    TEEC_LOGIN_PUBLIC = 0x0,            /* no Login data is provided */
    TEEC_LOGIN_USER,                    /* Login data about the user running the
                                           Client Application process is provided */
    TEEC_LOGIN_GROUP,                   /* Login data about the group running
                                           the Client Application process is provided */
    TEEC_LOGIN_APPLICATION = 0x4,       /* Login data about the running Client
                                           Application itself is provided */
    TEEC_LOGIN_USER_APPLICATION = 0x5,  /* Login data about the user running the
                                           Client Application and about the
                                           Client Application itself is provided */
    TEEC_LOGIN_GROUP_APPLICATION = 0x6, /* Login data about the group running
                                           the Client Application and about the
                                           Client Application itself is provided */
    TEEC_LOGIN_IDENTIFY = 0x7,          /* Login data is provided by REE system */
};
enum TST_CMD_ID {
    TST_CMD_ID_01 = 1,
    TST_CMD_ID_02,
    TST_CMD_ID_03,
    TST_CMD_ID_04,
    TST_CMD_ID_05
};

#define TEEC_PARAM_NUM 4 /* teec param max number */
#endif
