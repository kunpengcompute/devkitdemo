# **ZSTD demo**

简体中文 | [English](README_en.md)

## 介绍

1. [ZSTD](https://github.com/kunpengcompute/zstd)即ZSTD压缩库，是2016年开源的一款快速无损压缩算法，基于C语言开发，旨在提供zlib库对应级别的压缩解压速度和更高的压缩比。

## 编译依赖
1. 确保先编译 **ZSTD**;

[下载地址](https://github.com/kunpengcompute/zstd)
[编译参考](https://github.com/kunpengcompute/zstd)

2. 或者安装**kunpeng-gpc**和**kunpeng-gpc-devel** SDK

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/ZSTD/
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
    ./zstd_demo args
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```

