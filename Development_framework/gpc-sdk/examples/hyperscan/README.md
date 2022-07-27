# **HyperScan demo**

简体中文 | [English](README_en.md)

## 介绍

1. [HyperScan](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/system-lib/cg-hyperscan/kunpengaccel_hyperscan_02_0001.html)
   是一款来自于Intel的高性能正则表达式匹配库。它是以PCRE为原型开发，并以BSD许可证开源。

## 编译依赖
1. 确保先编译 **HyperScan**;

[下载地址](https://github.com/kunpengcompute/hyperscan)
[编译参考](https://support.huaweicloud.com/cg-hyperscan-kunpengaccel/kunpengaccel_hyperscan_02_0001.html)

2. 创建HyperScan include和lib的目录，将头文件和，so库拷贝到该目录下；
   ```shell
   # 创建目录
   mkdir -p /usr/local/hyperscan/include
   mkdir -p /usr/local/hyperscan/lib
   # 拷贝头文件 hyperscan-5.3.0.aarch64/src
   cp -p hs*.h /usr/local/hyperscan/include/
   # 拷贝头文件 hyperscan-5.3.0.aarch64/build
   cp -p hs_version.h /usr/local/hyperscan/include/
   # 拷贝so文件 hyperscan-5.3.0.aarch64/build/lib
   cp -p libhs_runtime.so.5.3.0 /usr/local/hyperscan/lib
   cp -p libhs.so.5.3.0 /usr/local/hyperscan/lib
   # 创建软连接
   cd /usr/local/hyperscan/lib
   ln -s libhs_runtime.so.5.3.0 libhs_runtime.so.5
   ln -s libhs_runtime.so.5 libhs_runtime.so
   ln -s libhs.so.5.3.0 libhs.so.5
   ln -s libhs.so.5 libhs.so
   ```

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

