# **data-sealing demo**

## 介绍

**data-sealing demo** 展示使用TEE安全存储对文件进行加密保存的CA和TA的代码示例

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
   cd ./devkitdemo/devkitdemo-main/sc-sdk/examples/data-sealing/
   ```

3. 修改./TA/cloud/config_cloud.ini中的开发者私钥和config的绝对路径

   ```shell
   vim ./TA/cloud/config_cloud.ini
   # 修改signKey为private_key.pem绝对路径
   # signKey = /home/kunpeng/data-sealing/TA/cloud/TA_cert/private_key.pem
   # 修改configPath为config绝对路径
   # configPath = /home/kunpeng/data-sealing/TA/cloud/signed_config/config
   ```

4. 将 ./TA/ 目录下的manifest.txt 文件替换成,申请开发者证书时使用的manifest.txt文件

5. 安装TA demo

   ```shell
   cd ./TA/
   bash ./install.sh
   ```

6. 将../CA/data_sealing_ca.h中的DATA_SEALING_TA_UUID修改为开发者申请证书的uuid

   ```shell
   vim ../CA/data_sealing_ca.h
   # #define DATA_SEALING_TA_UUID        "7824610d-0e54-4448-a905-86fffeeff327"    //开发者证书UUID[需根据实际情况进行修改]
   ```

7. 安装CA demo

   ```shell
   cd ../CA/
   bash ./install.sh
   ```

8. 运行demo

   ```shell
   /vendor/bin/data-sealing
   ```
