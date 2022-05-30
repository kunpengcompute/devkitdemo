# **KML_LAPACK library functions demo**

English | [简体中文](README.md)

## Introduction

1. [KML_LAPACK](https://www.hikunpeng.com/document/detail/en/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0203.html)
   optimizes the Linear Algebra PACKage (LAPACK) computing efficiency based on the Kunpeng architecture by means of
   block division, algorithm combination, multi-thread, and BLAS interface optimization.
2. The **KML_LAPACK library functions demo** shows a code example for KML_LAPACK library functions, demonstrates the use process of matrix factorization, matrix inversion, etc functions.

## Dependencies

1. Ensure that boostkit-kml-1.4.0 has been installed.
2. Ensure that lapack_adapt.a has been generated from lapack3.9.1.
3. Run the **env | grep LD_LIBRARY_PATH** command to ensure that the environment variable loaded.
4. Ensure that compiler version >= **GCC 7.3.0** has been installed.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/hpc-sdk/examples/kml/lapack/
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
   ./lapack_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```