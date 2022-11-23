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

# ctrl+c
onCtrlC() {
  logger "The Ctrl+C exception is triggered." {TIP_COLOR_FAILED}
  exit 0
}

# ctrl+z
onCtrlZ() {
  logger "The Ctrl+Z exception is triggered." {TIP_COLOR_FAILED}
  exit 0
}

# SSH disconnection exception
onDisconnect() {
  logger "The SSH connection was disconnected exception is triggered." {TIP_COLOR_FAILED}
  exit 0
}

create_log_file() {
  time_stamp=$(date +%s)
  # Installation log file name
  LOG_FILE_NAME="install_hpc_${time_stamp}.log"
  # Absolute path of log files
  log_file_of_abspath=${current_dir}/../log/$LOG_FILE_NAME
  # Create a log file
  mkdir -p ${current_dir}/../log
  touch ${log_file_of_abspath}
}

logger() {
  local msg="$1"
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
  if [ -f "${log_file_of_abspath}" ]; then
    echo "[${datetime}] [${level}] ${msg}" >>${log_file_of_abspath}
  fi
}

get_os_name() {
  # Obtain the os name
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

version_ge() {
  # Compare the version
  if [[ "$#" != 2 ]]; then
    return 1
  fi
  test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" == "$1"
  if [[ "$?" == 0 ]]; then
    return 0
  else
    return 1
  fi
}

user_customize_path() {
  # Enter a user-defined path.
  install_name=$1
  flag=1
  while [[ ${flag} == '1' ]]; do
    logger "For a non-root user, enter the directory on which the user has permission." ${TIP_COLOR_WARNING}
    read -p "Enter the path for installing ${install_name}(default /opt):" you_path
    if [[ -z "${you_path}" ]]; then
      # Default installation path
      customize_path="/opt"
    else
      customize_path="${you_path}"
    fi
    check_customize_path "${customize_path}"
    if [[ "$?" == 0 ]]; then
      break
    fi
  done
  check_space ${customize_path}
}

get_install_kml_mode() {
  # Obtain the dpkg or rpm installation mode of the math library
  if [[ "${os_name}" =~ "Ubuntu" ]]; then
    install_package_kind='dpkg'
  else
    install_package_kind='rpm'
  fi
}

check_customize_path() {
  # Check whether the entered path is valid [0-9a-zA-Z-_/]
  customize_path="$1"
  len_path=${#customize_path}
  char_max=$(($(getconf SCHAR_MAX) - 59))
  unset new_customize_path
  if [[ "${len_path}" -ge "${char_max}" ]]; then
    logger "The path length exceeds the limit. Enter another one." ${TIP_COLOR_FAILED}
    return 1
  fi
  if echo "${customize_path}" | grep -vqE "^/"; then
    logger "The path should be an absolute path. Enter another one." ${TIP_COLOR_FAILED}
    return 1
  fi
  ar=($(echo "${customize_path}" | tr '/' ' '))
  for value in ${ar[*]}; do
    str_status=$(echo "$value" | tr -d "[a-zA-Z0-9-_]")
    if [[ -n "$str_status" ]]; then
      logger "The entered installation path can contain digital alphabets and special characters such as ${ALLOW_STRING} Please enter a correct path and try again." ${TIP_COLOR_FAILED}
      return 1
    else
      new_customize_path=${new_customize_path}/${value}
    fi
  done
  if [ -f ${customize_path} ]; then
    logger "A file with the same name exists. Enter another one." ${TIP_COLOR_FAILED}
    return 1
  fi
  customize_path=${new_customize_path}
  [[ -z ${customize_path} ]] && customize_path="/"
  mark_customize_path_status
  mkdir -p ${customize_path}
  [[ ${customize_path} == "/" ]] || customize_path=$(
    cd ${customize_path}
    pwd
  )/
}

mark_customize_path_status() {
  if [[ -d ${customize_path} ]]; then
    customize_path_status=1
  else
    valid_customize_path=${customize_path}
    while [[ ${flag} == 1 ]]; do
      valid_customize_path=${valid_customize_path%/*}
      if [ -d ${valid_customize_path} ]; then
        customize_path_status=2
        break
      fi
      if [ -z ${valid_customize_path} ]; then
        break
      fi
    done
  fi
}

check_space() {
  # Check the drive space
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
        break
      fi
    fi
  done
  if [[ ${avail_space} -lt ${low_space} ]]; then
    logger "The disk space of the selected installation directory is insufficient." ${TIP_COLOR_FAILED}
    logger "Ensure that the available space of the ${customize_path} directory is greater than ${low_space_package_g} GB" ${TIP_COLOR_FAILED}
    exit 1
  fi
}

user_choose() {
  while_flag=1
  while [[ ${while_flag} == 1 ]]; do
    num=${#software_support_list[@]}
    # Install the software according to the sequence selected by the user.
    echo -n -e "\e[1;33mEnter the serial number of the software that can be installed. Format: a single digit or 'digits+commas', for example, (1,2,3). To exit the installation, enter no:\e[0m"
    read -r choose_install
    # If the length of the entered character string exceeds the upper limit, the system prompts you to enter a new character string.
    max_length_number=$((2 * ${num} - 1))
    length_number=$(echo ${choose_install} | wc -L)
    if [[ ${length_number} -gt ${max_length_number} ]]; then
      logger "The character string you entered is too long." ${TIP_COLOR_FAILED}
      continue
    fi
    # Only one Hyper MPI can be installed.
    if [[ "${os_name}" =~ "KylinLinux" ]] || [[ "${os_name}" =~ "CentOSLinux" ]]; then
      if [[ ${choose_install} =~ "1" ]] && [[ ${choose_install} =~ "2" ]]; then
        logger "Only one hyper-mpi can be installed." ${TIP_COLOR_FAILED}
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
    # Only one is selected by the user.
    # Display the item that the user selects to install.
    select_result=${software_support_list[((${choose_install} - 1))]}
  else
    # Display repeated user input.
    choose_install=$(echo ${choose_install} | tr ',' ' ' | xargs -n 1 | sort -u)
    for i in ${choose_install}; do
      select_result="$select_result,${software_support_list[(($i - 1))]}"
    done
  fi
  logger "The software you select to install is ${select_result#,}." ${TIP_COLOR_SUCCESS}
}

show_software_support_list() {
  # Display the software supported by the current system user.
  logger "Installation environment check result:" ${TIP_COLOR_CHECKING}
  if [[ "${software_support_list}" =~ "HMPI" ]]; then
    logger "The hyper-mpi corresponds to a specific compiler. Install the matched compiler for the hyper-mpi. For example, the compiler corresponding to HMPI-BISHENG is BISHENG." ${TIP_COLOR_ECHO}
  fi
  if [[ "${software_support_list}" =~ "KML" ]]; then
    logger "The KML corresponds to a specific compiler. Install the matched compiler for the KML. For example, the compiler corresponding to KML is GCC." ${TIP_COLOR_ECHO}
  fi
  printf "%-16s %-29s %-14s %-20s\n" "SequenceNumber" "Software" "Support" >>${log_file_of_abspath}
  printf "%-16s %-29s %-14s %-20s\n" "SequenceNumber" "Software" "Support"
  software_support_list=($(echo ${software_support_list} | tr ' ' ' '))
  for ((i = 0; i < ${#software_support_list[@]}; i++)); do
    printf "%-16s %-29s %-20s\n" "$((i + 1))" "${software_support_list[i]}" "${check_result_list[i]}" >>${log_file_of_abspath}
    printf "%-16s %-29s \033[1;32m%-20s\033[0m\n" "$((i + 1))" "${software_support_list[i]}" "${check_result_list[i]}"
  done
}

read_answer() {
  # Process user interaction operations.
  ask_question="$*"
  ask_flag=1
  while [[ ${ask_flag} == 1 ]]; do
    read -p "${ask_question} [Y/N]:" you_choose
    if [[ ${you_choose} =~ ^[Yy]$ ]]; then
      return 0
    elif [[ ${you_choose} =~ ^[nN]$ ]]; then
      return 1
    else
      logger 'input error' ${TIP_COLOR_FAILED}
      continue
    fi
  done
}


hand_precondition_mpi() {
  # Handle the installation exception caused by insufficient environment dependency.
  if [[ ${#miss_package[@]} -gt 0 ]]; then
    logger "The $(echo ${miss_package[@]}) dependency is not detected in the environment." ${TIP_COLOR_WARNING}
    read_answer "The corresponding ${miss_package[@]} is missing or the version is incorrect. Are you sure you want to continue the installation?"
    if [[ $? != 0 ]]; then
      logger "Exit the hyper-mpi installation." ${TIP_COLOR_FAILED}
      return 1
    fi
  fi
  install_hyper_mpi
}

check_precondition_kml() {
  # Check the math library dependency.
  local install_package_kind=$1
  miss_package_kml=()
  libc6="/usr/lib64/libc.so.6"
  libgomp="/usr/lib64/libgomp.so.1"
  [[ ${install_package_kind} == "dpkg" ]] && libc6="/lib/aarch64-linux-gnu/libc.so.6"
  [[ ${install_package_kind} == "dpkg" ]] && libgomp="/usr/lib/aarch64-linux-gnu/libgomp.so.1"
  [[ ! -f ${libc6} ]] && miss_package_kml[${#miss_package_kml[*]}]="libc6"
  [[ ! -f ${libgomp} ]] && miss_package_kml[${#miss_package_kml[*]}]="libgomp"
  result=$(command -v nm)
  if [[ -z ${result} ]]; then
    miss_package_kml[${#miss_package_kml[*]}]="nm"
  fi
}

hand_precondition_kml() {
  if [[ ${#miss_package_kml[@]} -gt 0 ]]; then
    logger "The ${miss_package_kml[@]} dependency is not detected in the environment."
    read_answer "The system does not have dependencies such as ${miss_package_kml[@]} or the version does not meet the requirements. Are you sure you want to continue the installing the KML?"
    if [[ $? != 0 ]]; then
      logger "Exit the KML installation." ${TIP_COLOR_FAILED}
      return 1
    fi
  fi
  if [[ ${kml_install_status} == ${SUCCESS} ]]; then
    del_math_kml
    if [[ $? == 1 ]]; then
      return 1
    fi
  fi
  install_math_kml
}

check_precondition_compiler() {
  # Check the compiler dependency.
  local compiler_type=$1
  miss_package_bisheng=()
  miss_package_gcc=()
  glibc_version=$(ldd --version | grep -Po '\d+.\d+\d+' | head -n 1)
  version_ge "${glibc_version}" "2.17"
  if [[ "$?" != 0 ]]; then
    miss_package_gcc[${#miss_package_gcc[*]}]="GLIBC(>=2.17)"
    miss_package_bisheng[${#miss_package_bisheng[*]}]="GLIBC(>=2.17)"
  fi
  if [[ ${compiler_type} == "gcc" ]]; then
    if [[ ${install_package_kind} == "rpm" ]]; then
      glibc_devel_version=$(${install_package_kind} -qa | grep -E "^glibc-devel" | head -n 1 | grep -Po "\d+\.\d+\d+")
      glibc_devel="glibc-devel(>=2.17)"
    else
      glibc_devel_version=$(${install_package_kind} -l | grep "libc-dev-bin" | head -n 1 | grep -Po "\d+\.\d+\d+")
      glibc_devel="libc-dev-bin(>=2.17)"
    fi
    version_ge "${glibc_devel_version}" '2.17'
    if [[ "$?" != 0 ]]; then
      miss_package_gcc[${#miss_package_gcc[*]}]="${glibc_devel}"
    fi
  fi
  if [[ ${compiler_type} == "bisheng" ]]; then
    gcc_version=$(gcc --version 2>&1 | head -n 1 | grep -Po "\d+\.\d+\.\d+" | head -n 1)
    version_ge "${gcc_version}" '4.8.5'
    if [[ "$?" != 0 ]]; then
      miss_package_bisheng[${#miss_package_bisheng[*]}]="gcc(>=4.8.5)"
    fi
    libatomic_version=$(ldconfig -v 2>&1 | grep libatomic | awk -F '->' '{print $2}' | grep -Po "\d+\.\d+\.\d+")
    version_ge "${libatomic_version}" '1.2.0'
    if [[ "$?" != 0 ]]; then
      miss_package_bisheng[${#miss_package_bisheng[*]}]="libatomic(>=1.2.0)"
    fi
  fi
}

hand_precondition_compiler() {
  local compiler_type=$1
  if [[ ${compiler_type} == "gcc" ]]; then
    miss_package_compiler=(${miss_package_gcc[*]})
    compiler_install_status=${gcc_install_status}
  else
    miss_package_compiler=(${miss_package_bisheng[*]})
    compiler_install_status=${bisheng_install_status}
  fi
  if [[ ${#miss_package_compiler[@]} -gt 0 ]]; then
    logger "The ${miss_package_compiler[@]} dependency is nit detected in the environment."
    read_answer "The system does not have dependencies such as ${miss_package_compiler[@]} or the version does not meet the requirements. Are you sure you want to continue the installing the ${compiler_type^^}?"
    if [[ $? != 0 ]]; then
      logger "Exit the ${compiler_type^^} installation." ${TIP_COLOR_FAILED}
      return 1
    fi
  fi
  install_compiler ${compiler_type}
  if [[ $? == 1 ]]; then
    logger "Exit the ${compiler_type^^} installation." ${TIP_COLOR_FAILED}
    return 1
  fi
}

check_install_user() {
  # Prompt for non-root users
  current_user=$(whoami)
  if [[ "${current_user}" != "root" ]]; then
    logger "The current user is not the root user, run the sudo bash install.sh command." ${TIP_COLOR_WARNING}
    exit 1
  fi
}

check_os_name() {
  # Check whether the current system supports the installation.
  get_os_name
  if [[ ! "${support_os_names}" =~ "${os_name}" ]]; then
    logger "The system does not support the HPC software." ${TIP_COLOR_FAILED}
    exit 1
  fi
}

check_os_architecture() {
  if [[ $(uname -m) != "aarch64" ]]; then
    logger "The system does not support installation. Please select the aarch64 architecture." ${TIP_COLOR_FAILED}
    exit 1
  fi
}

change_directory_owner() {
  local directory="$1"
  if [ -d "${directory}" ]; then
    chown root:root -R ${directory}
  fi
}

change_directory_permissions() {
  local directory="$1"
  local bin_type="$2"
  local flag=1
  local path=''
  if [[ ${bin_type} ]]; then
    [[ ${bin_type} == "hyper-mpi" ]] && local software_ompi_bin="${directory}/ompi/bin" && local software_ucx_bin="${directory}/ucx/bin"
    [[ ${bin_type} == "gcc" ]] && local software_gcc_bin="${directory}/bin" && local software_gcc_gnu_bin="${directory}/aarch64-linux-gnu/bin"
    [[ ${bin_type} == "bisheng" ]] && local software_bisheng_bin="${directory}/bin"
  fi
  if [[ ${customize_path_status} == 1 ]]; then
    cd ${customize_path}
  elif [[ ${customize_path_status} == 2 ]]; then
    if [ -z ${valid_customize_path} ]; then
      local customize_path_special=${customize_path}
    else
      cd ${valid_customize_path}
      local split_path=$(echo $directory | awk -F "^${valid_customize_path}/" '{print $2}')
    fi
  fi
  if [[ "${split_path}" ]];then
    local valid_customize_path_split=${valid_customize_path}
    for path in $(echo ${split_path} | tr '/' ' ');do
      valid_customize_path_split=${valid_customize_path_split}/${path}
      chmod 755 ${valid_customize_path_split}
    done
  else
    local delete_last_slashes=${customize_path%%/}
    for path in $(echo ${directory} | tr '/' ' ');do
      local chmod_path=${chmod_path}/${path}
      [[ -z "${customize_path_special}" && ${#delete_last_slashes} -ge ${#chmod_path} ]] && continue || chmod 755 ${chmod_path}
    done
  fi

  [[ $(ls -A "${directory}") ]] && chmod 755 -R ${directory}/*
  find ${directory} -type f | xargs chmod 644
  [[ ${software_ompi_bin} ]] && find ${software_ompi_bin} -type f | xargs chmod 755
  [[ ${software_ucx_bin} ]] && find ${software_ucx_bin} -type f | xargs chmod 755
  [[ ${software_gcc_bin} ]] && find ${software_gcc_bin} -type f | xargs chmod 755
  [[ ${software_gcc_gnu_bin} ]] && find ${software_gcc_gnu_bin} -type f | xargs chmod 755
  [[ ${software_bisheng_bin} ]] && find ${software_bisheng_bin} -type f | xargs chmod 755
}

del_math_kml() {
  # Delete the original kml environment variables
  read_answer "Before the installation, the existing software will be uninstalled. Are you sure you want to authorize the script to continue?"
  if [[ $? != 0 ]];then 
    logger "Do not install the kml repeatedly." ${TIP_COLOR_ECHO}
    return 1
  fi
  logger "Deleting kml" ${TIP_COLOR_ECHO}
  if [[ ${install_package_kind} == "rpm" ]]; then
    $install_package_kind -e "${boost_math_name}"
  else
    $install_package_kind -P "${boost_math_name}"
  fi
  logger "Deleting kml successfull" ${TIP_COLOR_ECHO}
}

del_hyper_mpi() {
  # Delete the original hyper mpi environment variables
  local hyper_mpi_type="$1"
  install_hyper_mpi_path="${customize_path}hyper_mpi/${hyper_mpi_type}/${hmpi_package_name}"
  hyper_mpi_modules_path="${customize_path}modules/${hyper_mpi_type}/hmpi_modulefiles"
  if [[ -d "${install_hyper_mpi_path}" ]] || [[ -f "${hyper_mpi_modules_path}" ]];then
    [[ -f "${hyper_mpi_modules_path}" ]] && logger "${hyper_mpi_modules_path} file already exists." ${TIP_COLOR_WARNING}
    [[ -d "${install_hyper_mpi_path}" ]] && logger "${install_hyper_mpi_path} directory is not empty." ${TIP_COLOR_WARNING}
    read_answer "Are you sure you want to continue the instation? if yes, ${install_hyper_mpi_path} the directory or ${hyper_mpi_modules_path} file will be overwritten."
    if [[ $? != 0 ]];then 
      logger 'Do not install the hyper-mpi repeatedly.' ${TIP_COLOR_ECHO}
      return 1
    fi
    [[ -f "${hyper_mpi_modules_path}" ]] && rm -rf "${hyper_mpi_modules_path}"
    [[ -d "${install_hyper_mpi_path}" ]] && rm -rf "${install_hyper_mpi_path}"
    logger "Deleting ${hyper_mpi_type}" ${TIP_COLOR_ECHO}
  fi
}

del_compiler() {
  local compiler_type=$1
  local compiler_path="${customize_path}${compiler_type}/${compiler_name}"
  local compiler_modules_path="${customize_path}modules/${compiler_type}_modulefiles"
  if [[ -f "${compiler_modules_path}" ]] || [[ -d "${compiler_path}" ]];then
    [[ -f "${compiler_modules_path}" ]] && logger "${compiler_modules_path} file already exists." ${TIP_COLOR_WARNING}
    [[ -d "${compiler_path}" ]] && logger "${compiler_path} directory is not empty." ${TIP_COLOR_WARNING}
    read_answer "Are you sure you want to continue the instllation? if yes, the ${compiler_path} directory or ${compiler_modules_path} file will be overwritten."
    if [[ $? != 0 ]];then 
      logger "Do not install the ${compiler_type^^} repeatedly." ${TIP_COLOR_ECHO}
      return 1
    fi
    [[ -d "${compiler_path}" ]] && rm -rf ${compiler_path}
    [[ -f "${compiler_modules_path}" ]] && rm -rf ${compiler_modules_path}
    logger "Deleting ${compiler_type^^} " ${TIP_COLOR_ECHO}
  fi
}
