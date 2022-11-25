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
project(kml_blas_demo C)

# Add source files
set(SRC src/blas_demo.c)

# Set BLAS option
option(BLAS "This is a default BLAS library nolocking." nolocking)

# Set target file
set(TARGET_FILE blas_demo)

# Set KML path
set(KML_PATH /usr/local/kml)

# Set compiler
set(CMAKE_C_COMPILER mpicc)

# Add header file include directories
include_directories(${KML_PATH}/include)

# Default build type is debug
set(CMAKE_BUILD_TYPE "Debug")

# Set conditional compilation
if(${BLAS} STREQUAL "omp")
    message(STATUS "The BLAS version used by demo is: omp.")
    # Find link library
    find_library(KBLAS kblas HINTS ${KML_PATH}/lib/kblas/omp)
    # Add link library
    link_directories(${KML_PATH}/lib/kblas/omp)   
elseif(${BLAS} STREQUAL "locking")
    message(STATUS "The BLAS version used by demo is: locking.")
    # Find link library
    find_library(KBLAS kblas HINTS ${KML_PATH}/lib/kblas/locking)
    # Add link library
    link_directories(${KML_PATH}/lib/kblas/locking)
elseif(${BLAS} STREQUAL "pthread")
    message(STATUS "The BLAS version used by demo is: pthread.")
    # Find link library
    find_library(KBLAS kblas HINTS ${KML_PATH}/lib/kblas/pthread)
    # Add link library
    link_directories(${KML_PATH}/lib/kblas/pthread)
else()
    message(STATUS "The BLAS version used by demo is: nolocking.")
    # Find link library
    find_library(KBLAS kblas HINTS ${KML_PATH}/lib/kblas/nolocking)
    # Add link library
    link_directories(${KML_PATH}/lib/kblas/nolocking)
endif()

# Set debug compilation option
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-g)
endif()

# Add openmp option
add_compile_options(-fopenmp)

# Generate execute file
add_executable(${TARGET_FILE} ${SRC})
target_link_libraries(${TARGET_FILE} ${KBLAS})

