# **ZSTD demo**

English | [简体中文](README.md)

## Introduction

1. [ZSTD](https://github.com/kunpengcompute/zstd)
   is an open source fast and lossless compression algorithm developed in 2016 using the C language. It aims to provide a compression ratio and higher compression and decompression rates.

## Build Dependency
1. Ensure **ZSTD** has been compiled;

[Download](https://github.com/kunpengcompute/zstd)
[Reference](https://github.com/kunpengcompute/zstd)

2. Create ZSTD include and lib dir，copy head file and so to dir；
   ```shell
   # Create dir
   mkdir -p /usr/local/ZSTD/include
   mkdir -p /usr/local/ZSTD/lib
   # Copy head file zstd-aarch64-1.4.4/lib
   cp -p zstd.h /usr/local/ZSTD/include
   # Copy so file zstd-aarch64-1.4.4/lib
   cp -p libzstd.so.1.4.4 /usr/local/ZSTD/lib
   # Create soft link
   cd /usr/local/ZSTD/lib
   ln -s libzstd.so.1.4.4 libzstd.so.1
   ln -s libzstd.so.1.4.4 libzstd.so
   ```

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/ZSTD/
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
    ./zstd_demo args
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```