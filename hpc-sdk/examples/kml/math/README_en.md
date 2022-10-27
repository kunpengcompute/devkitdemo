# **KML_MATH library functions demo**

English | [简体中文](README.md)

## Introduction

1. [KML_MATH](https://www.hikunpeng.com/document/detail/en/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0111.html)
   is a basic math function library implemented by the C language.
2. The **KML_MATH library functions demo** shows a code example for KML_MATH library functions, demonstrates the use process of trigonometric functions.

## Dependencies

1. Ensure that boostkit-kml-1.4.0 has been installed.
2. Run the **env | grep LD_LIBRARY_PATH** command to ensure that the environment variable loaded.
3. Ensure that compiler version >= **GCC 7.3.0** has been installed.

## Guidance

There are multiple versions of KML_MATH:
- High-performance version: /usr/local/kml/lib/libkm.so
- High-precision version: /usr/local/kml/lib/libkm_l9.so

**High-performance version is used by demo.**
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/hpc-sdk/examples/kml/math/
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
   ./math_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```