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

1. Using the [HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/) to install the HMPI library, compiler, and math library.

## Guidance

There are multiple versions of KML_BLAS:
- Single-thread version without locking: /usr/local/kml/lib/kblas/nolocking/libkblas.so
- Single-thread version with locking: /usr/local/kml/lib/kblas/locking/libkblas.so
- Multi-thread version using pthread: /usr/local/kml/lib/kblas/pthread/libkblas.so
- Multi-thread version using OpenMP: /usr/local/kml/lib/kblas/omp/libkblas.so

**The CMake compilation option is provided in the demo to link to different versions of the BLAS library.**
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/blas/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   # The BLAS library of the nolocking version is used by default.
   cmake ..
   make
   # Use the BLAS library of the locking version.
   cmake -DBLAS=locking ..
   make
   # Use the BLAS library of the pthread version.
   cmake -DBLAS=pthread ..
   make
   # Use the BLAS library of the omp version.
   cmake -DBLAS=omp ..
   make
   ```

4. Run the demo.

   ```shell
   # Run the demo independently.
   ./blas_demo
   # Run the demo using the mpirun command.
   mpirun -n rank numbers blas_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```