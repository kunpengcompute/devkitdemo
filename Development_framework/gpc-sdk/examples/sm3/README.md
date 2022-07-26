# **SM3 demo**

简体中文 | [English](README_en.md)

## 介绍
**SM3 demo** 展示使用SM3算法进行哈希计算代码示例，提供OpenSSL、KAE两种代码实现

## 使用依赖

确保已安装 **[Kunpeng KAEdriver 1.3.11](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/encryp-decryp/devg-kae/kunpengaccel_16_0011.html)**

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/sm3/
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
   ./sm3_demo file_path
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build/
   ```