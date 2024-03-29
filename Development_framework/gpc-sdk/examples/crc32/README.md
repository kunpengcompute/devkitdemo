# **crc32 demo**

简体中文 | [English](README_en.md)

## 介绍

循环冗余校验（Cyclic Redundancy Check， CRC）是一种根据网络数据包或计算机文件等数据产生简短固定位数校验码的一种信道编码技术，主要用来检测或校验数据传输或者保存后可能出现的错误。它是利用除法及余数的原理来作错误侦测的。

1. [Crc32 传统查表法] 

2. [Crc32 使用neon指令发] 提供两种api接口，函数格式为__crc32x和__crc32cx，不加c的，生成多项式为0x04C11DB7(CRC-32标准)，加c的，其生成多项式为0x1EDC6F41(CRC-32C标准)。

3. **Crc32 demo** 用于展示crc32不同实现方式的计算效率的比对

## 使用依赖

1.neon指令依赖gcc系统头文件“arm_acle.h”，在使用neon指令法进行crc二进制文件编译的时候，确保系统gcc版本高于4.8.5

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/gpc-sdk/examples/crc32/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. 运行demo.

   ```shell
   # 执行二进制文件, 如果不带路径参数，默认扫描../src/main.c文件。
   ./crc32_demo
   ```
   
   ```shell
   # 执行二进制文件, 带路径参数。
   ./crc32_demo /opt/main.c
   ```

5. 清理demo.

   ```shell
   cd ..
   rm -rf build
   ```