# =========================================================================
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
# ==========================================================================

# Description: KML fft library demo
# Create: 2022-05-15

cmake_minimum_required(VERSION 3.12)
project(kml_fft_demo C)

# Add source files
set(SRC src/fft_demo.c)

# Set target file
set(TARGET_FILE fft_demo)

# Set KML path
set(KML_PATH /usr/local/kml)

# Add header file include directories
include_directories(${KML_PATH}/include)

# Add link library
link_directories(${KML_PATH}/lib)

# Generate execute file
add_executable(${TARGET_FILE} ${SRC})
target_link_libraries(${TARGET_FILE} kfft)

