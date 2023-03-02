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

# global value define
AppName="kunpeng-sc"
RootPath=$(pwd)/..
SrcPath="${RootPath}/source"
BuildPath="${RootPath}/build"
BuildRoot="${RootPath}/buildroot"
ScriptPath="${RootPath}/script"

SrcClient="${SrcPath}/itrustee_client.tar.gz"
SrcSdk="${SrcPath}/itrustee_sdk.tar.gz"
SrcTzdriver="${SrcPath}/itrustee_tzdriver.tar.gz"
SrcBoundCheck="${SrcPath}/libboundscheck.tar.gz"

BldClient="${BuildPath}/itrustee_client"
BldSdk="${BuildPath}/itrustee_sdk"
BldTzdriver="${BuildPath}/itrustee_tzdriver"
BldBoundCheck="${BuildPath}/libboundscheck"

UsrBin="${BuildRoot}/usr/bin"
UsrLib64="${BuildRoot}/usr/lib64"
UsrLib="${BuildRoot}/usr/lib"

AppHome="${BuildRoot}/usr/local/${AppName}"
AppDriver="${AppHome}/driver"

function build_client(){
    cp ${SrcClient} ${BuildPath}
    cd ${BuildPath}
    tar xf itrustee_client.tar.gz
    
    cp -r ${BldBoundCheck} ${BldClient}
    cd ${BldClient}
    touch build.log
    make >> build.log 2>&1
}

function build_sdk(){
    cp ${SrcSdk} ${BuildPath}
    cd ${BuildPath}
    tar xf itrustee_sdk.tar.gz

    cd ${BldSdk}
    touch build.log
    make >> build.log 2>&1
}

function build_software(){
    cd ${BuildPath}
    cp ${SrcBoundCheck} ${BuildPath}
    tar xf libboundscheck.tar.gz

    build_client
    build_sdk
}

function make_dir(){
    mkdir -p ${UsrBin}
    mkdir -p ${UsrLib}
    mkdir -p ${UsrLib64}

    mkdir -p ${AppHome}
    mkdir -p ${AppDriver}

}

function install_software(){
    make_dir
    
    # install lib
    cp    ${BldClient}/dist/*.so.1 ${UsrLib64}
    cp -d ${BldClient}/dist/*.so   ${UsrLib64}
    cp    ${BldClient}/dist/*.so.1 ${UsrLib}
    cp -d ${BldClient}/dist/*.so   ${UsrLib}
    cp    ${BldSdk}/libteec_adaptor.so.1 ${UsrLib64}
    cp -d ${BldSdk}/libteec_adaptor.so   ${UsrLib64}
    cp    ${BldSdk}/libteec_adaptor.so.1 ${UsrLib}
    cp -d ${BldSdk}/libteec_adaptor.so   ${UsrLib}
    
    cp ${BldClient}/dist/teecd ${UsrBin}
    cp ${BldClient}/dist/tlogcat ${UsrBin}

    # install driver
    cd ${AppDriver}
    cp ${SrcTzdriver} ${AppDriver}
    tar xf itrustee_tzdriver.tar.gz
    cp -r ${BldBoundCheck} ${AppDriver}/itrustee_tzdriver
    rm -rf itrustee_tzdriver.tar.gz
}

function create_pkg(){
    cd ${ScriptPath}
    dpkg-deb -b ../buildroot kunpeng-sc_1.3.0_arm64.deb
}

function clean(){
    rm -rf ${BuildPath}/*
    rm -rf ${BuildPathRoot}/usr
}

build_software
install_software
create_pkg
