# **HyperScan demo**

English | [简体中文](README.md)

## Introduction

1. [HyperScan](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/system-lib/cg-hyperscan/kunpengaccel_hyperscan_02_0001.html)
   is an Intel-developed high-performance regular expression matching library. It is developed using PCRE as the prototype and is open-sourced under the BSD license.

## Build Dependency
1. Ensure **HyperScan** has been compiled;

[Download](https://github.com/kunpengcompute/hyperscan)
[Reference](https://support.huaweicloud.com/cg-hyperscan-kunpengaccel/kunpengaccel_hyperscan_02_0001.html)

2. Or install **kunpeng-gpc** and **kunpeng-gpc-devle** SDK

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
