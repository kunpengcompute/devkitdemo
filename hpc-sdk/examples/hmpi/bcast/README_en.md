# **Hyper MPI functions demo**

English | [简体中文](README.md)

## Introduction

1. The [Hyper MPI](https://www.hikunpeng.com/en/developer/hpc/hypermpi) is key to the HPC solution. It implements
   network communication for parallel computing and is applicable to manufacturing, meteorology, supercomputing, AI, and
   big data. Message Passing Interface (MPI) supports multi-language programming, featuring high performance, large
   scale, portability, and scalability.
2. The **Hyper MPI functions demo** shows a code example for excute Hyper MPI command.

## Dependencies

1. Ensure that Hyper MPI 1.1.0 has been installed.

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
   mpirun -n 4 bcast_demo
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```