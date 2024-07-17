# **DAS Demo**

English | [简体中文](README.md)

## Introduction

DAS is an intelligent prefetching algorithm developed by Kunpeng, which can analyze IO information and pre fetch data to the read cache in advance, improving 4K sequential stream read performance by 100%.

## Dependency
The following minimum requirements should be guaranteed.

- CPU: Huawei Kunpeng 920 Processor
- Server: TaiShan 200 Server (model 2280)
- Operating System: OpenEuler 20.03 LTS SP1, OpenEuler 22.03 LTS SP1, Kylin V10 SP1
- cmake3.20.5, gcc7.3.0, g++7.3.0
- kunpeng-dio-devel Software


## Usage Guidance
Step 1. Download Codes (Assuming at /home directory).
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
$ cd /home/devkitdemo
$ git checkout origin/devkitdemo-23.0.1 -b devkitdemo-23.0.1
```

Step 2. Change to Demo Directory.
```sh
$ cd /home/devkitdemo/Development_framework/dio-sdk/examples/das/
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
$ build/das_demo
```
Expected Result:  
1. DAS module is initialized successfully and there exists DAS thread at background.
2. Recommanded prefetching I/O information can be printed out.
3. DAS module is deinitialized successfully.

Step 5. Clean Demo.
```sh
$ rm -rf build
```

## Interface
```
int32_t Rcache_CeateDasModule(DasModuleParam *createInstanceParam);
void Rcache_ExitDasModule(void);
int32_t Rcache_PutDasInfo(DasKvParam *params);
```

Parameter 
| Parameter Name |                               Type                             |               Description                      |    Input/Output |
| :------------: | :------------------------------------------------------------: | :--------------------------------------------: | :-------------: |
| createInstanceParam |  Callback function structure pointer that user registers  |  Callback function that  user needs to register. |  Input  |
|    params      |  I/O inforamtion structure pointer that user submits           |  I/O information that user submits to DAS interface. |  Input  |
|     ret        |      Integer                                                   |  Return 0 for success; otherwise for failure.  |   Output        |


Data Structure  

struct DasOPS
|    Field Name       |       Type       |                        Description                    |
| :-----------------: | :--------------: | :---------------------------------------------------: |
|   SubmitDasPrefetch | Function pointer |           Callback function that user registers.      |
|    logFunc          | Function pointer |           User log module object.                     |


struct DasModuleParam
|    Field Name       |        Type       |                        Description                    |
| :-----------------: | :---------------: | :---------------------------------------------------: |
|      ops            | Structure pointer |           Callback function that user registers.      |
|    logger           |     Pointer       |           User log module object.                     |
|    cacheLineSize    |     Integer       |           Cache line size.                            |


struct DasKvParam
|    Field Name       |        Type       |                        Description                    |
| :-----------------: | :---------------: | :---------------------------------------------------: |
|      offset         |      Integer      |           Prefetching I/O position (multiple of bytes).      |
|       len           |      Integer      |           Prefetching I/O length (multiple of bytes).                     |
|     timeStamp       |      Integer      |           I/O time stamp (millisecond).                            |
|      lunID          |      Integer      |           I/O logical unit number.                    |
