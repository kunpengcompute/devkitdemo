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
 * Description: Reference of TEE time api definitions
 */

#ifndef __TEE_TIME_API_H
#define __TEE_TIME_API_H

#include "tee_defines.h"

/*
 * Get current TEE system rtc time
 *
 * @param time [OUT] current system rtc time
 * @return void
 */
void get_sys_rtc_time(TEE_Time *time);

/*
 * Get current TEE system time
 *
 * @param time [OUT] current system time
 * @return void
 */
void TEE_GetSystemTime(TEE_Time *time);

/*
 * Waits for the specified number of milliseconds
 *
 * @param timeout [IN] specified number of milliseconds
 *
 * @return  TEE_SUCCESS success
 * @return  TEE_ERROR_CANCEL the wait has been cancelled
 * @return  TEE_ERROR_OUT_OF_MEMORY not enough memory is available to complete the operation
 */
TEE_Result TEE_Wait(uint32_t timeout);

/*
 * Retrieves the persistent time of the Trusted Application
 *
 * @param time [IN] the persistent time of the Trusted Application
 *
 * @return  TEE_SUCCESS success
 * @return  TEE_ERROR_TIME_NOT_SET the persistent time has not been set
 * @return  TEE_ERROR_TIME_NEEDS_RESET the persistent time has been set but may have been
 * corrupted and MUST no longer be trusted
 * @return  TEE_ERROR_OVERFLOW the number of seconds in the TA Persistent Time overflows the range of a uint32_t
 * @return  TEE_ERROR_OUT_OF_MEMORY not enough memory is available to complete the operation
 */
TEE_Result TEE_GetTAPersistentTime(TEE_Time *time);

/*
 * Set the persistent time of the current Trusted Application
 *
 * @param time [IN] the persistent time of the Trusted Application
 *
 * @return  TEE_SUCCESS success
 * @return  TEE_ERROR_OUT_OF_MEMORY not enough memory is available to complete the operation
 * @return  TEE_ERROR_STORAGE_NO_SPACE insufficient storage space is available to complete the operation
 */
TEE_Result TEE_SetTAPersistentTime(TEE_Time *time);

/*
 * Get current REE system time
 *
 * @param time [OUT] current REE system time
 * @return void
 */
void TEE_GetREETime(TEE_Time *time);

/*
 * Get string of current REE system time
 *
 * @param time_str     [OUT]  current REE system time string
 * @param time_str_len [OUT]  the length of time string
 * @return void
 */
void TEE_GetREETimeStr(char *time_str, uint32_t time_str_len);
#endif
