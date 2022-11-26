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

project(hello_world C)

# Add source files
set(SRC src/hello_world.c)

# Set target file
set(TARGET_FILE hello_world)

# Set Compiler
set(CMAKE_C_COMPILER mpicc)

# Default build type is debug
set(CMAKE_BUILD_TYPE "Debug")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-g)
endif()

# Add openmp option
add_compile_options(-fopenmp)

# Generate execute file
add_executable(${TARGET_FILE} ${SRC})