# **AES demo**

简体中文 | [English](README_en.md)

## 介绍
**AES demo** 展示使用ARMV8 AES汇编加速指令进行AES对称加密的代码示例。

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/aes/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. 运行demo

   key为16进制格式，支持128-bit, 192-bit和256-bit

   ```shell
   ./aes encrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/aes.txt -o ../test_data/aes.txt.cipher
   ./aes decrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/aes.txt.cipher -o ../test_data/aes.txt.plaintext
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build/
   ```