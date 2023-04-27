# cert-assign demo

简体中文 | [English](README_en.md)

## 介绍
cert-assign demo是使用鲲鹏机密计算特性开发的证书签发系统的参考实现

[鲲鹏机密计算特性介绍](https://www.hikunpeng.com/zh/developer/boostkit/confidential-computing)

此demo主要用于为用户签发X509证书的场景，主要功能是通过创建根证书（支持RSA和SM2算法），然后对用户提交的证书请求文件进行签名，生成X509证书。使用TEE对根证书的私钥加密保存，可以保证私钥不会泄露，根证书生成和用户证书的签发在TEE侧进行计算，可以保证计算过程不被攻击。

## 使用依赖

1. 确保环境上已安装机密计算SDK，需要同时安装相同版本的 `kunpeng-sc-1.3.0` 和 `kunpeng-sc-devel-1.3.0`，下载地址[机密计算SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/)
2. 确保tzdriver正常加载，执行 `lsmod | grep tzdriver` 查看
3. 确保守护进程正常启动，执行 `ps -ef | grep teecd` 查看

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 进入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/sc-sdk/examples/cert-assign/
   ```

3. 修改`TA/config_cloud.ini`中的开发者私钥和config文件的绝对路径

   ```shell
   vim TA/config_cloud.ini
   # 修改signKey为private_key.pem绝对路径
   # secSignKey = /usr/local/kunpeng-sc-devel/examples/cert-assign/TA/TA_cert/private_key.pem
   # 修改configPath为config绝对路径
   # configPath = /usr/local/kunpeng-sc-devel/examples/cert-assign/TA/signed_config/config
   ```

4. 将 `./TA/` 目录下的 `manifest.txt` 文件替换成申请开发者证书时使用的 `manifest.txt` 文件

5. 安装TA demo，编译TA时需要静态链接适用于TEE的OpenSSL加解密库，安装`kunpeng-sc-devel-1.1.0`后会将适用于TEE的OpenSSL加解密库安装到 `/usr/local/kunpeng-sc-devel/examples/cert-assign/lib/libcrypto.a` ，也可以参考 **编译生成适用于TEE的libcrypto.a** 章节自行编译。

   ```shell
   cd ./TA/
   ./install.sh
   ```

6. 将 `../CA/cert_assign_ca.h` 中的 `TA_UUID` 修改为开发者申请证书的uuid

   ```shell
   vim ../CA/cert_assign_ca.h
       #define TA_UUID                "d17fa523-ab23-47fa-97b4-ede34acf748c"    //开发者证书UUID[需根据实际情况进行修改]
   ```

7. 安装CA demo

   ```shell
   cd ../CA/
   ./install.sh
   ```

8. 运行demo

   默认用户名：**admin**

   密码：**admin**

   可以使用OpenSSL制作证书请求文件，然后使用此应用对证书请求文件进行签名生成X509证书，例如：
   ```
   # 生成私钥
   openssl genrsa -out cert.key 2048
   # 生成证书请求文件
   openssl req -new -sha256 -key cert.key -subj "/C=CN/CN=test" -out cert.csr
   ```

   - 首次运行需要先生成根证书，需要输入根证书的common name和密钥算法（支持RSA和SM2），根证书生成后会将根证书的私钥使用TEE的安全存储功能加密保存，根证书会返回给TA保存到 `root_cert.pem` 。
   - 根证书生成后可以输入`show`查看根证书的信息。
   - 生成证书请求文件，输入`sign`可以使用根证书对证书请求文件进行签名后生成X509证书。
   - 删除TEE内保存的根证书和私钥： `rm -rf /var/itrustee/sec_storage_data/sec_storage_data/cert_assign` 。

   运行demo：
   ```shell
   /vendor/bin/cert-assign
   ```


## 编译生成适用于TEE的libcrypto.a

1. 下载OpenSSL-1.1.1k代码：
```shell
wget https://github.com/openssl/openssl/archive/refs/tags/OpenSSL_1_1_1k.tar.gz
```

2. 解压：
```shell
tar -zxf OpenSSL_1_1_1k.tar.gz
```

3. 合入适用于TEE的patch，patch存放在patch目录:
```shell
cd openssl-OpenSSL_1_1_1k
# patch路径根据实际路径修改
patch -p1 < /path/to/openssl_for_tee.patch
```

4. 编译，编译前需要先安装 `kunpeng-sc` 和 `kunpeng-sc-devel` ， 编译后会生成适用于TEE的`libcrypto.a`：
```shell
./config no-sock no-shared CFLAGS="-DOPENSSL_RAND_TEE -DNO_SYSLOG \
   -DOPENSSL_NO_UI_CONSOLE \
   -DOPENSSL_NO_SECURE_MEMORY -DOPENSSL_NO_STDIO \
   -DOPENSSL_NO_STATIC_ENGINE -D__STDC_NO_ATOMICS__ \
   -O -w -fno-short-enums -fno-omit-frame-pointer -Wextra \
   -nostdinc -nodefaultlibs -march=armv8-a -Os -Wno-main -fPIC \
   -Wno-error=unused-parameter -Wno-error=unused-but-set-variable \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/aarch64 \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/aarch64/bits \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/generic \
   -I/usr/include/itrustee_sdk/TA \
   -I/usr/include/itrustee_sdk/TA/huawei_ext"
make -j 4
```