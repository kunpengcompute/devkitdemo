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


cmake_minimum_required(VERSION 3.4.1)
project(ZSTD-demo)

# Add C++11 standard
add_compile_options(-std=c++11)

# Add source files
set(SRC src/command.cpp src/zstd_demo.cpp)

# Set target file
set(TARGET_FILE zstd_demo)

# Set zstd path
set(ZSTD_PATH /usr/local)

# Add header file include directories
include_directories(${ZSTD_PATH}/include)

# Set lib path
link_directories(${ZSTD_PATH}/lib)

# Default build type is debug
set(CMAKE_BUILD_TYPE "Debug")

# Set debug compilation option
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-g)
endif()

# Generate execute file
add_executable(${TARGET_FILE} ${SRC})

# link_libraries
target_link_libraries(${TARGET_FILE} zstd)
