# **CA demo**

简体中文 | [English](README_en.md)

## 介绍
**CA demo** 展示非安全侧示例，如果需要运行，请和TA demo配合使用

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
   cd ./devkitdemo/sc-sdk/examples/CA/
   ```

3. 安装CA demo

   ```shell
   chmod +x ./install.sh
   ./install.sh
   ```
