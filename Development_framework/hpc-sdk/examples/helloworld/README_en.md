# **HPC sample demo**

English | [简体中文](README.md)

## Introduction

1. Provide the Hello World project in the HPC scenario.
2. When a project is created using the Kunpeng Development Framework Plugin, the samples directory provides all kml demo projects for reference.

## Dependencies

1. Using the [HPC SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/HPC/) to install the HMPI library, compiler, and math library.

## Guidance


1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/hpc-sdk/examples/examples/helloworld/
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
   # Run the demo independently.
   ./hello_world
   # Run the demo using the mpirun command.
   mpirun -n rank numbers hello_world
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```