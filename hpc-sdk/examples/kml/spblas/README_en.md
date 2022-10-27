# **KML_SPBLAS library functions demo**

English | [简体中文](README.md)

## Introduction

1.

The [KML_SPBLAS](https://www.hikunpeng.com/document/detail/en/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0067.html)
library is a basic algebraic operation library of a sparse matrix. The majority of the elements in a sparse matrix are
zero.

2. The **KML_SPBLAS library functions demo** shows a code example for KML_SPBLAS library functions, demonstrates the use process of sparse matrix and vector calculation function.

## Dependencies

1. Ensure that boostkit-kml-1.4.0 has been installed.
2. Run the **env | grep LD_LIBRARY_PATH** command to ensure that the environment variable loaded.
3. Ensure that compiler version >= **GCC 7.3.0** has been installed.

## Guidance

There are multiple versions of KML_SPBLAS:
- Single-thread version: /usr/local/kml/lib/kspblas/single/libkspblas.so
- Multi-thread version: /usr/local/kml/lib/kspblas/multi/libkspblas.so

**Single-thread version is used by demo.**
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/hpc-sdk/examples/kml/spblas/
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
   ./spblas_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```