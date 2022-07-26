# **kaezip & gzip demo**

English | [简体中文](README.md)

## Introduction
1. [KAEzip](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/compress/devg-kaezip/kunpengaccel_kaezip_0001.html) is the compression module of the Kunpeng Accelerator Engine (KAE). It uses the Kunpeng hardware acceleration module to implement the deflate algorithm and works with the lossless user-mode driver framework to provide an interface for high-performance compression in gzip or zlib format.
2. [Gzip](https://github.com/kunpengcompute/gzip)(GNU zip) is a widely-used compression software that was the first of its kind to be released. In the optimized version, data prefetch, loop unrolling, and CRC instruction replacement are used to improve the compression and decompression rates of gzip-1.10 on the Kunpeng platform, especially the compression and decompression of text files.
## KAEzip demo dependency
1. Ensure **KAEdriver** has been installed;
2. Ensure **KAEzip** has been installed;

[Download KAEdriver](https://github.com/kunpengcompute/KAEdriver)
[Download KAEzip](https://github.com/kunpengcompute/KAEdriver)
[Reference](https://github.com/kunpengcompute/KAEzip)

## Gzip dependency
1. Ensure **Gzip** has been installed;
   
[Download Gzip](https://github.com/kunpengcompute/gzip)
[Reference](https://github.com/kunpengcompute/gzip)

## Guidance
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/kaezip_gzip/
   ```

3. Compile the KAEzip demo.

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run KAEzip demo

   ```shell
    ./kaezip_demo -h
   ```

5. run gzip
   ```shell
    gzip -h
   ```

7. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```
