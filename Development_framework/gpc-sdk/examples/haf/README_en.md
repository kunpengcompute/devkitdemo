# **HAF demo**

English | [简体中文](README.md)

## Introduction

1. [HAF](https://kunpeng-repo.obs.cn-north-4.myhuaweicloud.com/Kunpeng%20BoostKit/Kunpeng%20BoostKit%2022.0.RC3/BoostKit-haf_1.2.0.zip) the homogeneous acceleration framework  to replace the server/client interface of the original gRPC communication pushdown framework, and implements pushdown using annotations..

## Compile Dependencies

1. Ensure that **maven** has been installed, and repository mirror could connect.
2. Ensure that [BiShengJDK 1.8](https://mirror.iscas.ac.cn/kunpeng/archive/compiler/bisheng_jdk/bisheng-jdk-8u262-linux-aarch64.tar.gz) has been installed.
3. Ensure that **haf-1.2.0.jar** has been installed，this file is contained by [HAF](https://kunpeng-repo.obs.cn-north-4.myhuaweicloud.com/Kunpeng%20BoostKit/Kunpeng%20BoostKit%2022.0.RC3/BoostKit-haf_1.2.0.zip) haf-host package .

## Run Dependencies
1. Ensure that both of offload node and host node [HAF](https://kunpeng-repo.obs.cn-north-4.myhuaweicloud.com/Kunpeng%20BoostKit/Kunpeng%20BoostKit%2022.0.RC3/BoostKit-haf_1.2.0.zip) have been installed, you can reffer to [document](https://www.hikunpeng.com/document/detail/en/kunpengbds/appAccelFeatures/sqlqueryaccelf/kunpengomnidata_20_0018.html).



## Guidance

1. Obtain the code.

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. Switch to the project root path.

   ```shell
   cd ./devkitdemo/Development_framework/gpc-sdk/examples/haf/
   ```

3. Compile the jar.

   ```shell
    mvn install
   ```

4. Run the demo.

   ```shell
   cd ./target/
   java -jar haf-1.0-SNAPSHOT-jar-with-dependencies.jar 20 30
   ```

5. Clean up the demo.

   ```shell
   mvn clean
   ```