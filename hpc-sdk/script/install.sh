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
source ${current_dir}/const.sh
source ${current_dir}/common_fun.sh
install_package_dir=${current_dir}/../package
module_file_dir=${current_dir}/../module
time_stamp=$(date +%s)
# Press ctrl-c to capture terminal disconnection exceptions.
trap 'onCtrlC' INT
trap 'onCtrlZ' SIGTSTP
trap 'onDisconnect' HUP

set_software_status() {
  # Obtain the installation status of supported software.
  check_result_list=()
  suggestion_list=()
  software_support_list=${software_support_lists["$os_name"]}
  for software_support in $(echo $software_support_list | tr ' ' ' '); do
    if [[ ${software_support} =~ ^HMPI-GCC$|^HMPI-BISHENG$|^BISHENG$|^GCC$|^KML$ ]]; then
      check_result_list[${#check_result_list[*]}]='Y'
    fi
  done
}

get_hyper_mpi_package_name() {
  # Obtain the HMPI package name.
  if [[ "${os_name}" =~ "KylinLinux" ]]; then
    if [[ ${select_result} =~ "HMPI-GCC" ]]; then
      hmpi_package_name=${kylinv10_hmpi_gcc_name}
    elif [[ ${select_result} =~ "HMPI-BISHENG" ]]; then
      hmpi_package_name=${kylinv10_hmpi_bisheng_name}
    fi
  elif [[ "${os_name}" =~ "CentOSLinux" ]]; then
    if [[ ${select_result} =~ "HMPI-GCC" ]]; then
      hmpi_package_name=${centos7_6_linux_hmpi_gcc_name}
    elif [[ ${select_result} =~ "HMPI-BISHENG" ]]; then
      hmpi_package_name=${centos7_6_hmpi_bisheng_name}
    fi
  elif [[ "${os_name}" =~ "openEuler20.03" ]]; then
    if [[ ${select_result} =~ "HMPI-GCC" ]]; then
      hmpi_package_name=${openeuler20_03_lts_hmpi_gcc_name}
    elif [[ ${select_result} =~ "HMPI-BISHENG" ]]; then
      hmpi_package_name=${openeuler20_03_lts_hmpi_bisheng_name}
    fi
  fi
}

check_software_installed() {
  # Check the installed software
  case $1 in
  "HMPI-GCC"|"HMPI-BISHENG")
    hmpi_gcc_install_status=${FAILED}
    hmpi_bisheng_install_status=${FAILED}
    hyper_install_status=${FAILED}
    which mpirun >/dev/null 2>&1
    if [[ $? == 0 ]]; then
      hyper_install_status=${SUCCESS}
      hmpi_name_string=$(which mpirun | awk -F "/ompi/bin/mpirun" '{print $1}')
      if [[ "${hyper_mpi_gcc_package_names[@]}" =~ "${hmpi_name_string##*/}" ]];then
          hmpi_gcc_install_status=${SUCCESS}
      fi
      if [[ "${hyper_mpi_bisheng_package_names[@]}" =~ "${hmpi_name_string##*/}" ]];then
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
    # clang -v Standard errot output 2>&1
    check_system_bisheng_status
    ;;
  "KML")
    [[ ${install_package_kind} == "rpm" ]] && result=$(${install_package_kind} -qa ${boost_math_name}) || result=$(${install_package_kind} -l ${boost_math_name})
    [[ "${result}" ]] && kml_install_status=${SUCCESS} || kml_install_status=${FAILED}
    ;;
  esac
}

check_system_gcc_status() {
  # Check whether the GCC version in the system is supported.
  install_reply=$1
  [[ ${install_reply} == "kml" ]] && gcc_support_version='7.3.0' || gcc_support_version='9.3.0'
  gcc_version=$(gcc --version 2>&1 | head -n 1 | grep -Po "\d+\.\d+\.\d+" | head -n 1)
  if [[ ${install_reply} == "kml" ]]; then
    version_ge ${gcc_version} ${gcc_support_version}
    [[ "$?" == "0" ]] && gcc_kml_check_status=${SUCCESS} || gcc_kml_check_status=${FAILED}
  else
    [[ ${gcc_support_version} == ${gcc_version} ]] && gcc_hmpi_check_status=${SUCCESS} || gcc_hmpi_check_status=${FAILED}
  fi
}

check_system_bisheng_status() {
  # Check whether the Bisheng version in the system is supported.
  if clang -v 2>&1 | grep -q "HUAWEI BiSheng Compiler 2.1.0.B010 clang version 12.0.0"; then
    bisheng_check_status=${SUCCESS}
  else
    bisheng_check_status=${FAILED}
    if grep -q "bisheng-compiler-2.1.0-aarch64-linux" /etc/profile; then
      bisheng_check_status=${SUCCESS}
    fi
  fi
}

set_software_choose_status() {
  # Set the status of the software selected by the user.
  [[ "${select_result}" =~ ^GCC*$|,GCC* ]] && gcc_choose_status=${SUCCESS} || gcc_choose_status=${FAILED}
  [[ "${select_result}" =~ ^BISHENG*$|,BISHENG* ]] && bisheng_choose_status=${SUCCESS} || bisheng_choose_status=${FAILED}
  [[ "${select_result}" =~ "HMPI-GCC" ]] && hmpi_gcc_choose_status=${SUCCESS} || hmpi_gcc_choose_status=${FAILED}
  [[ "${select_result}" =~ "HMPI-BISHENG" ]] && hmpi_bisheng_choose_status=${SUCCESS} || hmpi_bisheng_choose_status=${FAILED}
  [[ "${select_result}" =~ "KML" ]] && kml_choose_status=${SUCCESS} || kml_choose_status=${FAILED}
}

install_hyper_mpi_env_check() {
  # Check the hyper mpi installation environment.
  hmpi_type=$1
  [[ ${hmpi_type} == "gcc" ]] && hmpi_choose_status=${hmpi_gcc_choose_status} || hmpi_choose_status=${hmpi_bisheng_choose_status}
  if [[ ${hmpi_choose_status} == ${FAILED} ]]; then
    return 1
  fi
  [[ ${hmpi_type} == "gcc" ]] && hmpi_install_status=${hmpi_gcc_install_status} || hmpi_install_status=${hmpi_bisheng_install_status}
  [[ ${hmpi_type} == "gcc" ]] && compiler_choose_status=${gcc_choose_status} || compiler_choose_status=${bisheng_choose_status}
  [[ ${hmpi_type} == "gcc" ]] && compiler_hmpi_check_status=${gcc_hmpi_check_status} || compiler_hmpi_check_status=${bisheng_check_status}
  logger "Start installing the HMPI-${hmpi_type^^}." ${TIP_COLOR_CHECKING}
  unset miss_package
  if [[ ! -f ${install_package_dir}/hyper_mpi/"${hmpi_package_name}.tar.gz" ]]; then
    logger "The installation package cannot be found." ${TIP_COLOR_SUCCESS}
    return 1
  fi
  if [[ ${hyper_install_status} == ${SUCCESS} ]];then
    if [[ ${hmpi_gcc_install_status} == ${SUCCESS} ]] && [[ ${hmpi_type} == "gcc" ]];then
      answer_msg="The HPMI-${hmpi_type^^} already exists. Are you sure you want to continue installing the HPMI-${hmpi_type^^}?"
    fi
    if [[ ${hmpi_gcc_install_status} == ${SUCCESS} ]] && [[ ${hmpi_type} == "bisheng" ]];then
      answer_msg="The HPMI-GCC already exists. Are you sure you want to continue installing the HPMI-${hmpi_type^^}?"
    fi
    if [[ ${hmpi_bisheng_install_status} == ${SUCCESS} ]] && [[ ${hmpi_type} == "gcc" ]];then
      answer_msg="The HPMI-BISHENG already exists. Are you sure you want to continue installing the HPMI-${hmpi_type^^}?"
    fi
    if [[ ${hmpi_bisheng_install_status} == ${SUCCESS} ]] && [[ ${hmpi_type} == "bisheng" ]];then
      answer_msg="The HPMI-${hmpi_type^^} already exists. Are you sure you want to continue installing the HPMI-${hmpi_type^^}?"
    fi
    logger "Hyper-mpi is installed in this system" ${TIP_COLOR_WARNING}
    read_answer "${answer_msg}"
    if [[ $? != 0 ]]; then
      logger "Do not install the hyper-mpi repeatedly." ${TIP_COLOR_ECHO}
      return 1
    fi
  fi
  if [[ ${compiler_hmpi_check_status} == ${SUCCESS} ]]; then
    hand_precondition_mpi
    return 0
  fi
  [[ ${hmpi_type} == "gcc" ]] && logger "The gcc version must be 9.3.0 in the system where the hmpi-gcc is to be installed" ${TIP_COLOR_WARNING}
  [[ ${hmpi_type} == "gcc" ]] && miss_package[${#miss_package[*]}]="${hmpi_type}(9.3.0)"
  hand_precondition_mpi
}

install_kml_env_check() {
  # Check the math library
  if [[ ${kml_choose_status} == ${FAILED} ]]; then
    return 1
  fi
  logger "Start installing the KML." ${TIP_COLOR_CHECKING}
  check_precondition_kml ${install_package_kind}
  if [[ ${install_package_kind} == "rpm" ]] && [[ ! -f ${install_package_dir}/kml/${boost_math_rpm_name}.rpm ]]; then
    logger "The installation package cannot be found." ${TIP_COLOR_SUCCESS}
    return 1
  elif [[ ${install_package_kind} == "dpkg" ]] && [[ ! -f ${install_package_dir}/kml/${boost_math_deb_name}.deb ]]; then
    logger "The installation package cannot be found." ${TIP_COLOR_SUCCESS}
    return 1
  fi
  if [[ ${kml_install_status} == ${SUCCESS} ]]; then
    logger "The KML already exists. Are you sure you want to continue installing the KML?"
    read_answer "The KML already exists. Are you sure you want to continue installing the KML?"
    if [[ $? != 0 ]]; then
      logger "The KML already exists. Are you sure you want to continue installing the KML? N"
      logger "Do not install the KML repeatedly." ${TIP_COLOR_FAILED}
      return 1
    fi
  fi
  if [[ ${gcc_kml_check_status} == ${SUCCESS} ]]; then
    hand_precondition_kml
    return 0
  fi
  miss_package_kml[${#miss_package_kml[*]}]='gcc(>=7.3.0)'
  hand_precondition_kml
}

install_compiler_env_check() {
  # Check the compiler before installation.
  local compiler_type=$1
  [[ ${compiler_type} == "gcc" ]] && compiler_choose_status=${gcc_choose_status} || compiler_choose_status=${bisheng_choose_status}
  if [[ ${compiler_choose_status} == ${SUCCESS} ]]; then
    [[ ${compiler_type} == "gcc" ]] && compiler_install_status=${gcc_install_status} || compiler_install_status=${bisheng_check_status}
    [[ ${compiler_type} == "gcc" ]] && compiler_name=${gcc_compiler_name} || compiler_name=${bisheng_compiler_name}
    check_precondition_compiler ${compiler_type}
    logger "Start installing the ${compiler_type^^}." ${TIP_COLOR_CHECKING}
    if [[ ! -f ${install_package_dir}/${compiler_type}/"${compiler_name}.tar.gz" ]]; then
      logger "The installation package cannot be found." ${TIP_COLOR_SUCCESS}
      return 1
    fi
    # Check whether the compiler is installed.
    if [[ ${compiler_install_status} == ${SUCCESS} ]]; then
      logger "The ${compiler_type^^} already exists. Are you sure you want to continue installing the ${compiler_type^^}?"
      read_answer "The ${compiler_type^^} already exists. Are you sure you want to continue installing the ${compiler_type^^}?"
      if [[ $? != 0 ]]; then
        logger "Do not install the ${compiler_type^^} repeatedly." ${TIP_COLOR_FAILED}
        return 1
      fi
    fi
    if [[ ${compiler_type} == "gcc" ]];then
      hand_precondition_compiler "gcc" 
    else
      hand_precondition_compiler "bisheng"
    fi
    [[ $? == 1 ]] && return 1
  fi
  if [[ ${compiler_type} == "gcc" ]]; then
    # Check whether the GCC in the systecm meets the requirements. If no, add a mark.
    check_system_gcc_status "kml"
    check_system_gcc_status "hmpi"
  else
    check_system_bisheng_status
  fi
}

install_hyper_mpi() {
  # Install Hyper MPI
  user_customize_path 'hyper-mpi'
  logger "You choose to install Hyper MPI in the $(
    cd ${customize_path}
    pwd
  ) directory." ${TIP_COLOR_SUCCESS}
  cd ${install_package_dir}/hyper_mpi
  [[ ${hmpi_gcc_choose_status} == 1 ]] && install_hyper_mpi_name="hyper_mpi_gcc"
  [[ ${hmpi_bisheng_choose_status} == 1 ]] && install_hyper_mpi_name="hyper_mpi_bisheng"
  del_hyper_mpi "${install_hyper_mpi_name}"
  if [[ $? == 1 ]];then
    return 1
  fi
  cd ${install_package_dir}/hyper_mpi
  install_hmpi_path=${customize_path}hyper_mpi/${install_hyper_mpi_name}
  if [ ! -d "${install_hmpi_path}" ]; then
    mkdir -p ${install_hmpi_path}
  fi
  tar -xf ${hmpi_package_name}.tar.gz -C ${install_hmpi_path}
  cd ${install_hmpi_path}/${hmpi_package_name}
  # Suggestions for setting Hyper MPI environment variables
  command_pwd="  echo 'hwmpi=$PWD' >> ~/.bashrc"
  command_opal_prefix="  echo 'export OPAL_PREFIX=\${hwmpi}/ompi' >> ~/.bashrc"
  command_path="  echo 'export PATH=\${hwmpi}/ompi/bin:\${hwmpi}/ucx/bin:\$PATH' >> ~/.bashrc"
  command_include="  echo 'export INCLUDE=\${hwmpi}/ompi/include:\${hwmpi}/ucx/include:\$INCLUDE' >> ~/.bashrc"
  command_ld_library_path="  echo 'export LD_LIBRARY_PATH=\${hwmpi}/ompi/lib:\${hwmpi}/ucx/lib:\$LD_LIBRARY_PATH' >> ~/.bashrc"
  logger "Hyper MPI is installed."
  if [[ ${hmpi_gcc_choose_status} == 1 ]]; then
    change_modules "HMPI-GCC"
    hmpi_gcc_installed=1
    sed -i "s#hyper_mpi_gcc=.*#hyper_mpi_gcc=${hmpi_package_name}#g" ${current_dir}/configure_environment_${time_stamp}.sh
    sed -i "s#hyper_mpi_gcc_install=.*#hyper_mpi_gcc_install=${install_hmpi_path}/${hmpi_package_name}#g"  ${current_dir}/configure_environment_${time_stamp}.sh
  elif [[ ${hmpi_bisheng_choose_status} == 1 ]]; then
    change_modules "HMPI-BISHENG"
    hmpi_bisheng_installed=1
    sed -i "s#hyper_mpi_bisheng=.*#hyper_mpi_bisheng=${hmpi_package_name}#g" ${current_dir}/configure_environment_${time_stamp}.sh
    sed -i "s#hyper_mpi_bisheng_install=.*#hyper_mpi_bisheng_install=${install_hmpi_path}/${hmpi_package_name}#g"  ${current_dir}/configure_environment_${time_stamp}.sh
  fi
  change_directory_owner "${install_hmpi_path}/${hmpi_package_name}"
  change_directory_permissions "${install_hmpi_path}/${hmpi_package_name}" "hyper-mpi"
  [[ "${install_hyper_mpi_name}" == "hyper_mpi_bisheng" ]] && install_hyper_mpi_tip="HMPI-BISHENG" || install_hyper_mpi_tip="HMPI-GCC"
  logger "To make the related commands take effect, run the following command:" ${TIP_COLOR_WARNING}
  logger "${command_pwd}" ${TIP_COLOR_COMMAND}
  logger "${command_opal_prefix}" ${TIP_COLOR_COMMAND}
  logger "${command_path}" ${TIP_COLOR_COMMAND}
  logger "${command_include}" ${TIP_COLOR_COMMAND}
  logger "${command_ld_library_path}" ${TIP_COLOR_COMMAND}
  logger "  source ~/.bashrc" ${TIP_COLOR_COMMAND}
  logger "Use module set env for ${install_hyper_mpi_tip}" ${TIP_COLOR_SUCCESS}
  logger "  module load ${show_modulefile_path}" ${TIP_COLOR_COMMAND}
  logger "The ${install_hyper_mpi_tip} is installed." ${TIP_COLOR_SUCCESS}
}

install_compiler() {
  # Install compiler
  local compiler_type=$1
  user_customize_path "${compiler_type} compiler"
  logger "you choose install ${compiler_type} path is $(cd ${customize_path};pwd)" ${TIP_COLOR_SUCCESS}
  del_compiler "${compiler_type}"
  if [[ $? == 1 ]];then
    return 1
  fi
  cd ${install_package_dir}/${compiler_type}
  install_compiler_path=${customize_path}${compiler_type}
  if [ ! -d ${install_compiler_path} ]; then
    mkdir -p ${install_compiler_path}
  fi
  tar -xf ${compiler_name}.tar.gz -C ${install_compiler_path}
  cd ${install_compiler_path}/${compiler_name}
  if [[ ${compiler_type} == "bisheng" ]]; then
    change_modules "BISHENG"
    bisheng_installed=1
    sed -i "s#compiler_bisheng=.*#compiler_bisheng=${compiler_name}#g" ${current_dir}/configure_environment_${time_stamp}.sh
    sed -i "s#compiler_bisheng_install=.*#compiler_bisheng_install=${install_compiler_path}#g"  ${current_dir}/configure_environment_${time_stamp}.sh
  else
    change_modules "GCC"
    gcc_installed=1
    sed -i "s#compiler_gcc=.*#compiler_gcc=${compiler_name}#g" ${current_dir}/configure_environment_${time_stamp}.sh
    sed -i "s#compiler_gcc_install=.*#compiler_gcc_install=${install_compiler_path}#g" ${current_dir}/configure_environment_${time_stamp}.sh
  fi
  if [[ ${compiler_type} == "gcc" ]]; then
    command_env_path="  echo 'export PATH=${install_compiler_path}/${compiler_name}/bin:\$PATH' >>/etc/profile"
    command_env_include="  echo 'export INCLUDE=${install_compiler_path}/${compiler_name}/INCLUDE:\$INCLUDE' >>/etc/profile"
    command_env_ld_library_path="  echo 'export LD_LIBRARY_PATH=${install_compiler_path}/${compiler_name}/lib64:\$LD_LIBRARY_PATH' >>/etc/profile"
    command_module=" module load ${show_modulefile_path}"
  fi
  if [[ ${compiler_type} == "bisheng" ]]; then
    command_env_path="  echo 'export PATH=${install_compiler_path}/${compiler_name}/bin:\$PATH' >>/etc/profile"
    command_env_ld_library_path="  echo 'export LD_LIBRARY_PATH=${install_compiler_path}/${compiler_name}/lib:\$LD_LIBRARY_PATH' >>/etc/profile"
    command_module="  module load ${show_modulefile_path}"
  fi
  change_directory_owner "${install_compiler_path}/${compiler_name}"
  change_directory_permissions "${install_compiler_path}/${compiler_name}" "${compiler_type}"
  logger "To make the related commands take effect, run the following command:" ${TIP_COLOR_WARNING}
  [[ ${command_env_path} ]] && logger "${command_env_path}" ${TIP_COLOR_COMMAND}
  [[ ${command_env_include} && ${compiler_type} == "gcc" ]] && logger "${command_env_include}" ${TIP_COLOR_COMMAND}
  [[ ${command_env_ld_library_path} ]] && logger "${command_env_ld_library_path}" ${TIP_COLOR_COMMAND}

  logger "  source /etc/profile" ${TIP_COLOR_COMMAND}
  logger "use module set env for $compiler_type" ${TIP_COLOR_SUCCESS}
  logger "${command_module}" ${TIP_COLOR_COMMAND}
  logger "The ${compiler_type} Compiler is installed." ${TIP_COLOR_SUCCESS}
}

install_math_kml() {
  # Install KML
  check_space /usr/local
  if [[ ${install_package_kind} == "rpm" ]]; then
    $install_package_kind -ivh ${install_package_dir}/kml/${boost_math_rpm_name}.rpm
  else
    $install_package_kind -i ${install_package_dir}/kml/${boost_math_deb_name}.deb
  fi
  if [[ $? -ne 0 ]];then
    logger "Failed to install the math library." ${TIP_COLOR_FAILED}
    return 1
  fi
  nm_math_kml
  if [[ $? -ne 0 ]];then
    logger "Failed to install the math library." ${TIP_COLOR_FAILED}
    return 1
  fi
  logger "To make the related commands take effect, run the following command: source /etc/profile" ${TIP_COLOR_WARNING}
  logger "The math library is installed." ${TIP_COLOR_SUCCESS}
}

nm_math_kml() {
  # Add math library symbols
  # /usr/local/kml/lib is the default path for installing the math library.
  logger "Generating a Complete LAPACK..." ${TIP_COLOR_CHECKING}
  cp -rf ${install_package_dir}/kml/liblapack_adapt.a /usr/local/kml/lib
  cp -rf ${install_package_dir}/kml/libblas.a /usr/local/kml/lib
  cd /usr/local/kml/lib
  klapack=/usr/local/kml/lib/libklapack.a
  # get symbols defined both in klapack and netlib lapack
  nm -g liblapack_adapt.a | grep 'T ' | grep -oP '\K\w+(?=_$)' | sort | uniq >netlib.sym
  nm -g $klapack | grep 'T ' | grep -oP '\K\w+(?=_$)' | sort | uniq >klapack.sym
  comm -12 klapack.sym netlib.sym >comm.sym

  # update symbols name of liblapack_adapt.a
  while read sym; do
    if ! nm liblapack_adapt.a | grep -qe " T ${sym}_\$"; then
      continue
    fi
    ar x liblapack_adapt.a $sym.f.o >/dev/null 2>&1
    mv $sym.f.o ${sym}_netlib.f.o
    objcopy --redefine-sym ${sym}_=${sym}_netlib_ ${sym}_netlib.f.o
    ar d liblapack_adapt.a ${sym}.f.o >/dev/null 2>&1
    ar ru liblapack_adapt.a ${sym}_netlib.f.o >/dev/null 2>&1
    rm ${sym}_netlib.f.o
  done <comm.sym
}

change_modules_hmpi(){
  local hmpi_type="$1"
  logger "Set modules for HMPI-${hmpi_type^^}." ${TIP_COLOR_ECHO}
  # Original module content
  old_msg=$(sed -n '/set prefix/p' ${module_file_dir}/hmpi_modulefiles)
  sed -i s"#${old_msg}#set prefix $(cd ${install_hmpi_path};pwd)/${hmpi_package_name}#" ${module_file_dir}/hmpi_modulefiles
  if [ ! -d ${customize_path}/modules/hyper_mpi_${hmpi_type} ]; then
    mkdir -p ${customize_path}/modules/hyper_mpi_${hmpi_type}
  fi
  cp -rf ${module_file_dir}/hmpi_modulefiles ${customize_path}/modules/hyper_mpi_${hmpi_type}/
  show_modulefile_path=${customize_path}modules/hyper_mpi_${hmpi_type}/hmpi_modulefiles
  logger "The path of HMPI-${hmpi_type^^} modules is ${show_modulefile_path}." ${TIP_COLOR_ECHO}
}

change_modules_compiler(){
  local compiler_type="$1"
  logger "Set modules for ${compiler_type^^}." ${TIP_COLOR_ECHO}
  old_msg=$(sed -n '/set prefix/p' ${module_file_dir}/${compiler_type}_modulefiles)
  sed -i s"#${old_msg}#set prefix $(
    cd ${install_compiler_path}
    pwd
  )/${compiler_name}#" ${module_file_dir}/${compiler_type}_modulefiles
  if [ ! -d ${customize_path}/modules/${compiler_type} ]; then
    mkdir -p ${customize_path}/modules/${compiler_type}
  fi
  cp -rf ${module_file_dir}/${compiler_type}_modulefiles ${customize_path}/modules/${compiler_type}
  show_modulefile_path=${customize_path}modules/${compiler_type}/${compiler_type}_modulefiles
  logger "The path of ${compiler_type^^} modules is ${show_modulefile_path}." ${TIP_COLOR_ECHO}
}

change_modules() {
  # Modify 'modules file'
  install_software=$1
  case $install_software in
  "HMPI-GCC")
    change_modules_hmpi "gcc"
    ;;
  "HMPI-BISHENG")
    change_modules_hmpi "bisheng"
    ;;
  "BISHENG")
    change_modules_compiler "bisheng"
    ;;
  "GCC")
    change_modules_compiler "gcc"
    ;;
  esac
  change_directory_owner "${customize_path}modules"
  change_directory_permissions "${customize_path}modules"
}

check_environment(){
  # Checking the Environment
  check_install_user
  create_log_file
  logger "Start installing the HPC software." ${TIP_COLOR_ECHO}
  logger "Do not press Ctrl+Z or Ctrl+C or restart the system during the installation." ${TIP_COLOR_WARNING}
  check_os_architecture
  check_os_name
}

install_main() {
  check_environment
  # Handles software support installation lists and user interacation.
  get_install_kml_mode
  set_software_status
  show_software_support_list
  user_choose
  get_hyper_mpi_package_name
  for result in $(echo ${select_result} | tr ',' ' '); do
    check_software_installed $result
  done
  cp ${current_dir}/configure_environment.sh ${current_dir}/configure_environment_${time_stamp}.sh

  # Handle the software installation process. 
  set_software_choose_status
  install_compiler_env_check "gcc"
  install_compiler_env_check "bisheng"
  install_hyper_mpi_env_check "gcc"
  install_hyper_mpi_env_check "bisheng"
  install_kml_env_check
  if [[ "${gcc_installed}${bisheng_installed}${hmpi_bisheng_installed}${hmpi_gcc_installed}" =~ '1' ]];then
    # Execute the script for configuring environment variables.
    echo -e "\e[1;33mRun the script ${current_dir}/configure_environment_${time_stamp}.sh to configure environment variables.\e[0m"
  else
    [[ -f "${current_dir}/configure_environment_${time_stamp}.sh" ]] && rm -f "${current_dir}/configure_environment_${time_stamp}.sh"
  fi
  exit 0
}

install_main
