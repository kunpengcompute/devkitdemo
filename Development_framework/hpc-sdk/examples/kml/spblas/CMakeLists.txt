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
project(kml_spblas_demo C)

# Add source files
set(SRC src/spblas_demo.c)

# Set SPBLAS option
option(SPBLAS "This is a default SPBLAS library single." single)

# Set target file
set(TARGET_FILE spblas_demo)

# Set KML path
set(KML_PATH /usr/local/kml)

# Set compiler
set(CMAKE_C_COMPILER mpicc)

# Add header file include directories
include_directories(${KML_PATH}/include)

# Default build type is debug
set(CMAKE_BUILD_TYPE "Debug")

# Set conditional compilation
if(${SPBLAS} STREQUAL "multi")
    message(STATUS "The SPBLAS version used by demo is: multi.")
    # Add link library
    link_directories(${KML_PATH}/lib/kspblas/multi)
    # Find link library
    find_library(KSPBLAS kspblas HINTS ${KML_PATH}/lib/kspblas/multi)
else()
    message(STATUS "The SPBLAS version used by demo is: single.")
    # Add link library
    link_directories(${KML_PATH}/lib/kspblas/single)
    # Find link library
    find_library(KSPBLAS kspblas HINTS ${KML_PATH}/lib/kspblas/single)
endif()

# Set debug compilation option
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-g)
endif()

# Add openmp option
add_compile_options(-fopenmp)

# Generate execute file
add_executable(${TARGET_FILE} ${SRC})
target_link_libraries(${TARGET_FILE} ${KSPBLAS})

