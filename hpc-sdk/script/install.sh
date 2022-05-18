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

current_dir=$(cd $(dirname $); pwd)
source ${current_dir}/const.sh
source ${current_dir}/common_fun.sh
install_package_dir=${current_dir}/../package
module_file_dir=${current_dir}/../module
# 捕获ctrl+c、终端断开链接异常
trap 'onCtrlC' INT
trap 'onCtrlZ' SIGTSTP
trap 'onDisconnect' HUP

set_soft_status(){
    # 根据os获取支持安装软件的状态
    check_result_list=()
    suggestion_list=()
    soft_support_list=${soft_support_lists["$os_name"]}
    for soft_support in $(echo $soft_support_list | tr ' ' ' '); do
        if [[ ${soft_support} =~ ^HMPI_GCC$|^HMPI_BISHENG$|^BISHENG$|^GCC$|^KML$ ]]; then
            check_result_list[${#check_result_list[*]}]='Y'
        fi
    done
}

get_hmpi_package_name(){
    # 获取hmpi包名
    if [[ "${os_name}" =~ "KylinLinux" ]]; then
        if [[ ${select_result} =~ "HMPI_GCC" ]]; then
            hmpi_package_name=${kylinv10_hmpi_gcc_name}
        elif [[ ${select_result} =~ "HMPI_BISHENG" ]]; then
            hmpi_package_name=${kylinv10_hmpi_bisheng_name}
        fi
    elif [[ "${os_name}" =~ "CentOSLinux" ]]; then
        if [[ ${select_result} =~ "HMPI_GCC" ]]; then
            hmpi_package_name=${centos7_6_linux_hmpi_gcc_name}
        elif [[ ${select_result} =~ "HMPI_BISHENG" ]]; then
            hmpi_package_name=${centos7_6_hmpi_gcc_name}
        fi
    fi
}

check_soft_installed(){
    # 检查软件已经安装了
    # 避免重复安装时找不到环境变量此处执行 source /etc/profile
    case $1 in
        "HMPI_GCC")
        which mpirun >/dev/null 2>&1
        if [[ $? == 0 ]]; then
            hmpi_gcc_install_status=${SUCCESS}
        else
            hmpi_gcc_install_status=${FAILED}
            if grep -q "ompi/lib" ~/.bashrc && grep -q "/ucx/lib" ~/.bashrc; then
                hmpi_gcc_install_status=${SUCCESS}
            fi
        fi
        ;;
        "HMPI_BISHENG")
        which mpirun >/dev/null 2>&1
        if [[ $? == 0 ]]; then
            hmpi_bisheng_install_status=${SUCCESS}
        else
            hmpi_bisheng_install_status=${FAILED}
            if grep -q "ompi/lib" ~/.bashrc && grep -q "/ucx/lib" ~/.bashrc; then
                hmpi_bisheng_install_status=${SUCCESS}
            fi
        fi
        ;;
        "GCC")
        if gcc -v 2>&1 | grep -q "Kunpeng gcc 10.3.1-2.0.0.b020"; then
            gcc_install_status=${SUCCESS}
        else
            gcc_install_status=${FAILED}
            if grep -q "gcc-10.3.1-2021.09-aarch64-linux" /etc/profile; then
                gcc_install_status=${SUCCESS}
            fi
        fi
        ;;
        "BISHENG")
        # clang -v 标准错误输出 2>&1
        if clang -v 2>&1 | grep -q "HUAWEI BiSheng Compiler 2.1.0.B010 clang version 12.0.0"; then
            bisheng_install_status=${SUCCESS}
        else
            bisheng_install_status=${FAILED}
            if grep -q "bisheng-compiler-2.1.1-aarch64-linux" /etc/profile; then
                bisheng_install_status=${SUCCESS}
            fi
        fi
        ;;
        "KML")
        [[ ${install_package_kind} == "rpm" ]] && result=$($install_package_kind -qa boostkit-kml) || result=$($install_package_kind -l boostkit-kml)
        [[ ${result} ]] && kml_install_status=${SUCCESS} || kml_install_status=${FAILED}
        ;;
    esac
}

check_system_gcc_status(){
    # 检查系统中gcc版本是否支持
    install_reply=$1
    [[ ${install_reply} == "kml" ]] && gcc_support_version='7.3' || gcc_support_version='9.3'
    gcc_version=$(gcc --version 2>&1 | head -n 1 | grep -Po "\d+\.\d+ | head -n 1")
    version_ge ${gcc_version} ${gcc_support_version}
    # 版本符合
    if [[ "$?" == 0 ]]; then
        [[ ${install_reply} == "kml" ]] && gcc_kml_check_status=${SUCCESS} || gcc_hmpi_check_status=${SUCCESS}
    else
        [[ ${install_reply} == "kml" ]] && gcc_kml_check_status=${FAILED} || gcc_hmpi_check_status=${FAILED}
    fi
}

set_soft_choose_status(){
    # 设置用户选择软件的状态
    [[ "${select_result}" =~ ^GCC*$|,GCC* ]] && gcc_choose_status=${SUCCESS} || gcc_choose_status=${FAILED}
    [[ "${select_result}" =~ ^BISHENG*$|,BISHENG* ]] && bisheng_choose_status=${SUCCESS} || bisheng_choose_status=${FAILED}
    [[ "${select_result}" =~ "HMPI_GCC" ]] && hmpi_gcc_choose_status=${SUCCESS} || hmpi_gcc_choose_status=${FAILED}
    [[ "${select_result}" =~ "HMPI_BISHENG" ]] && hmpi_bisheng_choose_status=${SUCCESS} || hmpi_bisheng_choose_status=${FAILED}
    [[ "${select_result}" =~ "KML" ]] && kml_choose_status=${SUCCESS} || kml_choose_status=${FAILED}
}

install_hmpi_env_check(){
    # hyper-mpi安装环境检查
    hmpi_type=$1
    [[ ${hmpi_type} == "gcc" ]] && hmpi_choose_status=${hmpi_gcc_choose_status} || hmpi_choose_status=${hmpi_bisheng_choose_status}
    [[ ${hmpi_type} == "gcc" ]] && hmpi_install_status=${hmpi_gcc_install_status} || hmpi_install_status=${hmpi_bisheng_install_status}
    [[ ${hmpi_type} == "gcc" ]] && compiler_choose_status=${gcc_choose_status} || compiler_choose_status=${bisheng_choose_status}
    [[ ${hmpi_type} == "gcc" ]] && compiler_hmpi_check_status=${gcc_hmpi_check_status} || compiler_hmpi_check_status=${bisheng_check_status}
    if [[ ${hmpi_choose_status} == ${SUCCESS} ]]; then
        logger "Start installing the HMPI-${hmpi_type}." ${TIP_COLOR_CHECKING}
        unset miss_package
        check_precondition_mpi
        if [[ ! -f ${install_package_dir}/hyper_mpi/"${hmpi_package_name}.tar.gz" ]]; then
            logger "The installation package cannot be found." ${TIP_COLOR_SUCCESS}
            return 1
        fi
    fi
    if [[ ${hmpi_gcc_choose_status} == ${SUCCESS} ]] && [[ ${compiler_choose_status} == ${SUCCESS} ]]; then 
        if [[ ${hmpi_gcc_install_status} == ${SUCCESS} ]]; then
            logger "HMPI-${hmpi_type} is installed in this system" ${TIP_COLOR_WARNING}
            read_anwser "The HMPI-${hmpi_type} already exists. Are you sure you want to continue installing the HPMI-GCC?"
            if [[ $? == 1 ]]; then
                # 此时可认为gcc 符合安装要求
                hand_precondition_mpi
            else
                logger "Do not install the HMPI repeatedly." ${TIP_COLOR_ECHO}
            fi
        else
            # 符合要求可以直接安装
            hand_precondition_mpi
        fi
    elif [[ ${hmpi_gcc_choose_status} == ${SUCCESS} ]] && [[ ${gcc_choose_status} == ${FAILED} ]]; then
        if [[ ${hmpi_gcc_install_status} == ${SUCCESS} ]]; then
            logger "Hmpi_${hmpi_type} is installed in this system" ${TIP_COLOR_WARNING}
            read_anwser "The Hmpi-${hmpi_type} already exists. Are you sure you want to continue installing the HPMI-GCC?"
            if [[ $? == 1 ]]; then
                if [[ ${compiler_hmpi_check_status} == ${SUCCESS} ]]; then
                    hand_precondition_mpi
                else
                    miss_package[${#miss_package[*]}]=${hmpi_type}
                    hand_precondition_mpi
                fi
            else
                logger "Do not install the HMPI repeatedly." ${TIP_COLOR_ECHO}
            fi
        else
            if [[ ${compiler_hmpi_check_status} == ${SUCCESS} ]]; then
                # 符合要求可以直接安装
                hand_precondition_mpi
            else
                miss_package[${#miss_package[*]}]=${hmpi_type}
                hand_precondition_mpi
            fi
        fi
    fi
}

install_kml_env_check(){
    # 数学库检查
    if [[ ${kml_choose_status} == ${SUCCESS} ]]; then
        logger "Start installing the KML." ${TIP_COLOR_CHECKING}
        check_precondition_kml
        if [[ ${install_package_kind} == "rpm" ]] && [[ ! -f ${install_package_dir}/boostkit_kml/${boost_math_rpm_name}.rpm ]]; then
            logger "The installation package cannot be found." ${TIP_COLOR_SUCCESS}
            return 1
        elif [[ ${install_package_kind} == "dpkg" ]] && [[ ! -f ${install_package_dir}/boostkit_kml/${boost_math_deb_name}.deb ]]; then
            logger "The installation package cannot be found." ${TIP_COLOR_SUCCESS}
            return 1
        fi
    fi
    if [[ ${kml_choose_status} == ${SUCCESS} ]] && [[ ${gcc_choose_status} == ${SUCCESS} ]]; then
        if [[ ${kml_install_status} == ${SUCCESS} ]]; then
            read_anwser "The KML already exists. Are you sure you want to continue installing the KML?"
            if [[ $? == 1 ]]; then
               hand_precondition_kml
            else
                logger "Do not install the KML repeatedly." ${TIP_COLOR_FAILED}
            fi
        else
            hand_precondition_kml
        fi
    elif [[ ${kml_choose_status} == ${SUCCESS} ]] && [[ ${gcc_choose_status} == ${FAILED} ]]; then
        if [[ ${kml_install_status} == ${SUCCESS} ]]; then
            logger "The KML already exists. Are you sure you want to continue installing the KML?"
            read_anwser "The KML already exists. Are you sure you want to continue installing the KML?"
            if [[ $? == 1 ]]; then
                logger "The KML already exists. Are you sure you want to continue installing the KML? Y"
                if [[ ${gcc_kml_check_status} == ${SUCCESS} ]]; then
                    hand_precondition_kml
                else 
                    miss_package_kml[${#miss_package_kml[*]}]='gcc'
                    hand_precondition_kml
                fi
            else
                logger "The KML already exists. Are you sure you want to continue installing the KML? N"
                logger "Do not install the KML repeatedly." ${TIP_COLOR_FAILED}
            fi
        else
            if [[ ${gcc_kml_check_status} == ${SUCCESS} ]]; then
                hand_precondition_kml
            else
                miss_package_kml[${#miss_package_kml[*]}]='gcc'
                hand_precondition_kml
            fi
        fi
    fi
}

install_compiler_env_check(){
    # 安装前检查gcc
    local compiler_type=$1
    [[ ${compiler_type} == "gcc" ]] && compiler_choose_status=${gcc_choose_status} || compiler_choose_status=${bisheng_choose_status}
    [[ ${compiler_type} == "gcc" ]] && compiler_install_status=${gcc_install_status} || compiler_install_status=${bisheng_install_status}
    [[ ${compiler_type} == "gcc" ]] && compiler_name=${gcc_compiler_name} || compiler_name=${bisheng_compiler_name}
    if [[ ${compiler_choose_status} == ${SUCCESS} ]]; then
        logger "Start installing the ${compiler_type}." ${TIP_COLOR_CHECKING}
        if [[ ! -f ${install_package_dir}/${compiler_type}/"${compiler_name}.tar.gz]"]; then
            logger "The installation package cannot be found." ${TIP_COLOR_SUCCESS}
        fi
        # 判断gcc是否有安装
        if [[ ${compiler_install_status} == ${SUCCESS} ]]; then
            logger "The ${compiler_type^^} already exists. Are you sure you want to continue installing the ${compiler_type^^}?"
            read_anwser "The ${compiler_type^^} already exists. Are you sure you want to continue installing the ${compiler_type^^}?"
            if [[ $? == 1 ]]; then
                [[ ${compiler_type} == "gcc" ]] && del_compiler "gcc" || del_compiler "bisheng"
                [[ ${compiler_type} == "gcc" ]] && install_compiler "gcc" || install_compiler "bisheng"
                # 设置gcc检查结果是否支持安装
                [[ ${compiler_type} == "gcc" ]] && gcc_kml_check_status=${SUCCESS};gcc_hmpi_check_status=${SUCCESS}
            else
                logger "Do not install the GCC repeatedly." ${TIP_COLOR_FAILED}
                if [[ ${compiler_type} == "gcc" ]]; then
                    check_system_gcc_status "kml"
                    check_system_gcc_status "hmpi"
                fi
            fi
        else
            [[ ${compiler_type} == "gcc" ]] && install_compiler "gcc" || install_compiler "bisheng"
            [[ ${compiler_type} == "gcc" ]] && gcc_kml_check_status=${SUCCESS};gcc_hmpi_check_status=${SUCCESS} || bisheng_check_status=${SUCCESS}
        fi
    else
        if [[ ${compiler_type} == "gcc" ]]; then
            # 检查系统中gcc 是否符合要求，不符合添加标记
            check_system_gcc_status "kml"
            check_system_gcc_status "hmpi"
        else
            bisheng_check_status=${FAILED}
        fi
    fi
}


install_hyper_mpi(){
    # 安装hyper_mpi
    user_customize_path 'hyper-mpi'
    logger "You choose to install Hyper MPI in the $(cd ${customize_path};pwd) directory." ${TIP_COLOR_SUCCESS}
    cd ${install_package_dir}/hyper_mpi
    [[ ${you_path} ]] && install_hmpi_path=${customize_path} || install_hmpi_path=${install_package_dir}/hyper_mpi
    tar -xf ${hmpi_package_name}.tar.gz -C ${install_hmpi_path}
    cd ${install_hmpi_path}/$hmpi_package_name
    # 执行安装脚本安装hmpi
    bash install.sh
    if [[ "$?" == 0 ]]; then
        logger "Hyper MPI is installed."
        if [[ ${hmpi_gcc_choose_status} == 1 ]]; then
            change_modules "HMPI_GCC"
            logger "To make the ralated commands take effect, run the following command: source ~/.bashrc" ${TIP_COLOR_SUCCESS}
            logger "The HMPI-GCC is installed." ${TIP_COLOR_SUCCESS}
         elif [[ ${hmpi_bisheng_choose_status} == 1 ]]; then
            change_modules "HMPI_BISHENG"
            logger "To make the ralated commands take effect, run the following command: source ~/.bashrc" ${TIP_COLOR_SUCCESS}
            logger "The HMPI-BISHENG is installed." ${TIP_COLOR_SUCCESS}
        fi
    fi
}

install_compiler(){
    local compiler_type=$1
    user_customize_path "${compiler_type} compiler"
    logger "you choose install ${compiler_type} path is $(cd ${customize_path};pwd)" ${TIP_COLOR_SUCCESS}
    cd ${install_package_dir}/${compiler_type}
    install_compiler_path=${customize_path}
    [[ -z ${you_path} ]] && install_compiler_path=$(cd ${install_package_dir}/${compiler_type};pwd)
    if [[ ${compiler_type} == "gcc" ]]; then
        tar -xf ${gcc_compiler_name}.tar.gz -C ${install_compiler_path}
        cd ${install_compiler_path}/${gcc_compiler_name}
        echo 'export PATH='${install_compiler_path}/${gcc_compiler_name}'/bin:$PATH' >>/etc/profile
        echo 'export INCLUDE='${install_compiler_path}/${gcc_compiler_name}'/INCLUDE:$INCLUDE' >>/etc/profile
        echo 'export LD_LIBRAY_PATH='${install_compiler_path}/${gcc_compiler_name}'/lib64:$LD_LIBRAY_PATH' >>/etc/profile
    fi
    if [[ ${compiler_type} == "bisheng" ]]; then
        tar -xf ${bisheng_compiler_name}.tar.gz -C ${install_compiler_path}
        cd ${install_compiler_path}/${bisheng_compiler_name}
        echo 'export PATH='${install_compiler_path}/${bisheng_compiler_name}'/bin:$PATH' >>/etc/profile
        echo 'export LD_LIBRAY_PATH='${install_compiler_path}/${bisheng_compiler_name}'/lib:$LD_LIBRAY_PATH' >>/etc/profile
    fi 
    # 默认安装把路径写入/etc/profile中
    if [[ "$?" == 0 ]]; then
        logger "Failed to install the ${compiler_type}." ${TIP_COLOR_FAILED}
    fi
    if [[ ${compiler_type} == "bisheng" ]]; then
        change_modules "BISHENG"
        if clang -v 2>&1 | grep -q "HUAWEI BiSheng Compiler 2.1.0.B010 clang version 12.0.0"; then
            logger "install ${compiler_type} success"
        fi
    else
        change_modules "GCC"
        if gcc --version | grep -q "Kunpeng gcc 10.3.1-2.0.0.b020"; then
            logger "The ${compiler_type} Compiler is installed."
        fi
    fi
    logger "To make the ralated commands take effect, run the following command: source /etc/profile" ${TIP_COLOR_SUCCESS}
    logger "The ${compiler_type} Compiler is installed." ${TIP_COLOR_SUCCESS}
}

install_math_kml(){
    # 安装数学库
    check_space /usr/local
    if [[ ${install_package_kind} == "rpm" ]]; then
        $install_package_kind -ivh ${install_package_dir}/boostkit_kml/${boost_math_rpm_name}.rpm
    else
        $install_package_kind -i ${install_package_dir}/boostkit_kml/${boost_math_deb_name}.deb
    fi
    source /etc/profile
    nm_math_kml
    logger "To make the ralated commands take effect, run the following command: source /etc/profile" ${TIP_COLOR_SUCCESS}
    logger "The math library is installed." ${TIP_COLOR_SUCCESS}
}

nm_math_kml(){
    # 添加数学符号
    cp -rf ${install_package_dir}/boostkit_kml/liblapack_adapt.a /usr/local/kml/lib     # /usr/local/kml/lib为数学库安装好地址
    cp -rf ${install_package_dir}/boostkit_kml/libblas.a /usr/local/kml/lib     # /usr/local/kml/lib为数学库安装好地址
    cd /usr/local/kml/lib
    klapack=/usr/local/kml/lib/libklapack.a
    # get symblos defind both in klapack and netlib lapack
    nm -g liblapack_adapt.a | grep 'T ' | grep -oP '\K\w+(?=_$)' | sort |uniq > netlib.sym
    nm -g $klapack | grep 'T ' | grep -oP '\K\w+(?=_$)' | sort |uniq > klapack.sym
    comm -12 klapack.sym netlib.sym > comm.sym

    # update symbols name of liblapack_adapt.a
    which read sym; do
        if ! nm liblapack_adapt.a | grep -qe " T ${sym}_\$"; then
            continue
        fi
        ar x liblapack_adapt.a $sym.f.o
        mv $sym.f.o ${sym}_netlib.f.o
        objcopy --redefine-sym ${sym}_=${sym}_netlib_ ${sym}_netlib.f.o;
        ar d liblapack_adapt.a ${sym}.f.o
        ar ru liblapack_adapt.a ${sym}_netlib.f.o
        rm ${sym}_netlib.f.o
    done < comm.sym
}

change_modules(){
    # 修改
    install_soft=$1
    case $install_soft in
        "HMPI_GCC")
            logger "Set moudles for HMPI-GCC." ${TIP_COLOR_ECHO}
            
            # 要添加用户自定义目录下有同名modules的情况
            old_msg=$(sed -n '/set prefix/p' ${module_file_dir}/hmpi_modulefiles)
            if [[ "${os_name}" =~ "KylinLinux" ]]; then
                sed -i s"#${old_msg}#set prefix $(cd ${install_hmpi_path};pwd)/${kylinv10_hmpi_gcc_name}#" ${module_file_dir}/hmpi_modulefiles
            elif [[ "${os_name}" =~ "CentOSLinux" ]]; then
                sed -i s"#${old_msg}#set prefix $(cd ${install_hmpi_path};pwd)/${centos7_6_linux_hmpi_gcc_name}#" ${module_file_dir}/hmpi_modulefiles  
            fi
            [[ ${you_path} ]] && cp -rf ${module_file_dir}/hmpi_modulefiles ${customize_path}
            [[ ${you_path} ]] && show_modulefile_path=${customize_path} || show_modulefile_path=$(cd ${customize_path}/../module;pwd)
            logger "The path of HMPI-GCC modules is ${show_modulefile_path}." ${TIP_COLOR_ECHO}
        ;;
        "HMPI_BISHENG")
            logger "Set moudles for HMPI-BISHENG." ${TIP_COLOR_ECHO}
            old_msg=$(sed -n '/set prefix/p' ${module_file_dir}/hmpi_modulefiles)
            if [[ "${os_name}" =~ "KylinLinux" ]]; then
                sed -i s"#${old_msg}#set prefix $(cd ${install_hmpi_path};pwd)/${kylinv10_hmpi_bisheng_name}#" ${module_file_dir}/hmpi_modulefiles
            elif [[ "${os_name}" =~ "CentOSLinux" ]]; then
                sed -i s"#${old_msg}#set prefix $(cd ${install_hmpi_path};pwd)/${centos7_6_linux_hmpi_bisheng_name}#" ${module_file_dir}/hmpi_modulefiles  
            fi
            [[ ${you_path} ]] && cp -rf ${module_file_dir}/hmpi_modulefiles ${customize_path}
            [[ ${you_path} ]] && show_modulefile_path=${customize_path} || show_modulefile_path=$(cd ${customize_path}/../module;pwd)
            logger "The path of HMPI-BISHENG modules is ${show_modulefile_path}." ${TIP_COLOR_ECHO}
        ;;
        "BISHENG")
            logger "Set moudles for BISHENG." ${TIP_COLOR_ECHO}
            old_msg=$(sed -n '/set prefix/p' ${module_file_dir}/bisheng_modulefiles)
            sed -i s"#${old_msg}#set prefix $(cd ${install_compiler_path};pwd)/${bisheng_compiler_name}#" ${module_file_dir}/bisheng_modulefiles
            [[ ${you_path} ]] && cp -rf ${module_file_dir}/bisheng_modulefiles ${customize_path}
            [[ ${you_path} ]] && show_modulefile_path=${customize_path} || show_modulefile_path=$(cd ${customize_path}/../module;pwd)
            logger "The path of BISHENG modules is ${show_modulefile_path}." ${TIP_COLOR_ECHO}
        ;;
        "GCC")
            logger "Set moudles for GCC." ${TIP_COLOR_ECHO}
            old_msg=$(sed -n '/set prefix/p' ${module_file_dir}/gcc_modulefiles)
            sed -i s"#${old_msg}#set prefix $(cd ${install_compiler_path};pwd)/${gcc_compiler_name}#" ${module_file_dir}/bisheng_modulefiles
            [[ ${you_path} ]] && cp -rf ${module_file_dir}/gcc_modulefiles ${customize_path}
            [[ ${you_path} ]] && show_modulefile_path=${customize_path} || show_modulefile_path=$(cd ${customize_path}/../module;pwd)
            logger "The path of GCC modules is ${show_modulefile_path}." ${TIP_COLOR_ECHO}
        ;;
    esac
}

install_main(){
    # 安装脚本入口
    logger "Start installing the HPC software." ${TIP_COLOR_ECHO}
    check_install_user
    check_os_name
    get_install_kml_way
    set_soft_status
    show_soft_support_list
    user_choose
    get_hmpi_package_name
    for result in $(echo ${select_result} | tr ',' ' '); do
        check_soft_installed $result
    done
    set_soft_choose_status
    install_compiler_env_check "gcc"
    install_compiler_env_check "bisheng"
    install_hmpi_env_check "gcc"
    install_hmpi_env_check "bisheng"
    install_kml_env_check
    exit 0   
}

install_main