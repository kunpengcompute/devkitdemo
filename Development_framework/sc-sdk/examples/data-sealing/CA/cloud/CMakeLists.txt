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
project(data-sealing-ca)

# Set variable
# kunpeng-sc-devel path set
set(iTrustee_SDK_PATH /usr/local/kunpeng-sc-devel)

# Add source files
set(
        SRCS ../data_sealing_ca.c
        ${iTrustee_SDK_PATH}/source/itrustee_sdk/src/CA/libteec_adaptor.c
)

# Add header file include directories
include_directories(
        /usr/include/itrustee_sdk/CA
        ${iTrustee_SDK_PATH}/source/libboundscheck/include
)

# Set options
set(CMAKE_TEEC_FLAGS "-fstack-protector-strong -fPIC")
set(CMAKE_TEEC_FLAGS "${CMAKE_TEEC_FLAGS} -z text -z now -z relro -z noexecstack -pie")
set(CMAKE_TEEC_LDFLAGS "-lboundscheck")

# Set output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Generate execute file
add_executable(data-sealing ${SRCS})
target_link_libraries(data-sealing dl pthread boundscheck)
set_target_properties(data-sealing PROPERTIES COMPILE_FLAGS ${CMAKE_TEEC_FLAGS})
set_target_properties(data-sealing PROPERTIES LINK_FLAGS ${CMAKE_TEEC_LDFLAGS})

# Clean output
add_custom_target(clean
        COMMAND rm -rf ${PROJECT_SOURCE_DIR}/data-sealing
        COMMAND @echo "clean up"
        )