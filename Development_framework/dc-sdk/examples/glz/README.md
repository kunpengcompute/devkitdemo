# **GLZ Demo**

简体中文 | [English](README_en.md)

## 使用依赖

用户需要满足以下最低约束:

- CPU: Kunpeng 920处理器
- 服务器: TaiShan 200服务器 (型号2280)
- 操作系统: OpenEuler 20.03 LTS SP1, OpenEuler 22.03 LTS SP1, Kylin V10 SP1
- cmake3.20.5, gcc7.3.0, g++7.3.0
- BoostKit_glz软件包

如没有安装BoostKit_glz软件包, 请依照以下步骤安装:

步骤1. 从鲲鹏社区获取[BoostKit_glz指导文档](https://www.hikunpeng.com/zh/developer/boostkit/sds)。 (注: 进入该链接后请点击"基础加速软件包-压缩算法-查看文档")  

步骤2. 根据[压缩算法 特性指南-编译压缩算法]指导，完成[下载压缩算法安装包、获取编译依赖、编译动态库]三部分操作即可  


## 使用教程
步骤1. 获取代码。 (假设当前在/home目录下)
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
```

步骤2. 切入到Demo根路径。
```sh
$ cd /home/devkitdemo/gpc-sdk/examples/glz
```

步骤3. 编译Demo。
```sh
$ rm -rf build
$ mkdir -p build

$ cd build
$ cmake ..
$ make
```

步骤4. 运行Demo。
```sh
$ ./glz_demo
```
预期结果:
```
TEST glz level 0 compress success!
origin size=10192446,after decompress=5781020,compress ratio=1.76
TEST glz level 0 decompress success!
origin size=5781020,after decompress=10192446
TEST glz level 1 compress success!
origin size=10192446,after decompress=3587468,compress ratio=2.84
TEST glz level 1 decompress success!
origin size=3587468,after decompress=10192446
```


步骤5. 清理Demo。
```sh
$ rm -rf build
```

## 接口说明
### 压缩接口：
```
int glz_compress_default(const glz_encoder * const glz_encode_state,
                         char *dst_buffer,
                         unsigned dst_size,
                         const char * const src_buffer,
                         unsigned src_size,
                         const encode_model encodeInf,
                         int level);;
```

参数释义
|    参数名  |               描述             |    输入/输出     |
| :--------: | :----------------------------: | :------------: |
|     glz_encode_state   |           压缩算法上下文           |   输入    |
|    src_buffer    |           输入buffer，用于存放待压缩字符串       |   输入   |
|    src_size     |             输入buffer长度         |   输入        |
|     encodeInf    |            压缩模式，支持高性能与高压缩率两种          |   输入      |
|       level     |          压缩等级，默认为1                      |   输入          |
|    dst_buffer    |           输出buffer，用于存放压缩后的字符串       |   输出   |
|    src_size     |              输出buffer长度        |   输出        |

### 解压缩接口：
```
int glz_decompress_default(const glz_encoder * const glz_decode_state,
                           char *dst_buffer,
                           unsigned dst_size,
                           const char * const src_buffer,
                           unsigned src_size,
                           const encode_model encodeInf);
```

参数释义
|    参数名  |               描述             |    输入/输出     |
| :--------: | :----------------------------: | :------------: |
|     glz_encode_state   |           压缩算法上下文           |   输入    |
|    src_buffer    |           输入buffer，用于存放待解压字符串      |   输入   |
|    src_size     |             输入buffer长度         |   输入        |
|     encodeInf    |            压缩模式，支持高性能与高压缩率两种          |   输入      |
|    dst_buffer    |           输出buffer，用于存放解压后的字符串       |   输出   |
|    src_size     |              输出buffer长度         |   输出        |

### 压缩上下文初始化接口：
```
glz_encoder *glz_compress_initial(long unsigned src_size);
```

参数释义
|    参数名       |               描述             |    输入/输出     |
| :-------------: | :----------------------------: | :------------: |
|    src_size     |             输入buffer长度         |   输入        |

### 压缩上下文删除接口：
```
void glz_compress_delete(glz_encoder **glz_state);
```

参数释义
|    参数名       |               描述             |    输入/输出     |
| :-------------: | :----------------------------: | :------------: |
|    glz_state     |             压缩算法上下文         |   输入        |