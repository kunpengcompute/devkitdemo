# Copyright 2022 Huawei Technologies Co., Ltd
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


cmake_minimum_required(VERSION 2.8)
project(crc32_demo C)

# Add source files
set(SRC src/crc32_demo.c)
set(SRCC src/crc32_neon_demo.c)

#Set target file
set(TARGET_FILE crc32_demo)
set(TARGET_NEON_FILE crc32_neon_demo)

#Set compile
set(CMAKE_C_COMPILER gcc)

#Generate execute file
add_executable(${TARGET_FILE} ${SRC})
set(CMAKE_C_FLAGS "-march=armv8-a+crc")
add_executable(${TARGET_NEON_FILE} ${SRCC})
