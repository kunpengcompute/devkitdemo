# **Erasure Code Demo**

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
$ cd ./devkitdemo/Development_framework/dc-sdk/examples/erausre_code
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
$ ./erasure_code_demo
```
Expected Result：decode succ!!

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

