#!/bin/bash
# 构建dev软件包

version=$1

if [ ! ${version} ]; then
    version="1.0.0"
fi

package_dir="kunpeng-dpak-devel-${version}-aarch64-linux"

rm -rf $package_dir
mkdir $package_dir

cd src

cd qemu
git diff v5.0.0 HEAD > ../qemu-5.0.0.patch
cd ..

cd libvirt
git diff v6.9.0 HEAD > ../libvirt-6.9.0.patch
cd ..

cd ..

cp -rf ../examples $package_dir

cd $package_dir

mkdir patch
mkdir patch/openstack patch/ovs patch/dpdk patch/libvirt patch/qemu

cp ../src/qemu-5.0.0.patch patch/qemu
cp ../src/libvirt-6.9.0.patch patch/libvirt
cp ../src/dpdk-21.11.patch patch/dpdk
cp ../src/ovs-2.14.2.patch patch/ovs

cp -rf ../src/openstack ./patch

cd ..

tar -zcf $package_dir.tar.gz $package_dir
