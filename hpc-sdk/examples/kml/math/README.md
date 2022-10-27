# **KML_MATH library functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. [KML_MATH](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/math-lib/devg-kml/kunpengaccel_kml_16_0111.html)
   是C语言实现的基本数学函数库。
2. **KML_MATH library functions demo** 展示使用KML_MATH库函数的代码示例，演示三角函数的使用流程。

## 使用依赖

1. 确保已安装 **boostkit-kml-1.4.0**
2. 确保环境变量正常加载，执行 **env | grep LD_LIBRARY_PATH**
3. 确保已安装编译器版本 >= **GCC 7.3.0**

## 使用教程

KML_MATH有多个版本：
- 高性能版本：/usr/local/kml/lib/libkm.so
- 高精度版本：/usr/local/kml/lib/libkm_l9.so

**Demo使用高性能版本**
1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/hpc-sdk/examples/kml/math/
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
   ./math_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```