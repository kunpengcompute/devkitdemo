# **Hyper MPI broadcast functions demo**

简体中文 | [English](README_en.md)

## 介绍

1. [Hyper MPI](https://www.hikunpeng.com/developer/hpc/hypermpi)
   是整个高性能计算解决方案的关键组件，它实现了并行计算的网络通讯功能，可以用来支持制造、气象、超算中心等应用场景，同时该通信库也可扩展应用于AI、大数据等通用领域。Message Passing
   Interface（MPI）是支持多编程语言编程的并行计算通讯应用接口，具有高性能、大规模性、可移植性、可扩展性等特点。
2. [Hyper MPI](https://www.hikunpeng.com/developer/hpc/hypermpi) 是基于Open MPI 4.1.1和Open UCX
   1.10.1，支持MPI-V3.1标准的并行计算API接口，新增了优化的集合通信框架。同时，Hyper MPI对数据密集型和高性能计算提供了网络加速能力，使能了节点间高速通信网络和节点内共享内存机制，以及优化的集合通信算法。Hyper
   MPI的COLL UCX通信框架能够支持的最大数据包长度为2^32字节。
3. **Hyper MPI broadcast functions demo** 用于展示对MPI_Bcast集合通信的优化。

## 使用依赖

1. 确保已安装 **Hyper MPI 1.1.1**
2. 确保已安装 **HMPI** 对应版本的编译器：**GCC 9.3.0** 或 **BiSheng 2.1.0**

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
   # 使用普通用户执行
   mpirun -n 4 bcast_demo
   # 使用普通用户执行，并屏蔽掉coll ucx
   mpirun -n 4 -mca coll ^ucx bcast_demo
   ```

5. 清理demo

   ```shell
   cd ..
   rm -rf build
   ```

## 命令参考链接

1. [Non-coll模式](https://www.hikunpeng.com/document/detail/zh/kunpenghpcs/hypermpi/userg_huaweimpi_0014.html)
2. [Coll模式](https://www.hikunpeng.com/document/detail/zh/kunpenghpcs/hypermpi/userg_huaweimpi_0015.html)
3. [集合操作算法选择说明](https://www.hikunpeng.com/document/detail/zh/kunpenghpcs/hypermpi/userg_huaweimpi_0016.html)
4. [命令说明及示例](https://www.hikunpeng.com/document/detail/zh/kunpenghpcs/hypermpi/userg_huaweimpi_0031.html)
