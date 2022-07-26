# **HyperScan demo**

English | [简体中文](README.md)

## Introduction

1. [HyperScan](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/system-lib/cg-hyperscan/kunpengaccel_hyperscan_02_0001.html)
   is an Intel-developed high-performance regular expression matching library. It is developed using PCRE as the prototype and is open-sourced under the BSD license.

## Build Dependency
1. Ensure **HyperScan** has been compiled;

[Download](https://github.com/kunpengcompute/hyperscan)
[Reference](https://support.huaweicloud.com/cg-hyperscan-kunpengaccel/kunpengaccel_hyperscan_02_0001.html)

2. Create HyperScan include and lib dir，copy head file and so to dir；
   ```shell
   # create dir
   mkdir -p /usr/local/hyperscan/include
   mkdir -p /usr/local/hyperscan/lib
   # copy head file hyperscan-5.3.0.aarch64/src
   cp -p hs*.h /usr/local/hyperscan/include/
   # copy head file hyperscan-5.3.0.aarch64/build
   cp -p hs_version.h /usr/local/hyperscan/include/
   # copy so file hyperscan-5.3.0.aarch64/build/lib
   cp -p libhs_runtime.so.5.3.0 /usr/local/hyperscan/lib
   cp -p libhs.so.5.3.0 /usr/local/hyperscan/lib
   # create soft link
   cd /usr/local/hyperscan/lib
   ln -s libhs_runtime.so.5.3.0 libhs_runtime.so.5
   ln -s libhs_runtime.so.5 libhs_runtime.so
   ln -s libhs.so.5.3.0 libhs.so.5
   ln -s libhs.so.5 libhs.so
   ```

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/hyperscan/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the demo.

   ```shell
    ./hyperscan_demo hs_data+base_t ../src/hyperscan_demo.cpp
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```
