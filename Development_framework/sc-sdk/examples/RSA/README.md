# **RSA demo**

简体中文 | [English](README_en.md)

## 介绍
**RSA demo** 展示使用TEE安全存储对文件进行加密保存的CA和TA的代码示例

## 使用依赖

1. 确保已安装 **kunpeng-sc-devel**
2. 确保 **tzdriver** 正常加载，执行 **lsmod | grep tzdriver**
3. 确保守护进程正常启动，执行 **ps -ef | grep teecd**

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/sc-sdk/examples/RSA/
   ```

3. 修改./TA/cloud/config_cloud.ini中的开发者私钥和config的绝对路径

   ```shell
   vim ./TA/cloud/config_cloud.ini
   # 修改signKey为private_key.pem绝对路径
   # signKey = /home/RSA/TA/cloud/TA_cert/private_key.pem
   # 修改configPath为config绝对路径
   # configPath = /home/RSA/TA/cloud/signed_config/config
   ```

4. 将 ./TA/ 目录下的manifest.txt 文件替换成,申请开发者证书时使用的manifest.txt文件

5. 安装TA demo

   ```shell
   cd ./TA/
   chmod +x ./install.sh
   ./install.sh
   ```

6. 修改../CA/rsa_ca_demo.c中的RSA_CRYPTO_uuid和RSA_CRYPTO_DEMO_TA_PATH

   ```shell
   vim ../CA/rsa_ca_demo.c
   # 修改RSA_CRYPTO_uuid为开发者证书UUID[需根据实际情况进行修改]
   # static const TEEC_UUID RSA_CRYPTO_uuid = {
   #    0xf68fd704, 0x6eb1, 0x4d14,
   #    { 0xb2, 0x18, 0x72, 0x28, 0x50, 0xeb, 0x3e, 0xf0 }
   # };
   # 修改RSA_CRYPTO_DEMO_TA_PATH为/data/${uuid}.sec
   # #define RSA_CRYPTO_DEMO_TA_PATH        "/data/f68fd704-6eb1-4d14-b218-722850eb3ef0.sec"
   ```

7. 安装CA demo

   ```shell
   cd ../CA/
   chmod +x ./install.sh
   ./install.sh
   ```

8. 运行demo

   ```shell
   /vendor/bin/rsa_demo
   ```
