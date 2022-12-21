# SP680虚拟化卸载 demo

简体中文 | [English](README_en.md)

## 介绍

针对Openstack Train版本纳管SP680 智能网卡关键特性进行描述，包含VirtIO-net直通设备管理，热插拔VirtIO-net直通设备、带直通设备热迁移等特性。

## 运行demo须知

1. 准备四台物理机，控制节点、存储节点和两个计算节点 系统应为openEuler 20.03 SP1。

2. 分别将四台机器防火墙关闭。

3. 计算节点配置hugepage，安装SP680 ovs、热迁移驱动、Neutron配置完整、ovs网桥配置正常。

4. 控制节点Openstack 版本应在3.19-4.02之间、控制节点检查ceph节点正常、nova服务正常、neutron服务正常。

5. 连接服务器需要使用root用户。

6. 配置控制节点到存储节点、计算节点免密连接。

7. 如果进行本地编译需要在“配置目标服务器”步骤中，配置本地到控制节点的免密连接。

## 使用教程

1. 获取代码

   ```shell
   git clone https://github.com/kunpengcompute/devkitdemo.git
   ```

2. 进入到项目根路径

   ```shell
   cd ./devkitdemo/Development_framework/dpak-sdk/examples/Virtualization_dpak/Virtualization_offload
   ```
3. 运行demo

   ```shell
   source /root/.admin-openrc
   bash Virtualization_offload.sh
   ```
