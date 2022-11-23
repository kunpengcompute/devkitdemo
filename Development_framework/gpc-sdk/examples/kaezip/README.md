# **KAEzip&Gzip demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KAEzip](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/compress/devg-kaezip/kunpengaccel_kaezip_0001.html)是鲲鹏加速引擎的压缩模块，使用鲲鹏硬加速模块实现deflate算法，结合无损用户态驱动框架，提供高性能Gzip/zlib格式压缩接口。

[KAEdriver下载地址](https://github.com/kunpengcompute/KAEdriver)
[KAEzip下载地址](https://github.com/kunpengcompute/KAEdriver)
[编译参考](https://github.com/kunpengcompute/KAEzip)

2. Or install **kunpeng-gpc** and **kunpeng-gpc-devle** SDK

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/kaezip/
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
    ./kaezip_demo -h // 查看帮助
    ./kaezip_demo -czf filename.tar.gz filename // 压缩
    ./kaezip_demo -xzf filename.tar.gz // 解压
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```

