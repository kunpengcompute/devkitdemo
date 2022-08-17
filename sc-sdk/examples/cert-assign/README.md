# cert-assign demo

简体中文 | [English](README_en.md)

## 介绍
cert-assign demo是使用鲲鹏机密计算特性开发的证书签发系统的参考实现

[鲲鹏机密计算特性介绍](https://www.hikunpeng.com/zh/developer/boostkit/confidential-computing)


## 使用依赖

1. 确保环境上以安装机密计算SDK，需要同时安装 `kunpeng-sc-1.0.1` 和 `kunpeng-sc-devel-1.0.1` ，下载地址[机密计算SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/)
2. 确保tzdriver正常加载，执行 `lsmod | grep tzdriver` 查看
3. 确保守护进程正常启动，执行 `ps -ef | grep teecd` 查看

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/sc-sdk/examples/cert-assign/
   ```

3. 修改./TA/config_cloud.ini中的开发者私钥和config的绝对路径

   ```shell
   vim ./TA/config_cloud.ini
   # 修改signKey为private_key.pem绝对路径
   # signKey = /home/kunpeng/devkitdemo/sc-sdk/examples/cert-assign/TA/TA_cert/private_key.pem
   # 修改configPath为config绝对路径
   # configPath = /home/kunpeng/devkitdemo/sc-sdk/examples/cert-assign/TA/signed_config/config
   ```

4. 将 ./TA/ 目录下的manifest.txt 文件替换成,申请开发者证书时使用的manifest.txt文件

5. 安装TA demo

   ```shell
   cd ./TA/
   ./install.sh
   ```

6. 将../CA/cert_assign_ca.h中的TA_UUID修改为开发者申请证书的uuid

   ```shell
   vim ../CA/cert_assign_ca.h
   # #define TA_UUID                "d17fa523-ab23-47fa-97b4-ede34acf748c"    //开发者证书UUID[需根据实际情况进行修改]
   ```

7. 安装CA demo

   ```shell
   cd ../CA/
   ./install.sh
   ```

8. 运行demo

   ```shell
   /vendor/bin/cert-assign
   ```


## 编译生成适用于TEE的libcrypto.a

下载openssl1.1.1k代码：
```
wget https://github.com/openssl/openssl/archive/refs/tags/OpenSSL_1_1_1k.tar.gz
```

解压：
```
tar -zxf OpenSSL_1_1_1k.tar.gz
```

合入适用于TEE的patch:
```
cd openssl-OpenSSL_1_1_1k
patch -p1 < /path/to/openssl_for_tee.patch
```

编译，编译后会生成适用于TEE的libcrypto.a：
```
./config no-sock no-shared CFLAGS="-DOPENSSL_RAND_TEE -DNO_SYSLOG \
   -DOPENSSL_NO_UI_CONSOLE \
   -DOPENSSL_NO_SECURE_MEMORY -DOPENSSL_NO_STDIO \
   -DOPENSSL_NO_STATIC_ENGINE -D__STDC_NO_ATOMICS__ \
   -O -w -fno-short-enums -fno-omit-frame-pointer \
   -fstack-protector-strong -Wextra \
   -nostdinc -nodefaultlibs -march=armv8-a -Os -Wno-main -fPIC \
   -Wno-error=unused-parameter -Wno-error=unused-but-set-variable \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/aarch64 \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/aarch64/bits \
   -I/usr/include/itrustee_sdk/thirdparty/musl/libc/arch/generic \
   -I/usr/local/kunpeng-sc-devel/source/liboundscheck/include \
   -I/usr/include/itrustee_sdk/TA \
   -I/usr/include/itrustee_sdk/TA/huawei_ext"
make -j 4
```