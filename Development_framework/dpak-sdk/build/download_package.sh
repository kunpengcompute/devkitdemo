#!/bin/bash
# 下载 openstack dpdk libvirt qemu openvswitchvfio 软件包

openstack_package_url_list=(
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/ceilometer-13.1.2/openstack-ceilometer-central-13.1.2-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/ceilometer-13.1.2/openstack-ceilometer-common-13.1.2-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/ceilometer-13.1.2/openstack-ceilometer-compute-13.1.2-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/ceilometer-13.1.2/openstack-ceilometer-ipmi-13.1.2-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/ceilometer-13.1.2/openstack-ceilometer-notification-13.1.2-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/ceilometer-13.1.2/openstack-ceilometer-polling-13.1.2-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/ceilometer-13.1.2/python3-ceilometer-13.1.2-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/ceilometer-13.1.2/python3-ceilometer-tests-13.1.2-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-common-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-linuxbridge-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-macvtap-agent-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-metering-agent-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-ml2-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-openvswitch-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-rpc-server-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/openstack-neutron-sriov-nic-agent-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/python3-neutron-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/neutron-15.3.4/python3-neutron-tests-15.3.4-2.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-api-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-common-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-compute-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-conductor-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-migration-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-novncproxy-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-scheduler-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-serialproxy-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/openstack-nova-spicehtml5proxy-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/python3-nova-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/nova-20.6.1/python3-nova-tests-20.6.1-6.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/os-vif-1.17.0/python-os-vif-help-1.17.0-1.noarch.rpm"
    "https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/openstack/os-vif-1.17.0/python3-os-vif-1.17.0-1.noarch.rpm"
)


dpdk_url="https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/dpdk/dpdk-21.11-1.aarch64.rpm"
dpdk_devel_url="https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/dpdk/dpdk-devel-21.11-1.aarch64.rpm"
libvirt_url="https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/libvirt/dpak-libvirt-6.9.0-1.aarch64.rpm"
openvswitch_url="https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/ovs/openvswitch-2.14.2-1.aarch64.rpm"
openvswitch_devel_url="https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/ovs/openvswitch-devel-2.14.2-1.aarch64.rpm"
qemu_url="https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/qemu/dpak-qemu-5.0.0-1.aarch64.rpm"
vfio_url="https://mirrors.huaweicloud.com/kunpeng/archive/kunpeng_solution/DPU_Solution/package/SP680/vfio/dpak-vfio-4.19.90-2012.4.0.0053.oe1.aarch64.rpm"


download_openstack_package() {
    for package_url in ${openstack_package_url_list[@]}
    do
        echo "download: $package_url"
        wget $package_url
    done
}

download_package() {
    mkdir dpdk
    cd dpdk
    echo "download: $dpdk_url"
    wget $dpdk_url
    echo "download: $dpdk_devel_url"
    wget $dpdk_devel_url
    cd ..

    mkdir libvirt
    cd libvirt
    echo "download: $libvirt_url"
    wget $libvirt_url
    cd ..

    mkdir openvswitch
    cd openvswitch
    echo "download: $openvswitch_url"
    wget $openvswitch_url
    echo "download: $openvswitch_devel_url"
    wget $openvswitch_devel_url
    cd ..

    mkdir qemu
    cd qemu
    echo "download: $qemu_url"
    wget $qemu_url
    cd ..

    mkdir vfio
    cd vfio
    echo "download: $vfio_url"
    wget $vfio_url
    cd ..
}

mkdir package
cd package
download_package

mkdir openstack
cd openstack
download_openstack_package

cd ../..
