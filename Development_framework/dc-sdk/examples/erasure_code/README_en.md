# **Erasure Code Demo**

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
$ cd /home/devkitdemo/gpc-sdk/examples/erausre_code
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
$ build/erasure_code_demo
```
Expected Result：decode succ!!

Step 5. Clean Demo.
```sh
$ rm -rf build
```

## Interface
```
int KsalEcEncode(uint8_t **data, uint8_t **parity, struct HecParam *param);
int KsalEcDecode(uint8_t **data, uint8_t **parity, struct HecParam *param);
```

Parameter 
| Parameter Name |                               Type      |               Description                      |    Input/Output |
| :------------: | :-------------------------------------: | :--------------------------------------------: | :-------------: |
|     data       |  Pointer array                           |  Pointer array for data block.                |   Input/Output  |
|    parity      |  Pointer array                           |  Pointer array for parity block.               |   Input/Output  |
|    param       |  Parameter pointer for encoding/decoding |  Parameter for encoding/decoding.              |   Input        |
|     ret        |      Integer                             |  Return 0 for success; otherwise for failure. |   Output        |


Data Structure
|    Field Name       |       Type       |                                Description                                                              |
| :-----------------: | :--------------: | :-----------------------------------------------------------------------------------------------------: |
|   dataNum           |      Integer     |           The Number of data blocks.                                                                    |
|    parity           |      Integer     |            The Number of parity blocks.                                                                 |
|  sectorSize         |      Integer     | Sector size, minimum size for encoding/decoding. Currently support multiple of 64 Bytes and 4096 Bytes. |
|  blockSize          |      Integer     | Data or parity block size, must be multiple of sectorSize.                                              |
|  targetColNum       |      Integer     | The number of lost blocks for decoding interface.                                                       |
|  targetColArray[6]  |      Integer     | List of lost blocks. Range is [0, dataNum + parityNum).                                                 |
|  version            |      Integer     | Algorithm version number, default 1. Version number 0: Sector size should be multiple of 64 Bytes; Version number 1: Sector size should be multiple of 4096 Bytes. |

