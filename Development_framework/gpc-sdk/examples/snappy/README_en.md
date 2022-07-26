# **Snappy demo**

English | [简体中文](README.md)

## Introduction

1. [Snappy](https://github.com/kunpengcompute/snappy)
   is a compression algorithm developed using the C++ language. It aims to provide high compression and decompression rates and a relatively reasonable compression ratio.

## Build Dependency
1. Ensure **Snappy** has been compiled;

[Download](https://github.com/kunpengcompute/snappy)
[Reference](https://github.com/kunpengcompute/snappy)

2. Create Snappy include and lib dir，copy head file and so to dir；
   ```shell
   # Create dir
   mkdir -p /usr/local/snappy/include
   mkdir -p /usr/local/snappy/lib
   # Copy head file snappy-aarch64-1.1.7/
   cp -p *.h* /usr/local/snappy/include
   # 拷贝so文件 snappy-aarch64-1.1.7/build
   cp -p libsnappy.so.1.1.7 /usr/local/snappy/lib
   # create soft link
   cd /usr/local/snappy/lib
   ln -s libsnappy.so.1.1.7 libsnappy.so.1
   ln -s libsnappy.so.1 libsnappy.so
   ```

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
    ./snappy_demo args
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```
