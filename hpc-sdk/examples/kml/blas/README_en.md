# **KML_BLAS library functions demo**

English | [简体中文](README.md)

## Introduction

1. Basic Linear Algebra Subprograms (BLAS) provides a series of standard interfaces for basic linear algebra operation
   functions, including vector linear combination, matrix multiplied by vector, and matrix multiplied by matrix. BLAS
   has been widely used in industry and scientific computing, and has become an industry standard.
   The [KML_BLAS](https://www.hikunpeng.com/document/detail/en/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0012.html)
   library provides the C language interface for BLAS functions.
2. The **KML_BLAS library functions demo** shows a code example for KML_BLAS library functions, demonstrates   the use process of matrix multiplication, symmetric matrix multiplication, update the rank of sysmmetric matrix, etc functions.

## Dependencies

1. Ensure that boostkit-kml-1.4.0 has been installed.
2. Run the **env | grep LD_LIBRARY_PATH** command to ensure that the environment variable loaded.
3. Ensure that compiler version >= **GCC 7.3.0** has been installed.

## Guidance

There are multiple versions of KML_BLAS:
- Single-thread version without locking: /usr/local/kml/lib/kblas/nolocking/libkblas.so
- Single-thread version with locking: /usr/local/kml/lib/kblas/locking/libkblas.so
- Multi-thread version using pthread: /usr/local/kml/lib/kblas/pthread/libkblas.so
- Multi-thread version using OpenMP: /usr/local/kml/lib/kblas/omp/libkblas.so

**Single-thread version without locking is used by demo.**
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/hpc-sdk/examples/kml/blas/
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
   ./blas_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```