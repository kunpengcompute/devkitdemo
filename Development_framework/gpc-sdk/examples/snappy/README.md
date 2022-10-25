# **Snappy demo**

简体中文 | [English](README_en.md)

## 介绍

1. [Snappy](https://github.com/kunpengcompute/snappy)是一款基于C++语言开发的压缩算法，旨在提供较高的压缩解压速率和相对合理的压缩比

## 编译依赖
1. 确保先编译 **Snappy**;
[下载地址](https://github.com/kunpengcompute/snappy)
[编译参考](https://github.com/kunpengcompute/snappy)
   cmake编译so编译选项：-DBUILD_SHARED_LIBS=ON

2. 或者安装**kunpeng-gpc**和**kunpeng-gpc-devel** SDK

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
    ./snappy_demo file   // 指定文件
    args                 // 1: 压缩 2：解压
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```

