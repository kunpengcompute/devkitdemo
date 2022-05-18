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

time_stamp=$(date +%s)
# 安装/升级日志文件名称
LOG_FILE_NAME="install_hpc_${time_stamp}.log"
# 日志文件的绝对路径
log_file_of_abspath=${current_dir}/../log/$LOG_FILE_NAME
# 创建日志文件
mkdir -p ${current_dir}/../log
touch ${log_file_of_abspath}
# ctrl+c异常
onCtrlC(){
    logger "The Ctrl+C exception is triggered." "WARNING"
    exit 0
}

# ctrl+z异常
onCtrlZ(){
    logger "The Ctrl+Z exception is triggered." "WARNING"
    exit 0
}

# ssh断开异常
onDisconnect(){
    logger "The SSH connection was disconnected exception is triggered." "WARNING"
}

# 日志对象
logger(){
    local msg=$1
    local tip_color=$2
    local datetime=$(date "+%Y-%m-%d %H:%M:%S")
    if [[ "${tip_color}" ]] && [[ -z $(echo "${tip_color}" | sed "s/[0-9]//g") ]]; then
        echo -e "\e[1;${tip_color}m${msg}\e[0m"
        if [[ "${tip_color}" == "${TIP_COLOR_FAILED}" ]]; then
            level='ERROR'
        elif [[ "${tip_color}" == "${TIP_COLOR_WARNING}" ]]; then
            level='WARNING'
        else
            level='INFO'
        fi
    else
        if [[ "${tip_color}" == "ERROR" ]]; then
            level='ERROR'
        elif [[ "${tip_color}" == "WARNING" ]]; then
            level='WARNING'
        else
            level='INFO'
        fi
    fi
    echo "[${datetime}] [${level}] [${msg}]" >> ${log_file_of_abspath}
}

get_os_name(){
    # 获取osname
    os_name="other-os"
    os_name=$(cat /etc/os-release | grep "PRETTY_NAME" | awk -F '"' '{print $2}')
    os_name_del_space=${os_name// /}
    if [[ ${os_name_del_space} =~ "KylinLinuxAdvancedServerV10" ]]; then
        os_name="KylinLinuxAdvancedServerV10"
    elif [[ ${os_name_del_space} =~ "Ubuntu20.04" ]]; then
        os_name="Ubuntu20.04"
    elif [[ ${os_name_del_space} =~ "Ubuntu18.04" ]]; then
        os_name="Ubuntu18.04"
    elif [[ ${os_name_del_space} =~ "CentOSLinux" ]]; then
        red_os=$(cat /etc/redhat-release)
        red_os=${red_os// /}
        [[ ${red_os} =~ "CentOSLinuxrelease7.6.1810" ]] && os_name="CentOSLinuxrelease7.6.1810"
    else
        os_name=${os_name_del_space}
    fi
}

version_ge(){
    # 比较版本号 $1是否为最大版本号（包含相等）
    if [[ "$#" != 2 ]]; then
        return 1
    fi
    test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" == "$1"
    if [[ "$?" == 0 ]]; then
        return 0
    else
        return
    fi
}

user_customize_path(){
    # 用户输入自定义路径
    install_name=$1
    flag=1
    while [[ ${flag} == '1' ]]; do
        read -p "Enter the path for installing ${install_name}:" you_path
        if [[ -z ${you_path} ]]; then
            # 默认安装路径 此路径为包所在路径
            customize_path=${install_package_dir}
        else
            customize_path=${you_path}
        fi
        check_customize_path ${customize_path}
        if [[ "$?" == 0 ]]; then
            break
        fi
    done
    check_space ${customize_path}
}

get_install_kml_way(){
    # 获取安装数学库方式dpkg或者rpm
    if [[ "${os_name}" =~ "Ubuntu" ]]; then
        install_package_kind='dpkg'
    else
        install_package_kind='rpm'
    fi
}

check_customize_path(){
    # 检查用户输入的路径是否合法限定[0-9a-zA-Z-_/]
    customize_path="$1"
    len_path=${#path}
    char_max=$(($(getconf SCHAR_MAX) - 60))
    if [[ "${len_path}" -gt "${char_max}" ]]; then
        logger "The path length exceeds the limit. Enter another one." ${TIP_COLOR_FAILED}
        return 1
    fi
    if echo ${customize_path} | grep -vqE "^/"; then
        logger "The path should be an absolute path. Enter another one." ${TIP_COLOR_FAILED}
        return 1
    fi
    ar=($(echo ${customize_path} | tr '/' ' '))
    for value in ${ar[*]};
    do
        str_status=$(echo "$value" | tr -d '[a-zA-Z0-9-_.]')
        if [[ -n "$str_status" ]]; then
            logger "The entered installation path can contain digital alphabets and special characters such as ${ALL_STRING} Please enter a correct path and try again." ${TIP_COLOR_FAILED}
            return 1
        fi
    done
    if [[ -f ${customize_path} ]]; then
        logger "A file with the same name exists. Enter another one." ${TIP_COLOR_FAILED}
        return 1
    fi
    mkdir -p ${customize_path}
}

check_space(){
    # 检查磁盘大小
    customize_path="$1"
    space_flag=1
    while [[ ${space_flag} == 1 ]]; do
        if [[ -d ${customize_path} ]]; then
            avail_space=$(df -lkP ${customize_path} | awk "NR>1" | awk -F " " '{print $4}')
            break
        else
            customize_path=${customize_path%/*}
            if [[ -z ${customize_path} ]]; then
                opt_space=$(df -lkP '/' | awk "NR>1" | awk -F " " '{print $4}')
            fi
        fi
    done
    if [[ ${avail_space} -lt ${low_space} ]]; then
        logger "The disk space of the selected installation directory is insufficient." ${TIP_COLOR_FAILED}
        exit 1
    fi
}

user_choose(){
    while_flag=1
    while [[ ${while_flag} == 1 ]]; do
        num=${#soft_support_list[@]}
        # 用户选择结果进行安装
        echo -n -e "\e[1;33Enter the serial number of the software that can be installed. Format: a single digit or 'digits+commas', for example, (1,2,3). To exit the installation, enter no:\e[0m"
        read -r choose_install
        # 用户输入的字符大于最大长度则提示重新输入
        max_length_number=$((2*${num}-1))
        length_number=$(echo ${choose_install} | wc -L)
        if [[ ${length_number} -gt ${max_length_number} ]]; then
            logger "too length" ${TIP_COLOR_FAILED}
            continue
        fi
        # HPMI安装只能选择一个
        if [[ "${os_name}" =~ "KylinLinux" ]] || [[ "${os_name}" =~ "CentOSLinux" ]]; then
            if [[ ${choose_install} =~ "1" ]] && [[ ${choose_install} =~ "2" ]]; then
                logger "Only one HMPI can be installed." ${TIP_COLOR_FAILED}
                continue
            fi
        fi
        if [[ ${choose_install} =~ ^[nN]$|^no$ ]]; then
            logger "Exit installation." ${TIP_COLOR_FAILED}
            exit 1
        fi
        if echo ${choose_install} | grep -qE "([1-$num],){1,$num}[1-$num]$|^[1-$num]$"; then
            break
        else
            logger "Select one or multiple sequences and use commas to separate them." ${TIP_COLOR_FAILED}
        fi
    done
    if [[ ${length_number} -eq 1 ]]; then
        # 用户只选择一个
        # 展示用户选择要安装的
        select_result=${soft_support_list[(({choose_install}-1))]}
    else
        # 去除用户重复输入的问题
        choose_install=$(echo ${choose_install} | tr ',' ' ' | xargs -n 1 | sort -u)
        for i in ${choose_install}; do
            select_result="$select_result,${soft_support_list[(($i-1))]}"
        done
    fi
    logger "your select ${select_result#,}" ${TIP_COLOR_SUCCESS}
}

show_soft_support_list(){
    # 展示当前系统用户支持软件
    logger "Installation environment check result:" ${TIP_COLOR_CHECKING}
    if [[ ${soft_support_list} =~ "HMPI" ]]; then
        logger "The HMPI corresponds to a specific compiler. Install the matched compiler for the HMPI. For example, the compiler corresponding to HMPI-GCC is GCC." ${TIP_COLOR_ECHO}
    fi
    if [[ ${soft_support_list} =~ "KML" ]]; then
        logger "The KML corresponds to a specific compiler. Install the matched compiler for the KML. For example, the compiler corresponding to KML is GCC." ${TIP_COLOR_ECHO}
    fi 
    printf "%-16s %-29s %-14s %-20s\n" "SequenceNumber" "Software" "Support" >> ${log_file_of_abspath}
    printf "%-16s %-29s %-14s %-20s\n" "SequenceNumber" "Software" "Support"
    soft_support_list=($(echo ${soft_support_list} | tr ' ' ' '))
    for ((1=0;i<${#soft_support_list[@]};i++)); do
        printf "%-16s %-29s %-20s\n" "$((i+1))" "${soft_support_list[i]}" "${check_result_list[i]}" >> ${log_file_of_abspath}
        printf "%-16s %-29s \033[1;32m%-20s\033[0m\n" "$((i+1))" "${soft_support_list[i]}" "${check_result_list[i]}"
    done
}

read_anser(){
    # 处理用户交互相关操作
    ask_question="$*"
    ask_flag=1
    while [[ ${ask_flag} == 1 ]]; do
        read -p "${ask_question} [Y/N]:" you_choose
        if [[ ${you_choose} =~ ^[Yy]$ ]]; then
            return 1
        elif [[ ${you_choose} =~ ^[nN]$ ]]; then
            return 0
        else
            logger 'input error' ${TIP_COLOR_FAILED}
            continue
        fi
    done
}

check_precondition_mpi(){
    local list_to_be_check=(perl-Data-Dumper autoconf automake libtool numactl binutils systemd-devel valgrind)
    miss_package=()
    for ((i=0;i<${#list_to_be_check[@]};i++))
    do
        package=${list_to_be_check[$i]}
        result=$(command -v "${package}")
        if [[ -z $result ]]; then
            if [[ ${package} == "libtool" ]]; then
                miss_package[$i]="libtool-2.4.2"
            else
                miss_package[$i]=${package}
            fi
        else
            if [[ ${package} == "libtool" ]]; then
                version=$(libtool --version | head -n 1 | grep -Po "\d+\.\d+\.\d+")
                if [[ "${version}" != "2.4.2" ]]; then
                    miss_package[$i]="libtool-2.4.2"
                fi
            fi
        fi
    done
}

hand_precondition_mpi(){
    # 处理环境中依赖不足导致安装异常
    if [[ ${#miss_package[@]} -gt 0 ]]; then
        logger "The ${miss_package[@]} dependency is not detected in the environment." ${TIP_COLOR_WARNING}
        read_anser "The corresponding ${miss_package[@]} is missing or the version is incorrect. Are you sure you want to continue the installation?"
        if [[ $? == 1 ]]; then
            if [[ ${hmpi_install_status} == ${SUCCESS} ]]; then
                del_hyper_mpi
            fi
            install_hyper_mpi
        else
            logger "Exit the HMPI installation." ${TIP_COLOR_FAILED}
        fi
    else
        install_hyper_mpi
    fi
}

check_precondition_kml(){
    # 数学库相关依赖检查
    miss_package_kml=()
    libc6="/usr/lib64/libc.so.6"
    libgomp1="/usr/lib64/libgomp.so.1"
    if [[ ! -f ${libc6} ]]; then
        miss_package_kml[${#miss_package_kml[*]}]="libc6"
    fi
    if [[ ! -f ${libgomp1} ]]; then
        miss_package_kml[${#miss_package_kml[*]}]="libgomp1"
    fi
    result=$(command -v nm)
    if [[ -z ${result} ]]; then
        miss_package_kml[${#miss_package_kml[*]}]="nm"
    fi
}

hand_precondition_kml(){
    if [[ ${#miss_package_kml[@]} -gt 0 ]]; then
        logger "The ${miss_package_kml[@]} dependency is not detected in the environment." ${TIP_COLOR_WARNING}
        read_anser "The system does not have dependencies such as ${miss_package_kml[@]} or the version does not meet the requirements. Are you sure you want to continue the installing the HMPI-GCC?"
        if [[ $? == 1 ]]; then
            if [[ ${kml_install_status} == ${SUCCESS} ]]; then
                del_math_kml
            fi
            install_math_kml
        else
            logger "Exit the KML installation." ${TIP_COLOR_FAILED}
        fi
    else
        install_math_kml
    fi
}

check_install_user(){
    # 非root用户
    logger "Check the user who installs the HPC software" ${TIP_COLOR_ECHO}
    current_user=$(whoami)
    if [[ "${current_user}" != "root" ]]; then
        logger "The current user is not the root user, run the sudo bash install.sh command." ${TIP_COLOR_WARNING}
        exit 1
    fi
}

check_os_name(){
    # 判断当前系统是否支持安装
    get_os_name
    if [[ "${support_os_names}" != "${os_name}" ]]; then
        logger "The system does not support the HPC software." ${TIP_COLOR_FAILED}
        exit 1
    fi 
}

del_math_kml(){
    if [[ ${install_package_kind} == "rpm" ]]; then
        $install_package_kind -e "${boost_math_name}"
    else
        $install_package_kind -p "${boost_math_name}"
    fi
}

del_hyper_mpi(){
    # 删除原有的hmpi的环境变量
    logger 'del hmpi' ${TIP_COLOR_ECHO}
    local keys=(hwmpi ompi ucx)
    for key in ${keys[@]}
    do 
        sed -i '/'"${key}"'/d' ~/.bashrc
    done
}

del_compiler(){
    local compiler_type=$1
    if [[ ${compiler_type} == "gcc" ]]; then
        # 删除原有的gcc的环境变量
        echo 'del gcc' ${TIP_COLOR_ECHO}
        sed -i '/gcc-10.3.1-2021.09-aarch64-linux/d' /etc/profile
    else
        # 删除原有的bisheng的环境变量
        echo 'del bisheng' ${TIP_COLOR_ECHO}
        sed -i '/bisheng-compiler-2.1.1-aarch64-linux/d' /etc/profile
    fi
}