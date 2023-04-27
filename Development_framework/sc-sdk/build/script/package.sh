#!/bin/bash
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

OsName=""
OsArch=""

RootPath=$(pwd)/..
CodePath=${RootPath}/code
PatchPath=${RootPath}/patch
ScriptPath=${RootPath}/script
SrcPath=${RootPath}/src
OutputPath=${RootPath}/output
PackageVersion="1.3.0"

# workdir
DebBuild="/root/debbuild"
RpmBuild="/root/rpmbuild"

function get_os_info() {
    local osName=$(cat /etc/os-release | grep "PRETTY_NAME" | awk -F '"' '{print $2}')
    local osNameTrip=${osName// /}
    echo "os name is ${osNameTrip}"
    if [[ ${osNameTrip} =~ "openEuler20.03(LTS-SP1)" ]]; then
        OsName="openEuler"
        OsArch="Redhat"
    # in order to support tee sdk publish environment in blue zone to add Uniontech OS Server 20 Enterprise
    elif [[ ${osNameTrip} =~ "uos20SP1" ]] || [[ ${osNameTrip} =~ "UniontechOSServer20Enterprise" ]]; then
        OsName="uos"
        OsArch="Debian"
        echo "update OsName OsArch value, os is ${OsName}, arch is ${OsArch}"
    elif [[ ${osNameTrip} =~ "CentOSLinux7" ]]; then
        local cnt=$(cat /etc/redhat-release | grep "7.6.1810" | wc -l)
        if [[ ${cnt} -ne 0 ]]; then
            OsName="centOS"
            OsArch="Redhat"
        fi
    fi

    if [[ ${OsName} == "" || ${OsArch} == "" ]]; then
        echo "The current system dose not support, os is ${OsName}, arch is ${OsArch}."
        exit 1
    fi

    echo "Current os is ${OsName}, arch is ${OsArch}."
}

function install_dependence_software() {
    if [[ ${OsArch} =~ "Redhat" ]]; then
        yum install -y rpmdevtools rpmlint
        yum install -y openssl-devel zlib-devel kernel-devel gcc-c++ git dos2unix
    elif [[ ${OsArch} =~ "Debian" ]]; then
        apt-get install zlib1g-dev
        apt-get install libssl-dev
        apt-get install git
        apt-get install dos2unix
    fi
}

function apply_patch() {
    local patchDir=""
    local distDir=${SrcPath}

    if [[ ${OsName} =~ "openEuler" ]]; then
        patchDir="${PatchPath}/openEuler"
    elif [[ ${OsName} =~ "centOS" ]]; then
        patchDir="${PatchPath}/centos"
    elif [[ ${OsName} =~ "uos" ]]; then
        patchDir="${PatchPath}/uos"
    fi

    # set git user name and email
    local cnt1=$(git config --global --list | grep user.name | wc -l)
    local cnt2=$(git config --global --list|grep user.email | wc -l)
    if [[ ${cnt1} -eq 0 ]] || [[ ${cnt2} -eq 0 ]]; then
        git config --global user.name "abc"
        git config --global user.email "abc@163.com"
    fi

    echo "apply patch in ${patchDir}"
    echo "apply itrustee sdk patch"
    cd ${SrcPath}/itrustee_sdk
    git am ${patchDir}/itrustee_sdk/*.patch
    rm -rf *.patch

    echo "apply itrustee client patch"
    cd ${SrcPath}/itrustee_client
    git am ${patchDir}/itrustee_client/*.patch
    rm -rf *.patch

    echo "apply itrustee tzdriver patch"
    cd ${SrcPath}/itrustee_tzdriver
    git am ${patchDir}/itrustee_tzdriver/*.patch
    rm -rf *.patch

    echo "apply libboundscheck patch"
    cd ${SrcPath}/libboundscheck
    git am ${patchDir}/libboundscheck/*.patch
    rm -rf *.patch

    # recover git config
    if [[ ${cnt1} -eq 0 ]] || [[ ${cnt2} -eq 0 ]]; then
        git config --global --unset user.name
        git config --global --unset user.email
    fi
}

function get_src_code() {
    mkdir ${SrcPath}
    mkdir ${SrcPath}/demo

    cd ${SrcPath}
    # download source code
    git clone https://gitee.com/openeuler/itrustee_sdk.git
    if [[ $? -ne 0 ]]; then
        echo "itrustee_sdk download failed."
        rm -rf ${SrcPath}
        exit 1
    fi

    git clone https://gitee.com/openeuler/itrustee_client.git
    if [[ $? -ne 0 ]]; then
        echo "itrustee_client download failed."
        rm -rf ${SrcPath}
        exit 1
    fi

    git clone https://gitee.com/openeuler/itrustee_tzdriver.git
    if [[ $? -ne 0 ]]; then
        echo "itrustee_tzdriver download failed."
        rm -rf ${SrcPath}
        exit 1
    fi

    git clone https://gitee.com/openeuler/libboundscheck.git
    if [[ $? -ne 0 ]]; then
        echo "libboundscheck download failed."
        rm -rf ${SrcPath}
        exit 1
    fi

    git clone -b devkitdemo-23.0.0 https://github.com/kunpengcompute/devkitdemo.git
    if [[ $? -ne 0 ]]; then
        echo "devkitdemo download failed."
        rm -rf ${SrcPath}
        exit 1
    fi

    wget https://gitee.com/lsjhw/devkitdemo/releases/download/1.3.0/libcrypto.a --no-check-certificate
    if [[ $? -ne 0 ]]; then
        echo "libcrypto.a download failed."
        rm -rf ${SrcPath}
        exit 1
    fi

    # modify source code
    apply_patch

    # tar source code
    cd ${SrcPath}
    rm -rf libboundscheck/.git
    tar zcvf libboundscheck.tar.gz libboundscheck

    rm -rf itrustee_tzdriver/.git
    tar zcvf itrustee_tzdriver.tar.gz itrustee_tzdriver

    rm -rf itrustee_client/.git
    tar zcvf itrustee_client.tar.gz itrustee_client

    rm -rf itrustee_sdk/.git
    tar zcvf itrustee_sdk.tar.gz itrustee_sdk

    cp -r devkitdemo/Development_framework/sc-sdk/examples/* demo
    # copy libcrypto.a for demo cert-assign
    mkdir -p demo/cert-assign/lib
    cp libcrypto.a demo/cert-assign/lib
    tar zcvf demo.tar.gz demo
}

function create_workdir() {
    if [[ ${OsArch} =~ "Redhat" ]]; then
        rpmdev-setuptree
    elif [[ ${OsArch} =~ "Debian" ]]; then
        if [ -d ${DebBuild} ]; then
            echo "${DebBuild} exist, please delete it first."
            exit 1
        fi
        mkdir ${DebBuild}
        cp -r ${ScriptPath}/uos/* ${DebBuild}
    fi
}

function copy_files() {
    local distDir=("")
    if [[ ${OsArch} =~ "Redhat" ]]; then
        distDir[0]=${RpmBuild}/SOURCES

        local specDir="";
        if [[ ${OsName} =~ "openEuler" ]]; then
            specDir="${ScriptPath}/openEuler/spec"
        elif [[ ${OsName} =~ "centOS" ]]; then
            specDir="${ScriptPath}/centos/spec"
        fi
        cp ${specDir}/* ${RpmBuild}/SPECS
    elif [[ ${OsArch} =~ "Debian" ]]; then
        distDir[0]=${DebBuild}/kunpeng-sc/source
        distDir[1]=${DebBuild}/kunpeng-sc-devel/source
    fi

    for value in ${distDir[*]}
    do
        echo "copy files to ${value}"
        cp ${SrcPath}/libboundscheck.tar.gz ${value}
        cp ${SrcPath}/itrustee_tzdriver.tar.gz ${value}
        cp ${SrcPath}/itrustee_client.tar.gz ${value}
        cp ${SrcPath}/itrustee_sdk.tar.gz ${value}
        cp ${SrcPath}/demo.tar.gz ${value}
    done
}

# close auto compile python file when rpm is packaged
function close_auto_compile_python() {
    if [[ ${OsName} =~ "openEuler" ]]; then
        cd /usr/lib/rpm/openEuler
        cp macros macros.bak
        sed -i '/%{?py_auto_byte_compile:%{?__brp_python_bytecompile}}/d' macros
    elif [[ ${OsName} =~ "centOS" ]]; then
        cd /usr/lib/rpm/redhat
        cp macros macros.bak
        sed -i '/brp-python-bytecompile/d' macros
    fi
}

function recover_auto_compile_python() {
    if [[ ${OsName} =~ "openEuler" ]]; then
        rm /usr/lib/rpm/openEuler/macros
        mv /usr/lib/rpm/openEuler/macros.bak /usr/lib/rpm/openEuler/macros
    elif [[ ${OsName} =~ "centOS" ]]; then
        rm /usr/lib/rpm/redhat/macros
        mv /usr/lib/rpm/redhat/macros.bak /usr/lib/rpm/redhat/macros
    fi
}

function output() {
    if [ ! -d ${OutputPath} ]; then
        mkdir ${OutputPath}
    fi

    cd ${OutputPath}
    if [[ ${OsArch} =~ "Redhat" ]]; then
        cp ${RpmBuild}/RPMS/aarch64/kunpeng-sc-${PackageVersion}-1.aarch64.rpm ${OutputPath}
        cp ${RpmBuild}/RPMS/aarch64/kunpeng-sc-devel-${PackageVersion}-1.aarch64.rpm ${OutputPath}
    elif [[ ${OsArch} =~ "Debian" ]]; then
        cp ${DebBuild}/kunpeng-sc/script/kunpeng-sc_${PackageVersion}_arm64.deb ${OutputPath}
        cp ${DebBuild}/kunpeng-sc-devel/script/kunpeng-sc-devel_${PackageVersion}_arm64.deb ${OutputPath}
    fi
    echo "/*** The package create successfully, please go to ${OutputPath} to get it. ***/"
}

function clean_tmp_file() {
    if [ -d ${SrcPath} ]; then
        rm -rf ${SrcPath}
    fi

    if [[ ${OsArch} =~ "Redhat" ]]; then
        if [ -d ${RpmBuild} ]; then
            echo "delete ${RpmBuild}"
            rm -rf ${RpmBuild}
        fi
    elif [[ ${OsArch} =~ "Debian" ]]; then
        if [ -d ${DebBuild} ]; then
            echo "delete ${DebBuild}"
            rm -rf ${DebBuild}
        fi
    fi
}

function make_package() {
    # install the dependence software
    get_os_info
    install_dependence_software

    # download source code from gitee and modify it
    get_src_code

    # copy source code and script to workdir
    create_workdir
    copy_files

    # make package
    if [[ ${OsArch} =~ "Redhat" ]]; then
        close_auto_compile_python

        cd ${RpmBuild}/SPECS
        dos2unix kunpeng-sc.spec
        rpmbuild -ba kunpeng-sc.spec
        dos2unix kunpeng-sc-devel.spec
        rpmbuild -ba kunpeng-sc-devel.spec

        recover_auto_compile_python

    elif [[ ${OsArch} =~ "Debian" ]]; then
        # make kunpeng-sc package
        cd ${DebBuild}/kunpeng-sc/buildroot/DEBIAN
        chmod 555 preinst prerm postinst postrm

        cd ${DebBuild}/kunpeng-sc/script
        bash create-kunpeng-sc-pkg.sh

        # make kunpeng-sc-devel package
        cd ${DebBuild}/kunpeng-sc-devel/buildroot/DEBIAN
        chmod 555 preinst prerm postinst postrm

        cd ${DebBuild}/kunpeng-sc-devel/script
        bash create-kunpeng-sc-devel-pkg.sh
    fi

    output
    clean_tmp_file
}

make_package
