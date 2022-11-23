# **KML_LAPACK library functions demo**

English | [简体中文](README.md)

## Introduction

1. [KML_LAPACK](https://www.hikunpeng.com/document/detail/en/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0203.html)
   optimizes the Linear Algebra PACKage (LAPACK) computing efficiency based on the Kunpeng architecture by means of
   block division, algorithm combination, multi-thread, and BLAS interface optimization.
2. The **KML_LAPACK library functions demo** shows a code example for KML_LAPACK library functions, demonstrates the use process of matrix factorization, matrix inversion, etc functions.

## Dependencies

1. Using the [HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/) to install the HMPI library, compiler, and math library.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/lapack/
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
   # Run the demo independently.
   ./lapack_demo
   # Run the demo using the mpirun command.
   mpirun -n rank numbers lapack_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```