# **SM3 demo**

English | [简体中文](README.md)

## Introduction

The **SM3 demo** shows a code example of hash calculation using SM3 algorithm, and provides two code implementations: OpenSSL and KAEdriver.

## Dependencies

Ensure that **[Kunpeng KAEdriver 1.3.11](https://www.hikunpeng.com/document/detail/en/kunpengaccel/encryp-decryp/devg-kae/kunpengaccel_16_0011.html)** has been installed.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/kae_sm3/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the demo.

   ```shell
   ./sm3_demo file_path
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build/
   ```
