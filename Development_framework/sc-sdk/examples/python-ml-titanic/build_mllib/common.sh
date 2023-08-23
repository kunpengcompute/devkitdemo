#!/bin/bash
set -eux
CUR_DIR=/opt/ccos_build
# SDK_DIR为机密OS SDK的安装路径
SDK_DIR=$CUR_DIR/ccos_sdk
# LOCAL_PYTHON_DIR为本地安装的python路径
LOCAL_PYTHON_DIR=$CUR_DIR/python_linux

INSTALL_DIR=$CUR_DIR/install
OUTPUT_PYTHON_DIR=$CUR_DIR/python_output
thirdlib_python_download=$CUR_DIR/thirdlib/download
thirdlib_python_build=$CUR_DIR/thirdlib/build

export PYTHONPATH=$LOCAL_PYTHON_DIR/lib/python3.6/site-packages/
export PYTHONHOME=$LOCAL_PYTHON_DIR
export PATH=$LOCAL_PYTHON_DIR/bin:$PATH
export SDKTARGETSYSROOT=$SDK_DIR/sysroots/ccos/
export PKG_CONFIG_SYSROOT_DIR=$SDKTARGETSYSROOT
export PKG_CONFIG_PATH=$SDKTARGETSYSROOT/usr/lib/pkgconfig:$SDKTARGETSYSROOT/usr/share/pkgconfig
export CC="gcc  -march=armv8-a -mlittle-endian  --sysroot=$SDKTARGETSYSROOT"
export CXX="g++  -march=armv8-a -mlittle-endian  --sysroot=$SDKTARGETSYSROOT"
export CPP="gcc -E  -march=armv8-a -mlittle-endian  --sysroot=$SDKTARGETSYSROOT"
export AS="as "
export LD="ld  --sysroot=$SDKTARGETSYSROOT"
export STRIP=strip
export RANLIB=ranlib
export AR=ar
export NM=nm
export CFLAGS="-fstack-protector-strong -O2 -pipe  --sysroot=$SDKTARGETSYSROOT -nostdinc \
        -I$SDKTARGETSYSROOT/usr/include -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0/include  \
        -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0/aarch64-hongmeng-musl/ -I$INSTALL_DIR/usr/include"
export CXXFLAGS="-fstack-protector-strong -O2 -pipe --sysroot=$SDKTARGETSYSROOT -nostdinc++ -DHAVE_IOSTREAM \
        -I$SDKTARGETSYSROOT/usr/include -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0 \
        -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0/aarch64-hongmeng-musl/ -I$INSTALL_DIR/usr/include"
export LDFLAGS=" --verbose -L$SDKTARGETSYSROOT/usr/lib -L$SDKTARGETSYSROOT/lib -L$INSTALL_DIR/usr/lib \
        -nostdlib -nostartfiles  -lc -lhmulibs -lhmsrv_fs -lhmsrv_net -lhwsecurec -lgcc_s -lm -lstdc++ \
        -Wl,--dynamic-linker=/lib/hmld.so.elf  -Wl,-z,relro  -Wl,-z,now  -Wl,-z,noexecstack  \
        -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed"
export CPPFLAGS="-fstack-protector-strong -O2 -pipe --sysroot=$SDKTARGETSYSROOT -nostdinc++  \
        -DHAVE_IOSTREAM -I$SDKTARGETSYSROOT/usr/include/c++/7.3.0 -I$SDKTARGETSYSROOT/usr/include"
export OPT="-D_GNU_SOURCE -fPIC -fwrapv"
export READELF="readelf"