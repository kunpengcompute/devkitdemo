# **Erasure Code Demo**

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
$ cd /home/devkitdemo/gpc-sdk/examples/erausre_code
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
$ build/erasure_code_demo
```
预期结果：decode succ!!

步骤5. 清理Demo。
```sh
$ rm -rf build
```

## 接口说明
```
int KsalEcEncode(uint8_t **data, uint8_t **parity, struct HecParam *param);
int KsalEcDecode(uint8_t **data, uint8_t **parity, struct HecParam *param);
```

参数释义
|    参数名  |       类型      |               描述             |    输入/输出     |
| :--------: | :-------------: | :----------------------------: | :------------: |
|     data   |     指针数组    |           数据块指针数组。       |   输入/输出    |
|    parity  |     指针数组    |           校验块指针数组。       |   输入/输出   |
|    param   |  编解码参数指针 |              编解码参数。        |   输入        |
|     ret    |      整型数     |  返回0表示成功, 其他则表示失败。 |   输出        |


数据结构
|    成员名           |       类型      |             描述             |
| :-----------------: | :-------------: | :--------------------------: |
|   dataNum           |      整型数     |           数据块个数。       |
|    parity           |      整型数    |            校验块个数。       |
|  sectorSize         |      整型数     | 扇区大小, 最小编解码粒度, 当前支持64Byte粒度和4096Byte粒度。        |
|  blockSize          |      整型数     | 数据块或校验块的大小, 须为sectorSize的整数倍。 |
|  targetColNum       |      整型数     | 损失块的个数, 解码接口使用。 |
|  targetColArray[6]  |      整型数     | 损失块列表, 各个元素取值范围为[0, dataNum + parityNum)。 |
|  version            |      整型数     | 算法版本号, 默认值为1。版本号为0: 支持扇区大小为64Byte的整数倍; 版本号为1: 支持扇区大小为4096Byte的整数倍。 |

