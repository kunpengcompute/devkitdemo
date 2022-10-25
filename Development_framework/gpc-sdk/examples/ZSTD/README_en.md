# **ZSTD demo**

English | [简体中文](README.md)

## Introduction

1. [ZSTD](https://github.com/kunpengcompute/zstd)
   is an open source fast and lossless compression algorithm developed in 2016 using the C language. It aims to provide a compression ratio and higher compression and decompression rates.

## Build Dependency
1. Ensure **ZSTD** has been compiled;

[Download](https://github.com/kunpengcompute/zstd)
[Reference](https://github.com/kunpengcompute/zstd)

2. Or install **kunpeng-gpc** and **kunpeng-gpc-devle** SDK

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
    ./zstd_demo file // select file
    args // 1: compress 2: decompress 3: multiple compress 4: multiple decompress 5: multiple dictionary compress
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```