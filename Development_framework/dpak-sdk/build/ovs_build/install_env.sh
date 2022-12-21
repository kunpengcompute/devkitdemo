#!/usr/bin/bash

# Copyright 2022 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

current_dir=$(cd $(dirname $0);pwd)
package_dir=${current_dir}/../package 

exit_or_not(){
    if [ $(echo $?) -ne 0 ];then
        echo "--------------$1 build failed-------------------"
        exit 1
    else
        echo "--------------$1 build success-------------------"
    fi
}

# install dependencies
yum install -y dtc rdma-core libatomic rdma-core-devel libpcap-devel zlib-devel openssl-devel unbound libunwind
if [ $? -ne 0 ];then
    echo "Failed to install the dependency.Check the network and original yum configuration."
    exit 1
fi
# 1 install dpak
rpm -e `rpm -qa | grep in220-sdk` --nodeps
rpm -e `rpm -qa | grep dpak | grep ovs` --nodeps
if [ -d ${current_dir}/package ];then
    if [ -f ${current_dir}/package/dpak*.rpm ];then
        rpm -ivh ${package_dir}/dpak*.rpm
    fi
else
    # default install
    rpm -Uvh ${package_dir}/dpak*.rpm
fi

exit_or_not 'install dpak'

#2 install dpdk
rpm -e --nodeps `rpm -qa | grep dpdk`
if [ -d ${current_dir}/package ];then
    if [ -f ${current_dir}/package/dpdk*.rpm ];then
        rpm -Uvh ${package_dir}/dpdk*.rpm --force
    fi
else
    # default install
    rpm -Uvh ${package_dir}/dpdk/dpdk-21.11-1.aarch64.rpm --force
    rpm -Uvh ${package_dir}/dpdk/dpdk-devel-21.11-1.aarch64.rpm --force
fi

ldconfig

# 3 install openvswitch(ovs)
rpm -e --nodeps `rpm -qa | grep openvswitch`
if [ -d ${current_dir}/package ];then
    if [ -f ${current_dir}/package/openvswitch*.rpm ];then
        rpm -Uvh ${package_dir}/package/openvswitch*.rpm --force
    fi
else
    # default install
    rpm -Uvh ${package_dir}/openvswitch/openvswitch-2.14.2-1.aarch64.rpm --force
    rpm -Uvh ${package_dir}/openvswitch/openvswitch-devel-2.14.2-1.aarch64.rpm --force
fi

systemctl restart rsyslog
# create ovs log directory
mkdir -p /var/log/openvswitch
[ ! -f /usr/lib64/libevent-2.0.so.5 ] && ln -s /usr/lib64/libevent-2.1.so.6 /usr/lib64/libevent-2.0.so.5
[ ! -f /usr/lib64/libevent-2.0.so.5 ] && ln -s /usr/lib64/libcrypto.so.1.1 /usr/lib64/libcrypto.so.10

# configure OVS startup items
echo -e "\e[1;33mIt may take a few minutes to deploy the OVS environment, please wait...\e[0m"
service openvswitch start

hwoff_pf_pci=$(hinicadm3 info | grep NIC | cut -c 19-30 | awk 'NR==1')
ovs-vsctl --no-wait set Open_vSwitch . other_config:dpdk-init=true
ovs-vsctl --no-wait set Open_vSwitch . other_config:dpdk-extra="--iova-mode=pa"
ovs-vsctl --no-wait set Open_vSwitch . other_config:pmd-cpu-mask=0x6
ovs-vsctl --no-wait set Open_vSwitch . other_config:dpdk-lcore-mask=0x6
ovs-vsctl --no-wait set Open_vSwitch . other_config:offload-cpu-mask=0x18
ovs-vsctl --no-wait set Open_vSwitch . other_config:dpdk-socket-mem="8192"
ovs-vsctl --no-wait set Open_vSwitch . other_config:hwoff-pf-pci=${hwoff_pf_pci}
ovs-vsctl --no-wait set Open_vSwitch . other_config:hw-offload=true
service openvswitch restart

virtio_1=$(hinicadm3 info | grep VirtIO | awk -F "(" '{print $1}' | awk -F "--------" '{print $2}' | sed -n '1p')
virtio_2=$(hinicadm3 info | grep VirtIO | awk -F "(" '{print $1}' | awk -F "--------" '{print $2}' | sed -n '2p')

echo 2 > /sys/bus/pci/devices/${virtio_1}/sriov_numvfs
echo 2 > /sys/bus/pci/devices/${virtio_2}/sriov_numvfs

service openvswitch restart
sleep 5s

# resolve MAC address conflicts

vfs=$(lspci | grep Virtio | awk '{print $1}' |sed -n '3,6p')
vf_pci_part=$(echo ${virtio_1} | cut -c 1-8)
ovs-vsctl del-br br0
for i in {1..4};do ovs-vsctl del-port vf$i;done

ovs-vsctl add-br br0 -- set bridge br0 datapath_type=netdev

for i in {1..4};do
    random_1=$(head -20 /dev/urandom | cksum | cut -c 1)
    random_2=$(head -20 /dev/urandom | cksum | cut -c 1)
    random_3=$(head -20 /dev/urandom | cksum | cut -c 1)
    vf_pci=$(echo ${vfs} | awk "{print \$${i}}")
    ovs-vsctl add-port br0 vf${i} -- set interface vf${i} type=dpdk \
    options:dpdk-devargs=0000:${vf_pci},vf_mac=02:05:0${random_1}:0${random_2}:0${random_3}:0c
done