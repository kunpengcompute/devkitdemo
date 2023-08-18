#!/bin/bash
# Description: preare toolchains and env for build ta.
# Copyright @ Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
# Licensed under the Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan
# PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#     http://license.coscl.org.cn/MulanPSL2
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
# KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
# See the Mulan PSL v2 for more details.
set -e

source common.sh

SDKDIR=$1
LOCAL_PYTHON_DIR=$2
ROOTDIR=$(pwd)
OUTPUT_PYTHON_DIR=$ROOTDIR/output_python
BUILD_DIR=${ROOTDIR}/build
SDKTARGETSYSROOT=$SDKDIR/sysroots/ccos

mkdir -p ${OUTPUT_PYTHON_DIR}/lib/python3.6/site-packages/
mkdir -p ${BUILD_DIR}/lib/python3.6/site-packages/

export PYTHONPATH=$PYTHONPATH:${LOCAL_PYTHON_DIR}:${LOCAL_PYTHON_DIR}/lib/python3.6/site-packages
export PYTHONPATH=$PYTHONPATH:$BUILD_DIR/lib/python3.6/site-packages
export PYTHONHOME=${LOCAL_PYTHON_DIR}
export PATH=${LOCAL_PYTHON_DIR}/bin:$PATH

if [ $SDKDIR == "clean" ]; then
	echo "clean begin..."
	rm -rf $ROOTDIR/cpython
	rm -rf $BUILD_DIR
	rm -rf $OUTPUT_PYTHON_DIR
	rm -rf $ROOTDIR/thirdlib/build
	rm -rf $ROOTDIR/thirdmodule/build
	rm -rf $ROOTDIR/thirdmodule/thirdlib
    exit
elif [[ ! $SDKDIR || ! $LOCAL_PYTHON_DIR ]]; then
	echo "usage: ./build.sh SDKDIR LOCAL_PYTHON_DIR "
	exit
fi

mkdir -p $BUILD_DIR/lib/python3.6/site-packages/test-easy-install-2815114.write-test

export CFLAGS=" -fstack-protector-strong -O2 -pipe --sysroot=$SDKTARGETSYSROOT -nostdinc -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0 -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0/aarch64-hongmeng-musl/ -I$SDKTARGETSYSROOT/usr/include -I$SDKTARGETSYSROOT/usr/lib/gcc/aarch64-hongmeng-musl/7.3.0/include -I$SDKDIR/sysroots/aarch64-euler-elf_all_in_one/usr/include/"

export CXXFLAGS=" -fstack-protector-strong -O2 -pipe --sysroot=$SDKTARGETSYSROOT -nostdinc++ -DHAVE_IOSTREAM -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0 -I$SDKTARGETSYSROOT/usr/include -I$SDKTARGETSYSROOT/usr/lib/gcc/aarch64-hongmeng-musl/7.3.0/include -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0/aarch64-hongmeng-musl/ "

install_py(){
	python3 setup.py clean
	python3 setup.py install --prefix=$BUILD_DIR
}

cd pandas-1.1.5

install_py
