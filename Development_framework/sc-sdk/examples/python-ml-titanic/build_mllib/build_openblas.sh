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

ROOTDIR=$(pwd)
BUILD_DIR=${ROOTDIR}/build

install_openblas(){
	rm -rf build
	mkdir build
	cd build
	cmake .. -DCMAKE_CROSSCOMPILING=ON -DTARGET=ARMV8 -DARCH=aarch64 -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_INSTALL_PREFIX=$BUILD_DIR
	make
	make install
}

rm -rf OpenBLAS-0.3.9
tar -xf OpenBLAS-0.3.9.tar.gz
cd OpenBLAS-0.3.9

install_openblas