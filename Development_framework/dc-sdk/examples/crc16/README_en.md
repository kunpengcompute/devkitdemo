# **CRC16 Demo**

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
$ cd ./devkitdemo/Development_framework/dc-sdk/examples/crc16
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
$ ./crc16_demo
```
Expected Result：crc = 0xec6a

## Interface
```
uint16_t KsalCrc16T10Dif(uint16_t seed, uint8_t *data, uint64_t len);
```

Parameter 
|    Parameter Name  |       Type      |               Description             |    Input/Output     |
| :--------: | :-------------: | :----------------------------: | :------------: |
|     seed   |   16-Bit Integer    |           CRC seed。           |   Input    |
|    data    |    Character Array    |        CRC character array.        |   Input   |
|    len     |  64-Bit Integer     |          Array length.         |   Input        |
|     crc    |   16-Bit Integer    |           CRC checksum.          |   Output        |
