# **Das Demo**

简体中文 | [English](README_en.md)

## 介绍
DAS是鲲鹏自研的智能预取算法，该算法可以分析IO信息，提前预取数据至读缓存中，4K顺序流读性能提升100%。

## 使用依赖

用户需要满足以下最低约束:

- CPU: Kunpeng 920处理器
- 服务器: TaiShan 200服务器 (型号2280)
- 操作系统: OpenEuler 20.03 LTS SP1, OpenEuler 22.03 LTS SP1, Kylin V10 SP1
- cmake3.20.5, gcc7.3.0, g++7.3.0
- kunpeng-dio-devel包

## 使用教程
步骤1. 获取代码。 (假设当前在/home目录下)
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
$ cd /home/devkitdemo
$ git checkout origin/devkitdemo-23.0.1 -b devkitdemo-23.0.1
```

步骤2. 切入到Demo根路径。
```sh
$ cd /home/devkitdemo/Development_framework/dio-sdk/examples/das/

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
