# **GLZ Demo**

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
$ cd ./devkitdemo/Development_framework/dc-sdk/examples/glz
```

Step 3. Compile Demo.
```sh
$ rm -rf build
$ mkdir -p build

$ cd build
$ cmake ..
$ make
```

Step 4. Run Demo.
```sh
$ ./glz_demo
```
Expected Result：
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

## Interface
### Compress API：
```
int glz_compress_default(const glz_encoder * const glz_encode_state,
                         char *dst_buffer,
                         unsigned dst_size,
                         const char * const src_buffer,
                         unsigned src_size,
                         const encode_model encodeInf,
                         int level);;
```

Parameter
|    Parameter Name  |              Description               |    Input/Output     |
| :--------: | :----------------------------: | :------------: |
|     glz_encode_state   |           Compress context           |  Input    |
|    src_buffer    |           Input buffer，used to store strings to be comppressed       |   Input   |
|    src_size     |             Length of input buffer         |   Input        |
|     encodeInf    |            Compression mode，supporting high compression ratio and high compression performance          |   Input      |
|       level     |          Compression level，default 1                      |   Input          |
|    dst_buffer    |           Output buffer，used to store comppressed strings       |   Output   |
|    src_size     |              Length of output buffer         |   Output        |

### Decompress API：
```
int glz_decompress_default(const glz_encoder * const glz_decode_state,
                           char *dst_buffer,
                           unsigned dst_size,
                           const char * const src_buffer,
                           unsigned src_size,
                           const encode_model encodeInf);
```

Parameter
|    Parameter Name  |              Description               |    Input/Output     |
| :--------: | :----------------------------: | :------------: |
|     glz_encode_state   |           Compress context           |  Input    |
|    src_buffer    |           Input buffer，used to store strings to be decomppressed       |   Input   |
|    src_size     |             Length of input buffer         |   Input        |
|     encodeInf    |            Compression mode，supporting high compression ratio and high compression performance          |   Input      |
|    dst_buffer    |           Output buffer，used to store decomppressed strings       |   Output   |
|    src_size     |              Length of output buffer         |   Output        |


### Compress context initializing API：
```
glz_encoder *glz_compress_initial(long unsigned src_size);
```

Parameter
|    Parameter Name  |              Description               |    Input/Output     |
| :--------: | :----------------------------: | :------------: |
|    src_size     |             Length of input buffer         |   Input        |

### Compress context deleting API：
```
void glz_compress_delete(glz_encoder **glz_state);
```

|    Parameter Name  |              Description               |    Input/Output     |
| :--------: | :----------------------------: | :------------: |
|    glz_state     |            Compress context        |   Input       |
