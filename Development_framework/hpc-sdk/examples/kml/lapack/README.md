# **KML_LAPACK library functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KML_LAPACK](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0203.html)
   通过分块、求解算法组合、多线程、BLAS接口优化等手段，基于鲲鹏架构对线性代数运算库（Linear Algebra PACKage）的计算效率进行了优化。
2. **KML_LAPACK library functions demo** 展示使用KML_LAPACK库函数的代码示例，演示矩阵分解、矩阵求逆等函数的使用流程。

## 使用依赖

1. 通过[HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/)安装HMPI库，编译器，数学库。

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/kml/lapack/
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
   # 单独执行
   ./lapack_demo
   # mpirun 执行
   mpirun -n rank数 lapack_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```