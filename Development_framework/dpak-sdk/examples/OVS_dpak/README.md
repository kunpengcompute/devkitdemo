# SP680网络加速-ovs卸载场景 demo

简体中文 | [English](README_en.md)

## 介绍

SP680智能网卡的网络加速功能用于配合服务器完成网络的卸载，此demo实现了服务器环境的基础配置检查，比如：防火墙开闭状态、配置hugepage、SP680驱动依赖是否安装、配置组等。以及ICMP协议卸载（同主机两台虚拟机互ping，查看流表的方式）。

## 运行demo须知

1. 运行环境为物理机，操作系统为openEuler 20.03 SP1。

2. 关闭防火墙。

3. 配置hugepage。

4. 安装SP680驱动依赖。

5. 部署网络ovs加速软件包，启动ovs服务。

6. 配置组网。

7. 连接服务器需要使用root用户。

8. 创建虚拟机并将VF设备直通给虚拟机，搭建两台虚拟机验证互通（可以互相ping通），并实现物理机到虚拟机之间的免密。
```
-------------          -------------
|192.168.1.2|          |192.168.1.3|
|           |----------|           |
-------------          -------------
     |                       |
 ---------              ----------
```

9. 虚拟机需配置可以带宽ssh的管理口ip，可以参考命令：virsh attach-interface vm1 --type bridge --source virbr0 --model virtio --current

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 进入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/dpak-sdk/examples/OVS_dpak/script
   ```
3. 运行demo

   ```shell
   bash run_demo.sh
   ```
