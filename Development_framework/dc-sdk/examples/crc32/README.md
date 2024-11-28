# **CRC32 Demo**

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
$ cd ./devkitdemo/Development_framework/dc-sdk/examples/crc32
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
$ ./crc32_demo
```
预期结果：crc = 0x4ee4ecc1


## 接口说明
```
uint32_t KsalCrc32c(uint32_t seed, uint8_t *data, uint64_t len);
```

参数释义
|    参数名  |       类型      |               描述             |    输入/输出     |
| :--------: | :-------------: | :----------------------------: | :------------: |
|     seed   |   32位整型数    |           CRC的种子。           |   输入    |
|    data    |     字符数组    |           CRC的字符数组。       |   输入   |
|    len     |  64位整型数     |              数组长度。         |   输入        |
|     crc    |   32位整型数    |            CRC校验和。          |   输出        |
