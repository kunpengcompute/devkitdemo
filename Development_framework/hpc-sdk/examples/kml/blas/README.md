# **KML_BLAS library functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. BLAS（Basic Linear Algebra
   Subprograms）提供了一系列基本线性代数运算函数的标准接口，包括矢量线性组合、矩阵乘以矢量、矩阵乘以矩阵等功能。BLAS已被广泛的应用于工业界和科学计算，成为业界标准。[KML_BLAS](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0012.html)
   库提供BLAS函数的C语言接口。
2. **KML_BLAS library functions demo** 展示使用KML_BLAS库函数的代码示例，演示矩阵乘法、对称矩阵乘法、更新对称矩阵轶等函数的使用流程。

## 使用依赖

1. 通过[HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/)安装HMPI库，编译器，数学库。

## 使用教程

KML_BLAS有多个版本：
- 单线程不加锁版本：/usr/local/kml/lib/kblas/nolocking/libkblas.so
- 单线程加锁版本：/usr/local/kml/lib/kblas/locking/libkblas.so
- pthread实现多线程版本：/usr/local/kml/lib/kblas/pthread/libkblas.so
- OpenMP实现多线程版本：/usr/local/kml/lib/kblas/omp/libkblas.so

**Demo中提供CMake编译选项来链接到不同版本的BLAS库**

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/blas/
   ```

3. 编译demo

   ```shell
   mkdir build
   cd build
   # 默认使用nolocking版本BLAS库
   cmake ..
   make
   # 使用locking版本BLAS库
   cmake -DBLAS=locking ..
   make
   # 使用pthread版本BLAS库
   cmake -DBLAS=pthread ..
   make
   # 使用omp版本BLAS库
   cmake -DBLAS=omp ..
   make
   ```

4. 运行demo

   ```shell
   # 单独执行
   ./blas_demo
   # mpirun 执行
   mpirun -n rank数 blas_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```