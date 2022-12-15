# DPAK自动化部署指南

简体中文 | [English](README_en.md)

## 介绍
SP680智能网卡，适用于虚拟化、协议解析场景。本文档包含SP680配套的虚拟化和OVS软件的自动化部署指南。自动部署脚本仅供参考。

## 虚拟化卸载自动化部署

### 环境要求

1. 此文档仅支持4节点部署，OpenStack由控制节点，计算节点，网络节点和存储节点四大部分组成，本文档使用一台控制节点，一台存储节点，两台计算节点，其中网络节点和控制节点部署在一台服务器上。
2. SP680仅安装在计算节点，需要在BIOS中开启IOMMU、配置内存大页并安装智能网卡驱动。
3. 所有节点需要关闭SELinux和防火墙，并能正常访问外部网络。
4. 仅支持`openEuler 20.03 LTS SP1`操作系统。


### 使用说明

1. 为所有节点配置hosts，修改hosts文件，在`/etc/hosts`文件中添加如下内容。
```
9.88.14.2 controller
9.88.14.4 compute01
9.88.14.5 compute02
9.88.14.6 storager
```

2. 控制节点和计算节点部署前需要安装python依赖：
```
pip3 install pymysql
```

3. 部署控制节点，在`/root`目录创建`.admin-openrc`文件，内容如下：
```
export OS_PROJECT_DOMAIN_NAME=Default
export OS_USER_DOMAIN_NAME=Default
export OS_PROJECT_NAME=admin
export OS_USERNAME=admin
export OS_PASSWORD=123456
export OS_AUTH_URL=http://controller:5000/v3
export OS_IDENTITY_API_VERSION=3
export OS_IMAGE_API_VERSION=2
```

创建完成后执行如下命令使之生效：
```
source /root/.admin-openrc
```

执行部署脚本：
```
cd dpak_studio
hostnamectl set-hostname controller
python3 install.py
```

**安装过程中需要多次输入密码，自动化部署仅支持密码：123456**

4. 部署计算节点，安装前请先获取`DPU-Solution-dpak-tools-host-repo_1.0.0_aarch64.zip`和`DPU-Solution-dpak-runtime-host-repo_1.0.0_aarch64.zip`软件包并放到`dpak_studio`目录中。两台计算节之间需要配置免密登录。
```
# 计算节点1
cd dpak_studio
hostnamectl set-hostname compute01
python3 install.py

# 计算节点2
cd dpak_studio
hostnamectl set-hostname compute02
python3 install.py
```

5. 部署存储节点，需要配置免密登录到控制节点和计算节点。部署前需要先在存储节点安装`crudini`:

安装crudini：
```
wget https://github.com/pixelb/crudini/releases/download/0.9.3/crudini-0.9.3.tar.gz
tar -xf crudini-0.9.3.tar.gz
cd crudini-0.9.3
sed -i "s?env python?env python3?" crudini
cp crudini /usr/bin
```

安装ceph：
```
hostnamectl set-hostname storager
cd dpak_studio
mkdir /etc/ceph
cp ceph_install.sh /etc/ceph
cd /etc/ceph
# enp125s0f0为用户存储面的网口, sdb sdc sdd为用户创建OSD的空白块设备
sh ceph_install.sh enp125s0f0 sdb sdc sdd
```


## OVS卸载自动化部署

### 环境要求

1. 此文档仅支持单节点部署，物理机要安装SP680智能网卡，需要在BIOS中开启IOMMU、配置内存大页并安装智能网卡驱动。
2. 节点需要关闭SELinux和防火墙，并能正常访问外部网络。
3. 仅支持`openEuler 20.03 LTS SP1`操作系统。

### 使用说明

1. 安装网卡驱动：
安装前请先获取相关驱动包和`Hinic3_flash.bin`文件，放到`kunpeng-dpak-1.0.1-aarch64-linux/ovs_install/driver`目录中。

| 驱动 | 
| ---- | 
| hinic3 | 
| hinicadm3 |
| hiovs3 | 
| hiovs3-lib | 
| hisdk3 | 

```
cd ovs_install
bash install_driver.sh
```
驱动安装完后重启服务器。

2. 使能virtio-net设备：

```
bash virtio_envable.sh
```
重新启动服务器后，virtio-net设备处于未使能状态，需要对其进行使能。可以执行上面命令使能virtio-net设备。
如果需要开机自动执行此脚本，可以进行如下操作。

1).为脚本添加一个服务，参考的`virtio-enable.service`文件如下:
```
[Unit]
Description=DPAK virtio enable script

[Service]
Type=simple
ExecStart=sh <path>/virtio_envable.sh

[Install]
WantedBy=multi-user.target
```

其中，`<path>`替换为脚本所在的绝对路径。

2).将`virtio-enable.service`文件放入`/usr/lib/systemd/system`' `目录，设置开机自启动。
```
systemctl enable virtio-enable
```

3. 部署ovs：
安装前请先获取`DPU-solution-dpak-runtime-host-repo_1.0.0_aarch64.zip`软件包，解压并放到`kunpeng-dpak-1.0.1-aarch64-linux/package`目录中。
如果dpak、dpdk和openvswitch均使用自己的包，请把对应的rpm包放到空的`kunpeng-dpak-1.0.1-aarch64-linux/ovs_install/package`目录中。
```
bash install_env.sh
```
