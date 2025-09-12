# **kaezlib & gzip demo**

English | [简体中文](README.md)

## Introduction
1. [KAEZlib](https://www.hikunpeng.com/developer/boostkit/library/detail?subtab=KAEZlib) is the compression module of the Kunpeng Accelerator Engine (KAE). It uses the Kunpeng hardware acceleration module to implement the deflate algorithm and works with the lossless user-mode driver framework to provide an interface for high-performance compression in gzip or zlib format.
## KAEZlib demo dependency
1. Ensure **KAEdriver** has been installed;
2. Ensure **KAEZlib** has been installed;

[Download KAEdriver](https://gitee.com/kunpengcompute/KAE/tree/kae2/KAEKernelDriver)
[Download KAEZlib](https://gitee.com/kunpengcompute/KAE/tree/kae2/KAEZlib)
[Reference](https://gitee.com/kunpengcompute/KAE/blob/kae2/README.md)

2. Or install **kunpeng-gpc** and **kunpeng-gpc-devle** SDK

## Guidance
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/kaezlib/
   ```

3. Compile the KAEZlib demo.

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run KAEZlib demo

   ```shell
   ./kaezlib_demo -h // help
   ./kaezlib_demo -czf filename.tar.gz filename // compress
   ./kaezlib_demo -xzf filename.tar.gz // decompress
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```
