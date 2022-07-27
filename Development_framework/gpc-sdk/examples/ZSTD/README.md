# **ZSTD demo**

简体中文 | [English](README_en.md)

## 介绍

1. [ZSTD](https://github.com/kunpengcompute/zstd)即ZSTD压缩库，是2016年开源的一款快速无损压缩算法，基于C语言开发，旨在提供zlib库对应级别的压缩解压速度和更高的压缩比。

## 编译依赖
1. 确保先编译 **ZSTD**;

[下载地址](https://github.com/kunpengcompute/zstd)
[编译参考](https://github.com/kunpengcompute/zstd)

2. 创建ZSTD include和lib的目录，将头文件和，so库拷贝到该目录下；
   ```shell
   # 创建目录
   mkdir -p /usr/local/ZSTD/include
   mkdir -p /usr/local/ZSTD/lib
   # 拷贝头文件 zstd-aarch64-1.4.4/lib
   cp -p zstd.h /usr/local/ZSTD/include
   # 拷贝so文件 zstd-aarch64-1.4.4/lib
   cp -p libzstd.so.1.4.4 /usr/local/ZSTD/lib
   # 创建软连接
   cd /usr/local/ZSTD/lib
   ln -s libzstd.so.1.4.4 libzstd.so.1
   ln -s libzstd.so.1.4.4 libzstd.so
   ```

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

