# **SM4 demo**

English | [简体中文](README.md)

## Introduction

The **SM4 demo** shows a code example of symmetric encryption using SM4 algorithm, and provides two code implementations: OpenSSL and KAEdriver.

## Dependencies

Ensure that **[Kunpeng KAEdriver 1.3.11](https://www.hikunpeng.com/document/detail/en/kunpengaccel/encryp-decryp/devg-kae/kunpengaccel_16_0011.html)** has been installed.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/sm4/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the demo.

   Key is in hexadecimal format and supports 128-bit.

   ```shell
   ./sm4_demo encrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/sm4.txt -o ../test_data/sm4.txt.cipher
   ./sm4_demo decrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/sm4.txt.cipher -o ../test_data/sm4.txt.plaintext
   ./sm4_demo encrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/sm4.txt -o ../test_data/sm4.txt.cipher -K
   ./sm4_demo decrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/sm4.txt.cipher -o ../test_data/sm4.txt.plaintext -K
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build/
   ```
