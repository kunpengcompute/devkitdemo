# python-ml-titanic demo

## 介绍
python-ml-titanic是使用鲲鹏机密计算高级语言特性开发的python机器学习参考实现

[鲲鹏机密计算特性介绍](https://www.hikunpeng.com/zh/developer/boostkit/confidential-computing)

此demo是一个使用Python机器学习库对泰坦尼克号生存数据进行预测的安全计算高级语言应用，展示了安全计算高级语言应用开发和运行的流程。

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
    cd ./devkitdemo/Development_framework/sc-sdk/examples/python-ml-titanic/
    ```

3. 打包
    先将开发者证书(ta_cert.der)和私钥(private_key.pem)放到`/usr/local/kunpeng-sc-devel/utils/pack-App/pack_tools/`目录下
    ```shell
    cp -rf titanic /usr/local/kunpeng-sc-devel/utils/pack-App/
    cd /usr/local/kunpeng-sc-devel/utils/pack-App/
    python3 pack.py titanic
    cd -
    cp /usr/local/kunpeng-sc-devel/utils/pack-App/titanic.sec ./
    ```

4. 安装依赖库, 可以参考 **使用鸿蒙SDK编译Python依赖库** 章节编译依赖库。

    ```shell
    # 签名
    cp -rf mllib /usr/local/kunpeng-sc-devel/utils/pack-App/
    cd /usr/local/kunpeng-sc-devel/utils/pack-App/
    python3 pack.py mllib
    cd -
    cp /usr/local/kunpeng-sc-devel/utils/pack-App/mllib.sec ./
    # 安装
    tee_teleport -m mllib.sec -t python
    ```

5. 安装

    ```shell
    tee_teleport -c titanic.sec
    ```

6. 运行

    ```shell
    tee_teleport -r titanic.py -i sessionID.txt
    ```

7. 获取日志

    ```shell
    tee_teleport -o output/tee.log -i sessionID.txt
    ```

8. 卸载

    ```shell
    tee_teleport -e -i sessionID.txt
    ```

## 使用鸿蒙SDK编译Python依赖库
