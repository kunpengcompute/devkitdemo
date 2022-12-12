# OVS卸载自动化部署指南

#### 介绍
SP680智能网卡，适用于虚拟化、协议解析场景。本文档为SP680配套OVS软件的自动haul安装指南。此文档仅支持单节点部署，自动部署脚本仅供参考。
#### 环境要求

1. 物理机要安装SP680智能网卡，需要在BIOS中开启IOMMU、配置内存大页并安装智能网卡驱动。
2. 节点需要关闭SELinux和防火墙，并能正常访问外部网络。
3. 仅支持openEuler 20.03 LTS SP1 操作系统。

#### 使用说明

1. 安装网卡驱动：
安装前请先获取相关驱动包和Hinic3_flash.bin 文件，放到'ovs_install/driver'目录中。

| 驱动 | 
| ---- | 
| hinic3 | 
| hinicadm3 |
| hiovs3 | 
| hiovs3-lib | 
| hisdk3 | 

```
cd ovs_studio
bash install_driver.sh
```
驱动安装完后重启服务器。

2. 使能virtio-net设备：

```
cd ovs_studio
bash virtio_enable.sh
```
重新启动服务器后，virtio-net设备处于未使能状态，需要对其进行使能。可以执行上面命令使能virtio-net设备。
如果需要开机自动执行此脚本，可以进行如下操作。

1).为脚本添加一个服务，参考的virtio-enable.service文件如下:
```
[Unit]
Description=DPAK virtio enable script

[Service]
Type=simple
ExecStart=sh <path>/virtio_enable.sh

[Install]
WantedBy=multi-user.target
```

其中，<path>替换为脚本所在的绝对路径。

2).将virtio-enable.service文件放入'/usr/lib/systemd/system' 目录，设置开机自启动。
systemctl enable virtio-enable

3. 部署ovs：
安装前请先获取DPU-solution-dpak-runtime-host-repo_1.0.0_aarch64.zip 软件包，解压并放到'ovs_install/../package'目录中。
如果dpak、dpdk和openvswitch均使用自己的包，请把对应的rpm包放到空的'ovs_install/package'目录中。
```
cd ovs_studio
bash install_env.sh
```