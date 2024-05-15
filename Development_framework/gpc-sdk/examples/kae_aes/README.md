# **AES demo**

简体中文 | [English](README_en.md)

## 介绍
**AES demo** 展示使用AES算法进行对称加密的代码示例，提供openSSL、KAEdriver两种代码实现

## 使用依赖

确保已安装 **[Kunpeng KAEdriver 1.3.12](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/encryp-decryp/devg-kae/kunpengaccel_16_0011.html)**

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/kae_aes/
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
   ./aes encrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/aes.txt -o ../test_data/aes.txt.cipher -K
   ./aes decrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/aes.txt.cipher -o ../test_data/aes.txt.plaintext -K
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build/
   ```