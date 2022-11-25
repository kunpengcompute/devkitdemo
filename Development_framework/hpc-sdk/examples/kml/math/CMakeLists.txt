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
project(kml_math_demo C)

# Add source files
set(SRC src/math_demo.c)

# Set MATH option
option(MATH "This is a default MATH library performance." performance)

# Set target file
set(TARGET_FILE math_demo)

# Set KML path
set(KML_PATH /usr/local/kml)

# Set compiler
set(CMAKE_C_COMPILER mpicc)

# Add header file include directories
include_directories(${KML_PATH}/include)

# Default build type is debug
set(CMAKE_BUILD_TYPE "Debug")

# Add link library
link_directories(${KML_PATH}/lib)

# Find link library
find_library(MATH_L m HINTS ${KML_PATH}/lib)

if(${MATH} STREQUAL "precision")
    message(STATUS "The MATH version used by demo is: precision.")
    find_library(KMATH km_l9 HINTS ${KML_PATH}/lib)
else()
    message(STATUS "The MATH version used by demo is: performance.")
    find_library(KMATH km HINTS ${KML_PATH}/lib)
endif()

# Set debug compilation option
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-g)
endif()

# Add openmp option
add_compile_options(-fopenmp)

# Generate execute file
add_executable(${TARGET_FILE} ${SRC})
target_link_libraries(${TARGET_FILE} ${MATH_L} ${KMATH})

