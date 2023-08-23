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

export CC="gcc -mlittle-endian --sysroot=$SDKTARGETSYSROOT -fstack-protector-strong -O2 -pipe -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0 -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0/aarch64-hongmeng-musl -nostdinc -I$SDKTARGETSYSROOT/usr/include -I$SDKTARGETSYSROOT/usr/lib/gcc/aarch64-hongmeng-musl/7.3.0/include -I$SDKDIR/sysroots/aarch64-euler-elf_all_in_one/usr/include -L$SDKTARGETSYSROOT/usr/lib -L$SDKTARGETSYSROOT/lib -L${ROOTDIR}/build/lib -nostdlib -nostartfiles -lc -lhmulibs -lhmsrv_fs -lhmsrv_net -lhwsecurec -lgcc_s -lm -lstdc++ -Wl,--dynamic-linker=/lib/hmld.so.elf -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed"

export LDFLAGS=" --verbose -L$SDKTARGETSYSROOT/usr/lib -L$SDKTARGETSYSROOT/lib -L$INSTALL_DIR/usr/lib -L$SDKTARGETSYSROOT/usr/lib/aarch64-hongmeng-musl/7.3.0 \
        -lc -lhmulibs -lhmsrv_fs -lhmsrv_net -lhwsecurec -lgcc_s -lm -lstdc++ \
        -Wl,--dynamic-linker=/lib/hmld.so.elf  -Wl,-z,relro  -Wl,-z,now  -Wl,-z,noexecstack  \
        -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed"

install_scipy(){
	rm -rf build
	python3 setup.py build install --prefix=${ROOTDIR}/build
}

cd scipy-1.5.4
install_scipy

cd ${ROOTDIR}/build/lib/python3.6/site-packages/scipy-1.5.4-py3.6-linux-aarch64.egg/scipy/optimize
mv _lsap_module.cpython-36m-aarch64-linux-gnu.so _lsap_module.cpython-36m.so
cd ${ROOTDIR}/scipy-1.5.4
