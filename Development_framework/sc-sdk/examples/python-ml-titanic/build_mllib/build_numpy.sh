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
OUTPUT_THIRDLIB=$ROOTDIR/output_thirdlib

mkdir -p ${OUTPUT_THIRDLIB}
mkdir -p ${OUTPUT_PYTHON_DIR}/lib/python3.6/site-packages/
mkdir -p ${BUILD_DIR}/lib/python3.6/site-packages/

install_py(){
	python3 setup.py build --fcompiler=gfortran install --prefix=$BUILD_DIR
}

export PYTHONPATH=$PYTHONPATH:${LOCAL_PYTHON_DIR}
export PYTHONPATH=$BUILD_DIR/lib/python3.6/site-packages:$PYTHONPATH
export PYTHONHOME=${LOCAL_PYTHON_DIR}
export PATH=${LOCAL_PYTHON_DIR}/bin:$PATH
export NPY_DISABLE_SVML=1

cd numpy-1.19.5
rm -rf build

install_py
