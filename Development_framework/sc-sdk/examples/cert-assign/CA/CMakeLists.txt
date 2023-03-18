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
project(cert-assign C)

# Set variable
# set kunpeng-sc-devel path
set(iTrustee_SDK_PATH /usr/local/kunpeng-sc-devel)

# Add source files
set(SRCS 
    cert_assign_ca.c
    ${iTrustee_SDK_PATH}/source/itrustee_sdk/src/CA/libteec_adaptor.c
)

# Add header file include directories
include_directories(
    /usr/include/itrustee_sdk/CA
    ${iTrustee_SDK_PATH}/source/libboundscheck/include
)

# Set options
set(CMAKE_FLAGS "-fstack-protector-strong -fPIC")
set(CMAKE_FLAGS "${CMAKE_FLAGS} -z text -z now -z relro -z noexecstack -pie")
set(CMAKE_LDFLAGS "-lboundscheck")

# Set output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Generate shared lib
add_executable(cert-assign ${SRCS})
target_link_libraries(cert-assign dl pthread crypto boundscheck)
set_target_properties(cert-assign PROPERTIES COMPILE_FLAGS ${CMAKE_FLAGS})
set_target_properties(cert-assign PROPERTIES LINK_FLAGS ${CMAKE_LDFLAGS})

#Clean output
add_custom_target(clean
    COMMAND rm -rf ${PROJECT_SOURCE_DIR}/cert-assign
    COMMAND @echo "clean up"
)
