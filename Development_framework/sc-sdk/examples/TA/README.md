# **TA demo**

简体中文 | [English](README_en.md)

## 介绍
**TA demo** 展示安全侧示例，如果需要运行，请和CA demo配合使用

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
   cd ./devkitdemo/sc-sdk/examples/TA/
   ```

3. 修改./cloud/config_cloud.ini中的开发者私钥和config的绝对路径

   ```shell
   vim ./cloud/config_cloud.ini
   # 修改signKey为private_key.pem绝对路径
   # signKey = /home/kunpeng/TA/cloud/TA_cert/private_key.pem
   # 修改configPath为config绝对路径
   # configPath = /home/kunpeng/TA/cloud/signed_config/config
   ```

4. 将 ./ 目录下的manifest.txt 文件替换成,申请开发者证书时使用的manifest.txt文件
   
5. 安装TA demo

   ```shell
   chmod +x ./install.sh
   ./install.sh
   ```

