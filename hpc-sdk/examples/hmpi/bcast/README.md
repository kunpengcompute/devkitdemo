# **Hyper MPI functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. [Hyper MPI](https://www.hikunpeng.com/developer/hpc/hypermpi)
   是整个高性能计算解决方案的关键组件，它实现了并行计算的网络通讯功能，可以用来支持制造、气象、超算中心等应用场景，同时该通信库也可扩展应用于AI、大数据等通用领域。Message Passing
   Interface（MPI）是支持多编程语言编程的并行计算通讯应用接口，具有高性能、大规模性、可移植性、可扩展性等特点。
2. **KHyper MPI functions demo** 展示运行Hyper MPI 命令。

## 使用依赖

1. 确保已安装 **Hyper MPI 1.1.0**

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/hpc-sdk/examples/hmpi/bcast/
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
   mpirun -n 4 bcast_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```