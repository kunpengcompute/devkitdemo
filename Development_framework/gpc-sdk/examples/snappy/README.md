# **Snappy demo**

简体中文 | [English](README_en.md)

## 介绍

1. [Snappy](https://github.com/kunpengcompute/snappy)是一款基于C++语言开发的压缩算法，旨在提供较高的压缩解压速率和相对合理的压缩比

## 编译依赖
1. 确保先编译 **Snappy**;

[下载地址](https://github.com/kunpengcompute/snappy)
[编译参考](https://github.com/kunpengcompute/snappy)

2. 创建Snappy include和lib的目录，将头文件和，so库拷贝到该目录下；
   ```shell
   # 创建目录
   mkdir -p /usr/local/snappy/include
   mkdir -p /usr/local/snappy/lib
   # 拷贝头文件 snappy-aarch64-1.1.7/
   cp -p snappy.h /usr/local/snappy/include
   # 拷贝so文件 snappy-aarch64-1.1.7/build
   cp -p libsnappy.so.1.1.7 /usr/local/snappy/lib
   # 创建软连接
   cd /usr/local/snappy/lib
   ln -s libsnappy.so.1.1.7 libsnappy.so.1
   ln -s libsnappy.so.1 libsnappy.so
   ```

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/snappy/
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
    ./snappy_demo args
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```

