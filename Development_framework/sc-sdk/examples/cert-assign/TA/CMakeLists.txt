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

set(CMAKE_VERBOSE_MAKEFILEON ON)

# Set variable
# set kunpeng-sc-devel path
set(iTrustee_SDK_PATH /usr/local/kunpeng-sc-devel)
set(iTrustee_SDK_HEADER_PATH /usr/include/itrustee_sdk)
set(SIGNTOOL_DIR ${iTrustee_SDK_PATH}/utils/signtools)
set(LIBC ${iTrustee_SDK_HEADER_PATH}/thirdparty/musl)
set(LIBSECURE ${iTrustee_SDK_PATH}/source/liboundscheck)

# Add source files
set(SRCS ${SRCS} ./cert_assign_ta.c)

# Add header file include directories
include_directories(
    ${LIBC}/libc
    ${LIBC}/libc/arch/aarch64
    ${LIBC}/libc/arch/aarch64/bits
    ${LIBC}/libc/arch/generic
    ${LIBSECURE}/include
    ${iTrustee_SDK_HEADER_PATH}/TA
    ${iTrustee_SDK_HEADER_PATH}/TA/huawei_ext
    ${PROJECT_SOURCE_DIR}/include
    /usr/local/openssl/include
)

link_directories(
    ${iTrustee_SDK_PATH}/examples/cert-assign/lib
)

# Set options
set(CMAKE_FLAGS "-DAPI_LEVEL=3 -O -W -Wall -Werror -fno-short-enums -fno-omit-frame-pointer -fstack-protector-strong")
set(CMAKE_FLAGS "${CMAKE_FLAGS} -Wextra -nostdinc -nodefaultlibs -march=armv8-a -Os -Wno-main -fPIC")
set(CMAKE_FLAGS "${CMAKE_FLAGS} -Wno-error=unused-parameter -Wno-error=unused-but-set-variable")
set(TA_CMAKE_LD_FLAGS "-s -z text -z now -z relro -z noexecstack -shared -lboundscheck")
set(CMAKE_LD_FLAGS "-s -z text -z now -z relro -z noexecstack -shared")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -T${iTrustee_SDK_PATH}/utils/tools/ta_link_64.ld")

# Set output dir
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Generate .so file
add_library(tmp_object OBJECT ${SRCS})
set_target_properties(tmp_object PROPERTIES COMPILE_FLAGS ${CMAKE_FLAGS})
add_library(combine SHARED $<TARGET_OBJECTS:tmp_object>)
set_target_properties(combine PROPERTIES LINK_FLAGS ${CMAKE_LD_FLAGS})
target_link_libraries(
    combine PUBLIC -nodefaultlibs libgcc.a libcrypto.a
)

add_custom_command(TARGET combine
    POST_BUILD
    COMMAND python3 -B ${SIGNTOOL_DIR}/signtool_v3.py ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} --privateCfg ${PROJECT_SOURCE_DIR}/config_cloud.ini
)

# Clean output
add_custom_target(clean
    COMMAND rm -rf ${PROJECT_SOURCE_DIR}/libcombine.so ${PROJECT_SOURCE_DIR}/*.sec
    COMMAND @echo "clean up"
)
