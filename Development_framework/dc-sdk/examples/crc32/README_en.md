# **CRC32 Demo**

English | [简体中文](README.md)


## Dependency
The following minimum requirements should be guaranteed.

- CPU: Huawei Kunpeng 920 Processor
- Server: TaiShan 200 Server (model 2280)
- Operating System: OpenEuler 20.03 LTS SP1, OpenEuler 22.03 LTS SP1, Kylin V10 SP1
- cmake3.20.5, gcc7.3.0, g++7.3.0
- BoostKit_KSAL Software

BoostKit_KSAL Software should be installed, if not, please follow the steps below:  

Step 1. Download [BoostKit_KSAL Software Package](https://www.hikunpeng.com/en/developer/boostkit/sds) from Kunpeng Community. (Note: Please find "Application Acceleration Software Packages-KSAL-Download Software")  

Step 2. Download BoostKit-KSAL_x.x.x.zip to /home Directory.  

Step 3. Uncompress BoostKit-KSAL_x.x.x.zip at /home Directory.  

Step 4. Install RPM Package.
```sh
$ rpm -ivh /home/libksal-release-*.oe1.aarch64.rpm
```

Step 5. Confirm RPM Installation Status.
```sh
$ rpm -qi libksal
```
Install successfully if such information shows:
```
Name        : libksal
Version     : 1.3.0
Release     : 1
Architecture: aarch64
Install Date: Fri 26 Apr 2024 09:59:15 AM CST
Group       : Unspecified
Size        : 1758092
License     : GPL
Signature   : (none)
Source RPM  : libksal-1.3.0-1.src.rpm
Build Date  : Tue 23 Apr 2024 02:50:00 PM CST
Build Host  : buildhost
Summary     : Kunpeng Storage Acceleration Library
Description :
Kunpeng Storage Acceleration Library
Product Name:           Kunpeng BoostKit
Product Version:        24.0.0
Component Name:         BoostKit-KSAL
Component Version:      1.3.0
Component AppendInfo:   kunpeng
```

## Usage Guidance
Step 1. Download Codes (Assuming at /home directory).
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
```

Step 2. Change to Demo Directory.
```sh
$ cd /home/devkitdemo/gpc-sdk/examples/crc32
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
$ build/crc32_demo
```
Expected Result：crc = 0x4ee4ecc1

Step 5. Clean Demo.
```sh
$ rm -rf build
```

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
