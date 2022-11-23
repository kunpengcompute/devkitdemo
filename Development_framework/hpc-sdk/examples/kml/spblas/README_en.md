# **KML_SPBLAS library functions demo**

English | [简体中文](README.md)

## Introduction

1. The [KML_SPBLAS](https://www.hikunpeng.com/document/detail/en/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0067.html)
library is a basic algebraic operation library of a sparse matrix. The majority of the elements in a sparse matrix are
zero.

2. The **KML_SPBLAS library functions demo** shows a code example for KML_SPBLAS library functions, demonstrates the use process of sparse matrix and vector calculation function.

## Dependencies

1. Using the [HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/) to install the HMPI library, compiler, and math library.

## Guidance

There are multiple versions of KML_SPBLAS:
- Single-thread version: /usr/local/kml/lib/kspblas/single/libkspblas.so
- Multi-thread version: /usr/local/kml/lib/kspblas/multi/libkspblas.so

**The CMake compilation option is provided in the demo to link to different versions of the SPBLAS library.**
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd /devkitdemo/Development_framework/hpc-sdk/examples/kml/spblas/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   # The SPBLAS library of the single-thread version is used by default.
   cmake ..
   make
   # Use the SPBLAS library of the multi-thread version.
   cmake -DSPBLAS=multi ..
   make
   ```

4. Run the demo.

   ```shell
   # Run the demo independently.
   ./spblas_demo
   # Run the demo using the mpirun command.
   mpirun -n rank numbers spblas_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```