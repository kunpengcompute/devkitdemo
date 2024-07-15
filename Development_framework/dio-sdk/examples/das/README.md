# **Das Demo**

简体中文 | [English](README_en.md)

## 使用依赖

用户需要满足以下最低约束:

- CPU: Kunpeng 920处理器
- 服务器: TaiShan 200服务器 (型号2280)
- 操作系统: OpenEuler 20.03 LTS SP1, OpenEuler 22.03 LTS SP1, Kylin V10 SP1
- cmake3.20.5, gcc7.3.0, g++7.3.0
- BoostKit_KSAL软件包

如没有安装BoostKit_KSAL软件包, 请依照以下步骤安装:

步骤1. 从鲲鹏社区获取[BoostKit_KSAL软件包](https://www.hikunpeng.com/zh/developer/boostkit/sds)。 (注: 进入该链接后请点击"应用加速软件包-存储加速算法库-软件包下载")

步骤2. 获取BoostKit-KSAL_x.x.x.zip, 放置于/home目录下。

步骤3. 在/home目录下解压BoostKit-KSAL_x.x.x.zip。

步骤4. 安装解压的RPM包。
```sh
$ rpm -ivh /home/libksal-release-*.oe1.aarch64.rpm
```

步骤5. 确认RPM安装情况。
```sh
$ rpm -qi libksal
```
显示如下类似信息证明安装成功:
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

## 使用教程
步骤1. 获取代码。 (假设当前在/home目录下)
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
```

步骤2. 切入到Demo根路径。
```sh
$ cd /home/devkitdemo/gpc-sdk/examples/das
```

步骤3. 编译Demo。
```sh
$ rm -rf build
$ mkdir -p build

$ cd build
$ cmake ..
$ make
$ cd ..
```

步骤4. 运行Demo。
```sh
$ build/das_demo
```
预期结果：
1、das模块初始化成功，后台有DAS线程
2、回显打印推荐的预取IO信息
3、das模块反初始化成功

步骤5. 清理Demo。
```sh
$ rm -rf build
```

## 接口说明
```
int32_t Rcache_CeateDasModule(DasModuleParam *createInstanceParam);
void Rcache_ExitDasModule(void);
int32_t Rcache_PutDasInfo(DasKvParam *params);
```

参数释义
|    参数名  |       类型      |               描述             |    输入/输出     |
| :--------: | :-------------: | :----------------------------: | :------------: |
|  createInstanceParam   |    用户注册的回调函数结构体指针    |           需要用户注册的回调函数。       |   输入    |
|    params         |     用户提交IO信息结构体指针    |           用户提交IO信息至智能预取算法接口。       |   输入   |
|    ret            |  整型数 |              成功返回RETURN_OK，其他失败返回对应错误码。        |   输出        |


数据结构
struct DasOPS
|    成员名           |       类型      |             描述             |
| :-----------------: | :-------------: | :--------------------------: |
|   SubmitDasPrefetch |      函数指针    |           预取IO的提交函数。       |
|   logFunc           |      函数指针    |            DAS内部日志打印函数。       |


struct DasModuleParam
|    成员名           |       类型      |             描述             |
| :-----------------: | :-------------: | :--------------------------: |
|   ops              |      结构体指针    |       用户注册的回调函数。   |
|   logger           |      指针         |       用户的日志模块对象。   |
|   cacheLineSize    |      整型数       |       缓存行大小。          |


struct DasKvParam
|    成员名           |       类型      |             描述             |
| :-----------------: | :-------------: | :--------------------------: |
|   offset             |     整型数      |       预取IO起始位置，字节的整数倍。  |
|   len                |     整型数      |       预取IO长度，字节的整数倍。       |
|   timeStamp          |     整型数      |       IO的时间戳，单位：纳秒。             |
|   lunID              |     整型数      |       IO的逻辑单元号。          |
