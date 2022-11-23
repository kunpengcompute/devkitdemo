# **KML_MATH library functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KML_MATH](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0111.html)
   是C语言实现的基本数学函数库。
2. **KML_MATH library functions demo** 展示使用KML_MATH库函数的代码示例，演示三角函数的使用流程。

## 使用依赖

1. 通过[HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/)安装HMPI库，编译器，数学库。

## 使用教程

KML_MATH有多个版本：
- 高性能版本：/usr/local/kml/lib/libkm.so
- 高精度版本：/usr/local/kml/lib/libkm_l9.so

**Demo中提供CMake编译选项来链接到不同版本的MATH库**
1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/math/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   # 默认使用高性能版本MATH库
   cmake ..
   make
   # 使用高精度版本MATH库
   cmake -DMATH=precision ..
   make
   ```

4. 运行demo

   ```shell
   # 单独执行
   ./math_demo
   # mpirun 执行
   mpirun -n rank数 math_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```