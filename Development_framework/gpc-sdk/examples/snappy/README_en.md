# **Snappy demo**

English | [简体中文](README.md)

## Introduction

1. [Snappy](https://github.com/kunpengcompute/snappy)
   is a compression algorithm developed using the C++ language. It aims to provide high compression and decompression rates and a relatively reasonable compression ratio.

## Build Dependency
1. Ensure **Snappy** has been compiled;
[Download](https://github.com/kunpengcompute/snappy)
[Reference](https://github.com/kunpengcompute/snappy)
compile the source code dynamic library: -DBUILD_SHARED_LIBS=ON

2. Or install **kunpeng-gpc** and **kunpeng-gpc-devle** SDK

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/snappy/
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
    ./snappy_demo file    // select file
    args                  // 1: compress 2: decompress
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```
