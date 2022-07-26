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
project(hyperscan-demo)

# Add C++11 standard
add_compile_option(-std=c++11)

# Add source files
set(SRC src/hyperscan_demo.cpp src/CommonGrep.cpp)

# Set target file
set(TARGET_FILE hyperscan_demo)

# Set HYPERSCAN path
set(HYPERSCAN_PATH /usr/local/hyperscan)

# Add header file include directories
include_directories(${HYPERSCAN_PATH}/include)

# Set lib path
link_directories(${HYPERSCAN_PATH}/lib)

# Generate execute file
add_executable(${TARGET_FILE} ${SRC})

# link library
target_link_libraries(${TARGET_FILE} hs)
