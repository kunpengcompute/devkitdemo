# **KML_FFT library functions demo**

English | [简体中文](README.md)

## Introduction

1. [KML_FFT](https://www.hikunpeng.com/document/detail/en/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0122.html)
   is a math library of fast Fourier transform (FFT).
2. The **KML_FFT library functions demo** shows a code example for KML_FFT library functions, demonstrates the use process of C2C transforms, etc functions.

## Dependencies

1. Using the [HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/) to install the HMPI library, compiler, and math library.

## Guidance

There are multiple versions of KML_FFT:
- Single-precision version: /usr/local/kml/lib/libkfft.so
- Double-precision version: -L /usr/local/kml/lib/libkfftf.so

**The CMake compilation option is provided in the demo to link to different versions of the FFT library.**
1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/fft/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   # The FFT library of the double-precision is used by default.
   cmake ..
   make
   # Use the FFT library of the single-precision version.
   cmake -DFFT=single ..
   make
   ```

4. Run the demo.

   ```shell
   # Run the demo independently.
   ./fft_demo
   # Run the demo using the mpirun command.
   mpirun -n rank numbers fft_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```