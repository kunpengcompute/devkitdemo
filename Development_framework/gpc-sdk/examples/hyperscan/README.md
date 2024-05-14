# **HyperScan demo**

简体中文 | [English](README_en.md)

## 介绍

1. [HyperScan](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/system-lib/cg-hyperscan/kunpengaccel_hyperscan_02_0001.html)
   是一款来自于Intel的高性能正则表达式匹配库。它是以PCRE为原型开发，并以BSD许可证开源。

## 编译依赖
1. 确保先编译 **HyperScan**;
[下载地址](https://codeload.github.com/kunpengcompute/hyperscan/zip/aarch64)
[编译参考](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/system-lib/cg-hyperscan/kunpengaccel_hyperscan_02_0001.html)
   cmake编译so编译选项：-DBUILD_SHARED_LIBS=ON

2. 或者安装**kunpeng-gpc**和**kunpeng-gpc-devel** SDK

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/hyperscan/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. 运行demo

   ```shell
    ./hyperscan_demo hs_data+base_t ../src/hyperscan_demo.cpp
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```

