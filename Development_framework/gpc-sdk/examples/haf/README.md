# **HAF demo**

简体中文 | [English](README_en.md)

## 介绍

1. [HAF](https://kunpeng-repo.obs.cn-north-4.myhuaweicloud.com/Kunpeng%20BoostKit/Kunpeng%20BoostKit%2022.0.RC3/BoostKit-haf_1.2.0.zip)同构加速框架（Homogeneous Acceleration Framework），替换原有GRPC通信下推框架的server/client接口，通过注解形式实现下推。

## 编译依赖

1. 确保已安装 **maven**，并且配置好远程仓库；
2. 确保已安装 [BiShengJDK 1.8](https://mirror.iscas.ac.cn/kunpeng/archive/compiler/bisheng_jdk/bisheng-jdk-8u262-linux-aarch64.tar.gz)
3. 确保已安装 **haf-1.2.0.jar**，该文件位于[HAF](https://kunpeng-repo.obs.cn-north-4.myhuaweicloud.com/Kunpeng%20BoostKit/Kunpeng%20BoostKit%2022.0.RC3/BoostKit-haf_1.2.0.zip)安装包haf-host包内

## 使用依赖
1.确保在主节点和卸载节点完成[HAF](https://kunpeng-repo.obs.cn-north-4.myhuaweicloud.com/Kunpeng%20BoostKit/Kunpeng%20BoostKit%2022.0.RC3/BoostKit-haf_1.2.0.zip)部署，操作步骤请参考[文档](https://www.hikunpeng.com/document/detail/zh/kunpengbds/appAccelFeatures/sqlqueryaccelf/kunpengomnidata_20_0018.html)

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/haf/
   ```

3. 编译jar包

   ```shell
   mvn install
   ```

4. 运行demo

   ```shell
   cd ./target/
   java -jar haf-1.0-SNAPSHOT-jar-with-dependencies.jar 20 30
   ```

5. 清理demo

   ```shell
   mvn clean
   ```