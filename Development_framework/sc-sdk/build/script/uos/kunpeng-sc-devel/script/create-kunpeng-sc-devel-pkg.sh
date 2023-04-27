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
AppName="kunpeng-sc-devel"
RootPath=$(pwd)/..
SrcPath="${RootPath}/source"
BuildPath="${RootPath}/build"
BuildRoot="${RootPath}/buildroot"
ScriptPath="${RootPath}/script"

SrcClient="${SrcPath}/itrustee_client.tar.gz"
SrcSdk="${SrcPath}/itrustee_sdk.tar.gz"
SrcTzdriver="${SrcPath}/itrustee_tzdriver.tar.gz"
SrcBoundCheck="${SrcPath}/libboundscheck.tar.gz"
SrcDemo="${SrcPath}/demo.tar.gz"

BldClient="${BuildPath}/itrustee_client"
BldSdk="${BuildPath}/itrustee_sdk"
BldTzdriver="${BuildPath}/itrustee_tzdriver"
BldBoundCheck="${BuildPath}/libboundscheck"
BldDemo="${BuildPath}/demo"

UsrInc="${BuildRoot}/usr/include"

AppHome="${BuildRoot}/usr/local/${AppName}"
AppInc="${UsrInc}/itrustee_sdk"
AppDemo="${AppHome}/examples"
AppUtil="${AppHome}/utils"
AppSrc="${AppHome}/source"

function build_client(){
    cp ${SrcClient} ${BuildPath}
    cd ${BuildPath}
    tar xf itrustee_client.tar.gz
}

function build_sdk(){
    cp ${SrcSdk} ${BuildPath}
    cd ${BuildPath}
    tar xf itrustee_sdk.tar.gz
}

function build_software(){
    cd ${BuildPath}
    cp ${SrcBoundCheck} ${BuildPath}
    tar xf libboundscheck.tar.gz

    cp ${SrcDemo} ${BuildPath}
    tar xf demo.tar.gz

    build_client
    build_sdk
}

function make_dir(){
    mkdir -p ${UsrInc}

    mkdir -p ${AppHome}
    mkdir -p ${AppInc}
    mkdir -p ${AppDemo}
    mkdir -p ${AppUtil}
    mkdir -p ${AppSrc}

    mkdir -p ${AppInc}/thirdparty
    mkdir -p ${AppDemo}/TA
    mkdir -p ${AppDemo}/CA
    mkdir -p ${AppDemo}/RSA/TA
    mkdir -p ${AppDemo}/RSA/CA 
}

function install_software(){
    make_dir

    # install utils
    cp -r ${BldSdk}/build/signtools ${AppUtil}
    cp -r ${BldSdk}/build/tools ${AppUtil}
    cp -r ${BldSdk}/build/pack-Config/ ${AppUtil}
    cp -r ${BldSdk}/build/pack-TA/ ${AppUtil}
    mkdir -p ${AppUtil}/pack-Config/xml2tlv_tools/csv
    cp -r ${BldSdk}/build/signtools/tag_parse_dict.csv ${AppUtil}/pack-Config/xml2tlv_tools/csv

    # install include
    cp -r ${BldSdk}/include/* ${AppInc}
    cp -r ${BldSdk}/thirdparty/open_source/musl/ ${AppInc}/thirdparty

    # install example
    cp -r ${BldDemo}/* ${AppDemo}
    
    # install source
    cd ${AppSrc}
    cp -r ${SrcClient} ${AppSrc}
    tar xf itrustee_client.tar.gz
    rm -rf itrustee_client.tar.gz

    cp -r ${SrcSdk} ${AppSrc}
    tar xf itrustee_sdk.tar.gz
    rm -rf itrustee_sdk.tar.gz

    cp -r ${SrcBoundCheck} ${AppSrc}
    tar xf libboundscheck.tar.gz
    rm -rf libboundscheck.tar.gz
}

function create_pkg(){
    cd ${ScriptPath}
    dpkg-deb -b ../buildroot kunpeng-sc-devel_1.3.0_arm64.deb
}

function clean(){
    rm -rf ${BuildPath}/*
    rm -rf ${BuildPathRoot}/usr
}

build_software
install_software
create_pkg
