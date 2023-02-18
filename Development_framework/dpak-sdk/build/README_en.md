# DPAK Automatic Deployment Guide

English | [简体中文](README.md)

## Introduction
SP680 SmartNIC, applicable to virtualization and protocol parsing scenarios. This document describes how to automatically deploy the virtualization and OVS software for the SP680 SmartNIC. The automatic deployment script is for reference only.

## Automatic Deployment of Virtualization Offload

### Environment Requirements

1. This document supports only four-node deployment. OpenStack consists of controller, compute, network, and storage nodes. This document describes one controller node, one storage node, and two compute nodes. The network node and controller node are deployed on the same server.
2. The SP680 SmartNIC is installed only on compute nodes. You need to enable the IOMMU in the BIOS, configure memory hugepages, and install the iNIC driver.
3. The SELinux and firewall must be disabled on all nodes, and the nodes can access the Internet.
4. Only the `openEuler 20.03 LTS SP1` OS is supported.


### Instructions

1. To configure and modify the 'hosts' file for all nodes, add the following content to the `/etc/hosts` file:
```
9.88.14.2 controller
9.88.14.4 compute01
9.88.14.5 compute02
9.88.14.6 storager
```

2. Before deploying the controller node and compute node, install the Python dependency:
```
pip3 install pymysql
```

3. To deploy the controller node, create an `.admin-openrc` file in the `/root` directory. The file content is as follows:
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

Run the following command for the creation to take effect:
```
source /root/.admin-openrc
```

Run the deployment script:
```
cd dpak_studio
hostnamectl set-hostname controller
python3 install.py
```

**During the installation, you need to enter the password for multiple times. Only the password 123456 is supported for automatic deployment.**

4. Before deploying compute nodes, obtain the `DPU-Solution-dpak-tools-host-repo_1.0.0_aarch64.zip` and `DPU-Solution-dpak-runtime-host-repo_1.0.0_aarch64.zip` software packages and save them to the `dpak_studio` directory. The SSH password-free connection must be configured between two compute nodes.
```
# Compute node 1
cd dpak_studio
hostnamectl set-hostname compute01
python3 install.py

# Compute node 2
cd dpak_studio
hostnamectl set-hostname compute02
python3 install.py
```

5. To deploy storage nodes, configure password-free login for the controller node and compute node. Before the deployment, install the Crudini on the storage node.

Install the Crudini:
```
wget https://github.com/pixelb/crudini/releases/download/0.9.3/crudini-0.9.3.tar.gz
tar -xf crudini-0.9.3.tar.gz
cd crudini-0.9.3
sed -i "s?env python?env python3?" crudini
cp crudini /usr/bin
```

Install Ceph:
```
hostnamectl set-hostname storager
cd dpak_studio
mkdir /etc/ceph
cp ceph_install.sh /etc/ceph
cd /etc/ceph
# enp125s0f0 is the network port on the user storage plane, and sdb, sdc, and sdd are the blank block devices for creating OSDs.
sh ceph_install.sh enp125s0f0 sdb sdc sdd
```


## Automatic Deployment of OVS Offload

### Environmental Requirements

1. This document applies only to single-node deployment. To install the SP680 SmartNIC on a physical machine, enable the IOMMU in the BIOS, configure hugepages, and install the iNIC driver.
2. The SELinux and firewall must be disabled on the node, and the node can access the Internet.
3. Only the `openEuler 20.03 LTS SP1` OS is supported.

### Instructions

1. Install the NIC driver:
Before the installation, obtain the related driver package and `Hinic3_flash.bin` file and save it to the `kunpeng-dpak-1.0.1-aarch64-linux/ovs_install/driver` directory."

| Drivers | 
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
Restart the server after the driver is installed.

2. Enable VirtIO-Net:

```
bash virtio_envable.sh
```

After the server is restarted, the VirtIO-Net device is disabled. Run the preceding command to enable the VirtIO-Net device.
If the script needs to be automatically executed upon startup, perform the following operations:


1).Add a service to the script. For reference, see the following `virtio-enable.service` file:
```
[Unit]
Description=DPAK virtio enable script

[Service]
Type=simple
ExecStart=sh <path>/virtio_envable.sh

[Install]
WantedBy=multi-user.target
```

Replace `<path>` with the absolute path of the script.

2).Add the `virtio-enable.service` file in the `/usr/lib/systemd/system`'` directory to enable automatic startup.
```
systemctl enable virtio-enable
```

3. Deploy the OVS:
Before the installation, obtain the `DPU-solution-dpak-runtime-host-repo_1.0.0_aarch64.zip` software package, decompress and save it to the `kunpeng-dpak-1.0.1-aarch64-linux/package` directory.
If the software packages of DPAK, DPDK, and OVS are used, place the corresponding RPM packages in the empty `kunpeng-dpak-1.0.1-aarch64-linux/ovs_install/package` directory.

```
bash install_env.sh
```
