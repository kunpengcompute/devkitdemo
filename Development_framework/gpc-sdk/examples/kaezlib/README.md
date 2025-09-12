# **KAEZlib&Gzip demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KAEZlib](https://www.hikunpeng.com/developer/boostkit/library/detail?subtab=KAEZlib)是鲲鹏加速引擎的压缩模块，使用鲲鹏硬加速模块实现deflate算法，结合无损用户态驱动框架，提供高性能Gzip/zlib格式压缩接口。

[KAEdriver下载地址](https://gitee.com/kunpengcompute/KAE/tree/kae2/KAEKernelDriver)
[KAEZlib下载地址](https://gitee.com/kunpengcompute/KAE/tree/kae2/KAEZlib)
[编译参考](https://gitee.com/kunpengcompute/KAE/blob/kae2/README.md)

2. Or install **kunpeng-gpc** and **kunpeng-gpc-devle** SDK

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/kaezlib/
   ```

3. 编译KAEZlib demo

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. 运行KAEZlib demo

   ```shell
    ./kaezlib_demo -h // 查看帮助
    ./kaezlib_demo -czf filename.tar.gz filename // 压缩
    ./kaezlib_demo -xzf filename.tar.gz // 解压
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```

