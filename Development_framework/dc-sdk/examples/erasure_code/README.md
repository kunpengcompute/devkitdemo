# **Erasure Code Demo**

简体中文 | [English](README_en.md)

## 使用依赖

确保环境上已经安装数据压缩SDK,需要同时安装kunpeng-dc-1.0.0和kunpeng-dc-devel-1.0.0，下载地址[数据压缩SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/DC/)。


## 使用教程
步骤1. 获取代码。
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
```

步骤2. 切入到Demo根路径。
```sh
$ cd ./devkitdemo/Development_framework/dc-sdk/examples/erausre_code
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
$ ./erasure_code_demo
```
预期结果：decode succ!!

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

