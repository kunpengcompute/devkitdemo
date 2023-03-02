#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# This script is used to compile the demo sdk.
set -e

export SOURCE_PATH=$(dirname $0)
export ABS_SOURCE_PATH=$(cd ${SOURCE_PATH};pwd)
export ITRUSTEE_BUILD_PATH=${ABS_SOURCE_PATH}/../../..

#clean
if [ "$#" -eq 1 ] && [ "$1"x = "clean"x ]; then
    rm -f *.o *.so *.sec
    if [ -d "cmake_build" ]; then
        rm -rf cmake_build
        echo "rm -rf cmake_build"
    fi
    exit 0
fi

echo "Cmake compile TA begin"
if [ -d "cmake_build" ]; then
    rm -rf cmake_build
    echo "rm -rf cmake_build"
fi
mkdir -p cmake_build
echo "mkdir cmake_build"
cd cmake_build/

cmake -DCMAKE_TOOLCHAIN_FILE=${ITRUSTEE_BUILD_PATH}/build/cmake/aarch64_toolchain.cmake ..

make VERBOSE=1

cd ..
rm -rf cmake_build