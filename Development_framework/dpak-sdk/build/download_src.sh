#!/bin/bash
# 下载qemu libvirt dpdk ovs openstack 补丁和参考实现代码

export GIT_SSL_NO_VERIFY=true

mkdir src
cd src

# clone qemu 代码，生成patch
git clone https://gitee.com/openeuler/qemu.git -b qemu-5.0.0-dpu

# clone libvirt 代码，生成patch
git clone https://gitee.com/openeuler/libvirt.git -b libvirt-6.9.0-dpu

wget https://gitee.com/openeuler/dpdk/raw/dpu/dpdk.patch -O dpdk-21.11.patch
wget https://gitee.com/src-openeuler/openvswitch/raw/dpu-openEuler-20.03-LTS-SP1/ovs.patch -O ovs-2.14.2.patch

# 下载 openstack patch
mkdir openstack
wget https://gitee.com/src-openeuler/openstack-nova/raw/oepkg_openstack-train_oe-20.03-LTS-SP1/nova_smartNIC.patch -O openstack/nova-20.6.1.patch
wget https://gitee.com/src-openeuler/openstack-neutron/raw/oepkg_openstack-train_oe-20.03-LTS-SP1/neutron_smartNIC.patch -O openstack/neutron-15.3.4.patch
wget https://gitee.com/src-openeuler/openstack-ceilometer/raw/oepkg_openstack-train_oe-20.03-LTS-SP1/ceilometer_smartNIC.patch -O openstack/ceilometer-13.1.2.patch
wget https://gitee.com/src-openeuler/python-os-vif/raw/oepkg_openstack-train_oe-20.03-LTS-SP1/os-vif_smartNIC.patch -O openstack/os-vif-1.17.0.patch

cd ..
