# Copyright 2024 Huawei Technologies Co., Ltd
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
project(erasure_code_demo)

# Add include files
include_directories(
    ./include
)

# Add source files
aux_source_directory(src SRC)

# Set target file
set(TARGET_FILE erasure_code_demo)

# Set compile
set(CMAKE_C_COMPILER gcc)

# Default build type is debug
set(CMAKE_BUILD_TYPE "Debug")

# Set debug compilation option
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-g)
endif()

#Generate execute file
add_executable(${TARGET_FILE} ${SRC})
target_link_libraries(${TARGET_FILE}
    -lksal
)