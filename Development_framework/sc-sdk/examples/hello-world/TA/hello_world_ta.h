/*
 * Copyright 2022 Huawei Technologies Co., Ltd
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: hello_world_ta.h
 * Create: 2022-08-15
 */
#ifndef __HELLO_WORLD_TA_H
#define __HELLO_WORLD_TA_H

#include <stddef.h>
#include "tee_defines.h"

#define TA_VERSION                    "v1.0.0"
#define CLIENT_APPLICATION_NAME       "/vendor/bin/hello-world"

#define PARAMS_IDX0                0            // Subscript index 0 of the params parameter.
#define PARAMS_IDX1                1            // Subscript index 1 of the params parameter.
#define PARAMS_IDX2                2            // Subscript index 2 of the params parameter.
#define PARAMS_IDX3                3            // Subscript index 3 of the params parameter.
#define PARAMS_SIZE                4            // Size of the params array.

typedef enum {
    CMD_HELLO_WORLD = 1,
} HELLO_WORLD_CMD;

extern TEE_Result addcaller_ca_exec(const char *ca_name, const char *user_name);

#endif
