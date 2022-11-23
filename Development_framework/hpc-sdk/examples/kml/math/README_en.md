# **KML_MATH library functions demo**

English | [简体中文](README.md)

## Introduction

1. [KML_MATH](https://www.hikunpeng.com/document/detail/en/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0111.html)
   is a basic math function library implemented by the C language.
2. The **KML_MATH library functions demo** shows a code example for KML_MATH library functions, demonstrates the use process of trigonometric functions.

## Dependencies

1. Using the [HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/) to install the HMPI library, compiler, and math library.

## Guidance

There are multiple versions of KML_MATH:
- High-performance version: /usr/local/kml/lib/libkm.so
- High-precision version: /usr/local/kml/lib/libkm_l9.so

**The CMake compilation option is provided in the demo to link to different versions of the MATH library.**
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/math/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   # The MATH library of the high-performance version is used by default.
   cmake ..
   make
   # Use the MATH library of the high-precition version.
   cmake -DMATH=precision ..
   make
   ```

4. Run the demo.

   ```shell
   # Run the demo independently.
   ./math_demo
   # Run the demo using the mpirun command.
   mpirun -n rank numbers math_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```