# **HAF demo**

English | [简体中文](README.md)

## Introduction

1. [HAF](https://www.hikunpeng.com/document/detail/en/kunpengaccel/system-lib/ug-haf/kunpengaccel_haf_06_0002.html) the homogeneous acceleration framework  to replace the server/client interface of the original gRPC communication pushdown framework, and implements pushdown using annotations..

## Compile Dependencies

1. Ensure that **maven** has been installed, and repository mirror could connect.
2. Ensure that **BiSheng JDK 1.8** has been installed.
3. Ensure that **haf-1.3.0** has been installed.(New Kunpeng-gpc SDK has generated haf, the BoostKit-haf_1.3.0.zip exists in /usr/local/kunpeng-gpc/haf.) 

## Run Dependencies
1. Ensure that both of offload node and host node [HAF](https://www.hikunpeng.com/document/detail/en/kunpengaccel/system-lib/ug-haf/kunpengaccel_haf_06_0012.html) have been installed.



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
   cd ..
   mvn clean
   ```
