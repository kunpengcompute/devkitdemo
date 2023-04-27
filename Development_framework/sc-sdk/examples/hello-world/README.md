# hello-world demo

简体中文 | [English](README_en.md)

## 介绍
hello-world demo是使用鲲鹏机密计算特性开发的hello-world参考实现

[鲲鹏机密计算特性介绍](https://www.hikunpeng.com/zh/developer/boostkit/confidential-computing)

此demo是一个简单的安全计算应用，包含CA和TA，包含了一个基础的安全计算应用需要实现的接口，展示了CA和TA应用数据传递的方式。

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
   cd ./devkitdemo/Development_framework/sc-sdk/examples/hello-world/
   ```

3. 修改`TA/config_cloud.ini`中的开发者私钥和config文件的绝对路径

   ```shell
   vim TA/config_cloud.ini
   # 修改signKey为private_key.pem绝对路径
   # secSignKey = /usr/local/kunpeng-sc-devel/examples/hello-world/TA/TA_cert/private_key.pem
   # 修改configPath为config绝对路径
   # configPath = /usr/local/kunpeng-sc-devel/examples/hello-world/TA/signed_config/config
   ```

4. 将 `./TA/` 目录下的 `manifest.txt` 文件替换成申请开发者证书时使用的 `manifest.txt` 文件

5. 安装TA demo

   ```shell
   cd ./TA/
   ./install.sh
   ```

6. 安装CA demo

   ```shell
   cd ../CA/
   ./install.sh
   ```

7. 运行demo

   运行demo：
   ```shell
   /vendor/bin/hello-world
   ```
