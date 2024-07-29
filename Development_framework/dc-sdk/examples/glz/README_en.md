# **GLZ Demo**

English | [简体中文](README.md)

## Dependency
The following minimum requirements should be guaranteed.

- CPU: Huawei Kunpeng 920 Processor
- Server: TaiShan 200 Server (model 2280)
- Operating System: OpenEuler 20.03 LTS SP1, OpenEuler 22.03 LTS SP1, Kylin V10 SP1
- cmake3.20.5, gcc7.3.0, g++7.3.0
- BoostKit_glz Software

BoostKit_glz Software should be installed, if not, please follow the steps below: 

Step 1. Download[ BoostKit_glz Guidance Document ](https://www.hikunpeng.com/en/developer/boostkit/sds)from Kunpeng Community. (Note: Please find "Basic Acceleration Software Packages-Compression Algorithm-Download Software")  

Step 2. According to [Compression Algorithm Feature Guide-Compiling the Compression Algorithm]，finish [Downloading the Compression Algorithm Installation Package、Obtaining Compilation Dependencies、Compiling the Dynamic Library]  


## Usage Guidance
Step 1. Download Codes (Assuming at /home directory).
```sh
$ git clone https://github.com/kunpengcompute/devkitdemo.git
```

Step 2. Change to Demo Directory.
```sh
$ cd /home/devkitdemo/gpc-sdk/examples/glz
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


Step 5. Clean Demo.
```sh
$ rm -rf build
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