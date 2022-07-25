# **CRC-32 demo**

English | [简体中文](README.md)

## Introduction

Cyclic Redundancy Check (CRC) is a channel coding technology that generates a short and fixed-length check value according to data such as network data packets or computer files. It detects or checks errors that may occur after data transmission or storage based on the principle of division and remainder.

1. [Use the traditional table lookup method to implement CRC-32.]

2. [Use the neon instruction method to implement CRC-32.] Two APIs are provided. The function formats are __crc32x and __crc32cx. The generator polynomial of the former is 0x04C11DB7 (using CRC-32 algorithm), and the generator polynomial of the latter is 0x1EDC6F41 (using CRC-32C algorithm).

3. CRC-32 demo displays the computing efficiency comparison of different CRC32 implementation modes.

## Dependency

1. The dependency of NEON instruction is the GCC system header file 'arm_acle.h'. When using the NEOn instruction method to compile the CRC binary file, ensure that the GCC version is later than 4.8.5.

## Usage Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/gpc-sdk/examples/crc32/
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
   # Execute the binary file. If the command does not contain a path parameter, the '../src/mian.c' file is scanned by default.
   ./crc32_demo
   ```

   ```shell
   # Execute the binary file with the path parameter.
   ./crc32_demo /opt/main.c
   ```

5. Clean up the demo.

   ```shell
   cd ..
   rm -rf build
   ```