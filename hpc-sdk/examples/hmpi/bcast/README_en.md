# **Hyper MPI broadcast functions demo**

English | [简体中文](README.md)

## Introduction

1. [Hyper MPI](https://www.hikunpeng.com/en/developer/hpc/hypermpi) is key to the HPC solution. It implements network
   communication for parallel computing and is applicable to manufacturing, meteorology, supercomputing, AI, and big
   data. Message Passing Interface (MPI) supports multi-language programming, featuring high performance, large scale,
   portability, and scalability.
2. [Hyper MPI](https://www.hikunpeng.com/en/developer/hpc/hypermpi) based on Open MPI 4.1.1 and Open UCX 1.10.1, Hyper
   MPI supports the parallel computing API interface of the MPI-V3.1 standard, and optimizes the collection
   communication framework. In addition, Hyper MPI accelerates the network for data-intensive and high-performance
   computing, and enables a high-speed communication network and shared memory mechanism between nodes, and an optimized
   collection communication algorithm. The maximum data packet length supported by the COLL UCX communication framework
   of Hyper MPI is 232 bytes.
3. The **Hyper MPI broadcast functions demo** shows a code example of MPI_Bcast.

## Dependencies

1. Ensure that **Hyper MPI 1.1.1** has been installed.
2. Ensure that **GCC 9.3.0** or **BiSheng 2.1.0** has been installed.

## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/hpc-sdk/examples/hmpi/bcast/
   ```

3. Compile the demo.

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the demo.

   ```shell
   # Non-root user run demo.
   mpirun -n 4 bcast_demo
   # non-root user run demo with non-coll mode.
   mpirun -n 4 -mca coll ^ucx bcast_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```

## Command Reference Link

1. [Non-coll Mode](https://support.huaweicloud.com/intl/en-us/usermanual-kunpenghpcs/userg_huaweimpi_0014.html)
2. [Coll Mode](https://support.huaweicloud.com/intl/en-us/usermanual-kunpenghpcs/userg_huaweimpi_0015.html)
3. [Algorithm Selection for Set Operations](https://support.huaweicloud.com/intl/en-us/usermanual-kunpenghpcs/userg_huaweimpi_0016.html)
4. [Command Description and Example](https://support.huaweicloud.com/intl/en-us/usermanual-kunpenghpcs/userg_huaweimpi_0031.html)