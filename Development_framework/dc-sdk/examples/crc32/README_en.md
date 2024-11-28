# **CRC32 Demo**

English | [简体中文](README.md)

## Dependency

Make sure that the data compression SDK is installed on the environment, and both kunpeng-dc-1.0.0 and kunpeng-dc-devel-1.0.0 need to be installed simultaneously. Download the [Data compression SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/DC/).

## Usage Guidance
Step 1. Download Codes.
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
```

Step 2. Change to Demo Directory.
```sh
$ cd ./devkitdemo/Development_framework/dc-sdk/examples/crc32
```

Step 3. Compile Demo.
```sh
$ rm -rf build
$ mkdir -p build

$ cd build
$ cmake ..
$ make
$ cd ..
```

Step 4. Run Demo.
```sh
$ ./crc32_demo
```
Expected Result：crc = 0x4ee4ecc1

## Interface
```
uint32_t KsalCrc32c(uint32_t seed, uint8_t *data, uint64_t len);
```

Parameter 
|    Parameter Name  |       Type      |               Description             |    Input/Output     |
| :--------: | :-------------: | :----------------------------: | :------------: |
|     seed   |   32-Bit Integer    |           CRC seed。           |   Input    |
|    data    |    Character Array    |        CRC character array.        |   Input   |
|    len     |  64-Bit Integer     |          Array length.         |   Input        |
|     crc    |   32-Bit Integer    |           CRC checksum.          |   Output        |
