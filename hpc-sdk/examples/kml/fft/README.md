# **KML_FFT library functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KML_FFT](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0122.html)
   是一个快速傅里叶变换(FFT)的数学库。
2. **KML_FFT library functions demo** 展示使用KML_FFT库函数的代码示例，演示C2C变换等函数的使用流程。

## 使用依赖

1. 确保已安装 **boostkit-kml-1.4.0**
2. 确保环境变量正常加载，执行 **env | grep LD_LIBRARY_PATH**
3. 确保已安装编译器版本 >= **GCC 7.3.0**

## 使用教程

KML_FFT有多个版本：
- 单精度版本：/usr/local/kml/lib/libkfft.so
- 双精度版本：/usr/local/kml/lib/libkfftf.so

**Demo中使用单精度版本**
1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/hpc-sdk/examples/kml/fft/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. 运行demo

   ```shell
   ./fft_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```