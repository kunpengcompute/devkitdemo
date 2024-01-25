/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: Header file of memory management api
 */

#ifndef TEE_MEM_MGMT_API_H
#define TEE_MEM_MGMT_API_H

#include "tee_defines.h"

/*
 * below definitions are defined by Global Platform or Platform SDK released previously
 * for compatibility:
 * don't make any change to the content below
 */
#ifndef ZERO_SIZE_PTR
#define ZERO_SIZE_PTR       ((void *)16)
#define zero_or_null_ptr(x) ((unsigned long)(x) <= (unsigned long)ZERO_SIZE_PTR)
#endif

enum MALLOC_HINT {
    ZERO           = 0,
    NOT_ZERO       = 1,
    ALIGN_004      = 0x80000002, /* buf align */
    ALIGN_008      = 0x80000003,
    ALIGN_016      = 0x80000004,
    ALIGN_032      = 0x80000005,
    ALIGN_064      = 0x80000006,
    ALIGN_128      = 0x80000007,
    ALIGN_256      = 0x80000008,
    ALIGN_004_ZERO = 0x80000012, /* buf align and set to zero */
    ALIGN_008_ZERO = 0x80000013,
    ALIGN_016_ZERO = 0x80000014,
    ALIGN_032_ZERO = 0x80000015,
    ALIGN_064_ZERO = 0x80000016,
    ALIGN_128_ZERO = 0x80000017,
    ALIGN_256_ZERO = 0x80000018,
};

#define TEE_MALLOC_FILL_ZERO 0x00000000
#define TEE_MALLOC_NO_FILL   0x00000001
#define TEE_MALLOC_NO_SHARE  0x00000002

#define TEE_MEMORY_ACCESS_READ      0x00000001
#define TEE_MEMORY_ACCESS_WRITE     0x00000002
#define TEE_MEMORY_ACCESS_ANY_OWNER 0x00000004

/*
 * fill the first size bytes of buffer with x.
 *
 * @param buffer [OUT] the pointer of buffer
 * @param x [IN] fill value
 * @param size [IN] number of bytes
 *
 * @return void
 */
#if defined(API_LEVEL) && defined(API_LEVEL1_2) && (API_LEVEL >= API_LEVEL1_2)
void TEE_MemFill(void *buffer, uint8_t x, size_t size);
#else
void TEE_MemFill(void *buffer, uint32_t x, size_t size);
#endif

/*
 * copy size bytes from src to dest
 *
 * @param dest [OUT] dest buffer pointer
 * @param src [IN] src buffer pointer
 * @param size [IN] number of bytes
 *
 * @return void
 */
void TEE_MemMove(void *dest, const void *src, size_t size);

/*
 * alloc memory of size bytes with hint value
 * The pointer returned will be compatible to any C basic data type
 *
 * @param size [IN] size of memory that will be allocated
 * @param hint [IN] a flag, 0 mean that the memory returned will filled with '\0'
 *
 * @return a pointer to the new allocated memory
 * @return NULL means failed when allocated
 */
void *TEE_Malloc(size_t size, uint32_t hint);

/*
 * release the memory which allocated by TEE_Malloc
 * If buffer equals to NULL,TEE_Free will do nothing
 * Caller should make sure that the buffer is created by TEE_Malloc or TEE_Realloc
 * and should NOT free one memory twice, operation result is unpredictable
 *
 * @param buffer [IN] pointer to memory
 *
 * @return none
  */
void TEE_Free(void *buffer);

/*
 * If new size larger than old size, the contents of old memory will not changed,
 * the remained memory are random bytes
 * There will be a new allocate action when modify the size of memory
 * If allocated failed, old memory will be returned and this function will return NULL
 * If buffer equals to NULL, this function is same to TEE_Malloc
 *
 * @param buffer [IN] pointer to memory
 * @param new_size [IN] new size
 *
 * @return pointer to new memory, should NOT be NULL
 * @return NULL means failed
 */
void *TEE_Realloc(void *buffer, size_t new_size);

/*
 * compare of memory contents
 *
 * @param buffer1 [IN] first pointer
 * @param buffer2 [IN] second pointer
 * @param size [IN] bytes size to be compared
 *
 * @return -1 buffer1 < buffer2
 * @return 0 buffer1 == buffer2
 * @return 1 buffer1 > buffer2
 */
int32_t TEE_MemCompare(const void *buffer1, const void *buffer2, size_t size);

/*
 * check the access rights of buffer
 *
 * @param accessFlags [IN] access rights to be checked
 * @param buffer [IN] pointer to memory
 * @param size [IN] size of memory to be checked
 *
 * @return TEE_SUCCESS have access rights
 * @return TEE_ERROR_ACCESS_DENIED do NOT have access rights
 */
TEE_Result TEE_CheckMemoryAccessRights(uint32_t accessFlags, const void *buffer, size_t size);

/*
 * A global variable used to share in different Session of same Instance
 *
 * @param instanceData [IN] Address of the global variable
 *
 * @return void
 */
void TEE_SetInstanceData(void *instanceData);

/*
 * get the pointer which set by TEE_SetInstanceData
 *
 * @return a pointer to the variable which set by TEE_SetInstanceData, the pointer should NOT be NULL
 * @return NULL no InstanceData is set
 */
void *TEE_GetInstanceData(void);

#endif
