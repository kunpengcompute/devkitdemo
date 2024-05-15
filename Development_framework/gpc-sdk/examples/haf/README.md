# **HAF demo**

简体中文 | [English](README_en.md)

## 介绍

1. [HAF](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/system-lib/ug-haf/kunpengaccel_haf_06_0002.html)同构加速框架（Homogeneous Acceleration Framework），替换原有GRPC通信下推框架的server/client接口，通过注解形式实现下推。

## 编译依赖

1. 确保已安装 **maven**，并且配置好远程仓库；
2. 确保已安装 **BiShengJDK 1.8**
3. 确保已安装 **haf-1.3.0**(Kunpeng-gpc SDK已集成haf，在/usr/local/kunpeng-gpc/haf目录下存在BoostKit-haf_1.3.0.zip)

## 使用依赖
1.确保在主节点和卸载节点参考[文档](https://www.hikunpeng.com/document/detail/zh/kunpengaccel/system-lib/ug-haf/kunpengaccel_haf_06_0012.html)部署完成**HAF**

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
   cd ..
   mvn clean
   ```
