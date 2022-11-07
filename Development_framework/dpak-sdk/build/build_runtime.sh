#!/bin/bash
# 构建runtime软件包

version=$1

if [ ! ${version} ]; then
    version="1.0.0"
fi

package_dir="kunpeng-dpak-${version}-aarch64-linux"

rm -rf $package_dir
mkdir $package_dir

cp -rf package $package_dir
cp -rf dpak_studio $package_dir
mv $package_dir/dpak_studio/README.md $package_dir
rm -f $package_dir/dpak_studio/README.en.md
rm -rf $package_dir/dpak_studio/.git
rm -rf $package_dir/dpak_studio/.vscode
rm -rf $package_dir/dpak_studio/.idea
dos2unix $package_dir/dpak_studio/ceph_install.sh

tar -zcf $package_dir.tar.gz $package_dir
