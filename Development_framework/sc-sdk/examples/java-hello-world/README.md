# java-hello-world demo

简体中文 | [English](README_en.md)

## 介绍
java-hello-world demo是使用鲲鹏机密计算高级语言特性开发的hello-world参考实现

[鲲鹏机密计算特性介绍](https://www.hikunpeng.com/zh/developer/boostkit/confidential-computing)

此demo是一个简单的安全计算高级语言应用，展示了安全计算高级语言应用开发和运行的流程。

## 使用依赖

1. 确保环境上已安装机密计算SDK，需要同时安装相同版本的 `kunpeng-sc-2.0.0` 和 `kunpeng-sc-devel-2.0.0`，下载地址[机密计算SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/)
2. 确保tzdriver正常加载，执行 `lsmod | grep tzdriver` 查看
3. 确保守护进程正常启动，执行 `ps -ef | grep teecd` 查看

## 使用教程

1. 获取代码

    ```shell
    git clone https://github.com/kunpengcompute/devkitdemo.git
    ```

2. 进入到项目根路径

    ```shell
    cd ./devkitdemo/Development_framework/sc-sdk/examples/java-hello-world/
    ```

3. 编译

    ```shell
    mkdir hellojava
    javac src/HelloWorld.java
    cp src/HelloWorld.class hellojava/
    ```

4. 打包
    先将开发者证书(ta_cert.der)和私钥(private_key.pem)放到`/usr/local/kunpeng-sc-devel/utils/pack-App/pack_tools/`目录下
    ```shell
    cp -rf hellojava /usr/local/kunpeng-sc-devel/utils/pack-App/
    cd /usr/local/kunpeng-sc-devel/utils/pack-App/
    python3 pack.py hellojava
    ```

5. 安装

    ```shell
    tee_teleport -c hellojava.sec
    ```

6. 运行

    ```shell
    tee_teleport -r HelloWorld.class -i sessionID.txt
    ```

7. 获取日志

    ```shell
    tee_teleport -o output/tee.log -i sessionID.txt
    ```

8. 卸载

    ```shell
    tee_teleport -e -i sessionID.txt
    ```
