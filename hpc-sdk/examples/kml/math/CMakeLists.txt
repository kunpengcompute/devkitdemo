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

# Set target file
set(TARGET_FILE math_demo)

# Set KML path
set(KML_PATH /usr/local/kml)

# Add header file include directories
include_directories(${KML_PATH}/include)

# Add link library
link_directories(${KML_PATH}/lib)

# Find link library
find_library(MATH m HINTS ${KML_PATH}/lib)
find_library(KMATH km HINTS ${KML_PATH}/lib)

# Add openmp option
add_compile_options(-fopenmp)

# Generate execute file
add_executable(${TARGET_FILE} ${SRC})
target_link_libraries(${TARGET_FILE} ${MATH} ${KMATH})

