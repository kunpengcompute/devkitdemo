# **KML_FFT library functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KML_FFT](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0122.html)
   是一个快速傅里叶变换(FFT)的数学库。
2. **KML_FFT library functions demo** 展示使用KML_FFT库函数的代码示例，演示C2C变换等函数的使用流程。

## 使用依赖

1. 通过[HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/)安装HMPI库，编译器，数学库。

## 使用教程

KML_FFT有多个版本：
- 单精度版本：/usr/local/kml/lib/libkfftf.so
- 双精度版本：/usr/local/kml/lib/libkfft.so

**Demo中提供CMake编译选项来链接到不同版本的FFT库**
1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/fft/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   # 默认使用双精度版本FFT库
   cmake ..
   make
   # 使用单精度版本FFT
   cmake -DFFT=single ..
   make
   ```

4. 运行demo

   ```shell
   # 单独执行
   ./fft_demo
   # mpirun 执行
   mpirun -n rank数 fft_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```