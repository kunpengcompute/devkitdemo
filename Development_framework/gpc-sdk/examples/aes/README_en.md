# **AES demo**

English | [简体中文](README.md)

## Introduction

The **AES demo** shows a code example of AES symmetric encryption using ARMV8 AES assembly accelerated instructions.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/aes/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the demo.

   Key is in hexadecimal format and supports 128-bit, 192-bit, and 256-bit.

   ```shell
   ./aes encrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/aes.txt -o ../test_data/aes.txt.cipher
   ./aes decrypt -k c5a99f63eceb7f32926bd6008ba06187 -m cbc -i ../test_data/aes.txt.cipher -o ../test_data/aes.txt.plaintext
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build/
   ```
