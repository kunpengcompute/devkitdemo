# **KAEzip&Gzip demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KAEzip](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/compress/devg-kaezip/kunpengaccel_kaezip_0001.html)是鲲鹏加速引擎的压缩模块，使用鲲鹏硬加速模块实现deflate算法，结合无损用户态驱动框架，提供高性能Gzip/zlib格式压缩接口。
2. [Gzip](https://github.com/kunpengcompute/gzip)（GNU zip）是一款发布较早并已广泛应用的压缩软件。其优化版本在官网发布的Gzip-1.10 Release版本基础上，通过数据预取、循环展开、CRC指令替换等方法，来提升其在鲲鹏计算平台上的压缩和解压缩速率，尤其对文本类型文件的压缩及解压具有更明显的性能优势。
## KAEzip编译依赖
1. 确保先安装 **KAEdriver**;
2. 确保先编译 **KAEzip**

[KAEdriver下载地址](https://github.com/kunpengcompute/KAEdriver)
[KAEzip下载地址](https://github.com/kunpengcompute/KAEdriver)
[编译参考](https://github.com/kunpengcompute/KAEzip)

## Gzip编译依赖
1. 确保先按照 **Gzip**;
[Gzip下载地址](https://github.com/kunpengcompute/gzip)
[编译参考](https://github.com/kunpengcompute/gzip)

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/kaezip_gzip/
   ```

3. 编译KAEzip demo

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. 运行KAEzip demo

   ```shell
    ./kaezip_demo -h
   ```

5. 运行gzip
   ```shell
    gzip -h
   ```

7. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```

