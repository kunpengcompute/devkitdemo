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
current_dir=$(
  cd $(dirname $0)
  pwd
)
WORKSPACE=${current_dir}/output
opensource_dir=${current_dir}/opensource
[[ -d "${WORKSPACE}" ]] && { rm -rf ${WORKSPACE};mkdir -p ${WORKSPACE}; } || mkdir -p ${WORKSPACE}
[[ -d "${opensource_dir}" ]] && { rm -rf ${opensource_dir};mkdir -p ${opensource_dir}; } || mkdir -p ${opensource_dir}
# Read the configuration file to get the LAPACK package address
lib_klapack_name=$(sed '/^lib_klapack_name=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
# Read the configuration file to obtain the package information to be downloaded
all_need_download_softwares=$(sed '/^all_need_download_softwares=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
hpc_kunpeng_package_name=$(sed '/^hpc_kunpeng_package_name=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)

check_dos2unix(){
    dos2unix -V >/dev/null 2>&1
    if [[  "$?" -ne 0 ]];then
        yum install -y dos2unix
        if [[  "$?" -ne 0 ]];then
            apt install dos2unix -y
        fi
    fi
}

compiler_libklapack(){
    download_package "${opensource_dir}" "${lib_klapack_name}"
    netlib=${opensource_dir}/v3.9.1.tar.gz
    klapack=/usr/local/kml/lib/libklapack.a
    cd ${opensource_dir}
    mkdir lapack_adapt
    cd lapack_adapt

    # build netlib lapack
    mkdir netlib
    cd netlib
    tar -xf ${netlib}
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON ../lapack-3.9.1
    make -j $(lscpu | grep "^CPU(s):" | awk '{print $2}')
    cd ../..

    cp netlib/build/lib/libblas.a ${WORKSPACE}/${hpc_kunpeng_package_name}/package/kml
    cp netlib/build/lib/liblapack.a  ${WORKSPACE}/${hpc_kunpeng_package_name}/package/kml/liblapack_adapt.a
}

download_package()
{
    local package_dir="$1"
    local url="$2"
    mkdir -p "${package_dir}"
    cd "${package_dir}"
    wget "${url}" -t 10 --no-check-certificate
    if [[ "$?" -ne 0 ]];then
        echo "Download ${url} Failed"
        exit 1
    fi
}

rm_path()
{
    local directory="$1"
    [[ -d "${directory}" ]] && rm -rf "${directory}"
}

rm_file()
{
    local filename="$1"
    [[ -f "${filename}" ]] && rm -rf "${filename}"
}

check_dos2unix
cd ${WORKSPACE}
rm_path "${WORKSPACE}/${hpc_kunpeng_package_name}"
rm_file "${WORKSPACE}/${hpc_kunpeng_package_name}.tar.gz"
rm_file "${hpc_kunpeng_package_name}.sha256sum"

split_software_info(){
    local software_name="$1"
    software_infos=$(sed "/^${software_name}=/!d;s/.*=//;s/\r//" ${current_dir}/const.conf)
    software_infos=(${software_infos//;/ })
    package=${software_infos[0]}
    url=${software_infos[1]}
}

download_software_common_func()
{
    for software in $(echo ${all_need_download_softwares//;/ })
    do
        split_software_info "${software}"
        {
            download_package "${WORKSPACE}/${hpc_kunpeng_package_name}/package/${package}" "${url}"
        }&
    done
    wait
}

clean()
{
    # clean
    rm_path "${hpc_kunpeng_package_name}"
    rm_path "${opensource_dir}"
}

packing()
{
    cd ${WORKSPACE}/${hpc_kunpeng_package_name}/package/kml && unzip *.zip && rm_file *.zip
    cp -rf ${current_dir}/../script ${WORKSPACE}/${hpc_kunpeng_package_name}
    cp -rf ${current_dir}/../module ${WORKSPACE}/${hpc_kunpeng_package_name}
    mv ${WORKSPACE}/${hpc_kunpeng_package_name}/script/README*.md ${WORKSPACE}/${hpc_kunpeng_package_name}
    # modifying per,ossions
    find ${WORKSPACE}/${hpc_kunpeng_package_name} -type d | xargs chmod 755
    find ${WORKSPACE}/${hpc_kunpeng_package_name}/package -type f | xargs chmod 644
    chmod 644 ${WORKSPACE}/${hpc_kunpeng_package_name}/README*.md 
    chmod 644 ${WORKSPACE}/${hpc_kunpeng_package_name}/module/* 
    chmod 544 ${WORKSPACE}/${hpc_kunpeng_package_name}/script/* 
    cd ${WORKSPACE}
    find ${WORKSPACE}/${hpc_kunpeng_package_name} -type f | xargs dos2unix 
    tar -cvf ${hpc_kunpeng_package_name}.tar.gz ${hpc_kunpeng_package_name}
    sha256sum ${hpc_kunpeng_package_name}.tar.gz > ${hpc_kunpeng_package_name}.sha256sum
}

download_software_common_func

compiler_libklapack

packing

clean
