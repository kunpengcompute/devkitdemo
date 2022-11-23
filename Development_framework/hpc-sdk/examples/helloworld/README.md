# **HPC sample demo**

简体中文 | [English](README_en.md)

## 介绍

1. 提供HPC场景Hello world工程；
2. 通过鲲鹏开发框架创建该工程，samples目录下会包含所有kml demo工程，供开发者参考。

## 使用依赖

1. 通过[HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/)安装HMPI库，编译器，数学库。

## 使用教程


1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/examples/helloworld/
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
   ./hello_world
   # mpirun 执行
   mpirun -n rank数 hello_world
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```