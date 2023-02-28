# **CA demo**

简体中文 | [English](README_en.md)

## 介绍
**CA demo** 展示非安全侧示例，如果需要运行，请和TA demo配合使用

## 使用依赖

1. 确保环境上已安装机密计算SDK，需要同时安装相同版本的 `kunpeng-sc-1.3.0` 和 `kunpeng-sc-devel-1.3.0`，下载地址[机密计算SDK](https://mirrors.huaweicloud.com/kunpeng/archive/Kunpeng_SDK/itrustee/)
2. 确保tzdriver正常加载，执行 `lsmod | grep tzdriver` 查看
3. 确保守护进程正常启动，执行 `ps -ef | grep teecd` 查看

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 切入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/sc-sdk/examples/CA/
   ```

3. 安装CA demo

   ```shell
   chmod +x ./install.sh
   ./install.sh
   ```
