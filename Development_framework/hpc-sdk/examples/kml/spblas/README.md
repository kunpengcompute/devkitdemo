# **KML_SPBLAS library functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KML_SPBLAS](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0067.html)
   库是稀疏矩阵（Sparse Matrix）的基础代数运算库，稀疏矩阵是指大部分矩阵元素为零的矩阵。
2. **KML_SPBLAS library functions demo** 展示使用KML_SPBLAS库函数的代码示例，演示系数矩阵与向量计算函数的使用流程。

## 使用依赖

1. 通过[HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/)安装HMPI库，编译器，数学库。

## 使用教程

KML_SPBLAS有多个版本：
- 单线程版本：/usr/local/kml/lib/kspblas/single/libkspblas.so
- 多线程版本：/usr/local/kml/lib/kspblas/multi/libkspblas.so

**Demo中提供CMake编译选项来链接到不同版本的SPBLAS库**
1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/spblas/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   # 默认使用单线程版本SPBLAS库
   cmake ..
   make
   # 使用多线程版本SPBLAS库
   cmake -DSPBLAS=multi ..
   make
   ```

4. 运行demo

   ```shell
   # 单独执行
   ./spblas_demo
   # mpirun 执行
   mpirun -n rank数 spblas_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```