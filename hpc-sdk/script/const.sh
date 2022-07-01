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

# Read the configuration file and use it as the global variable of the installation script
current_dir=$(
  cd $(dirname $0)
  pwd
)
# Supported OSs
support_os_names=$(sed '/^SUPPORT_OS_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)

declare -A software_support_lists=(["openEuler20.03(LTS)"]="HMPI-BISHENG BISHENG GCC KML"
  ["openEuler20.03(LTS-SP1)"]="BISHENG GCC KML"
  ["openEuler20.03(LTS-SP2)"]="BISHENG GCC KML"
  ["openEuler20.03(LTS-SP3)"]="BISHENG GCC KML"
  ["KylinLinuxAdvancedServerV10"]="HMPI-GCC HMPI-BISHENG BISHENG GCC"
  ["UnionTechOSServer20"]="BISHENG GCC"
  ["CentOSLinuxrelease7.6.1810"]="HMPI-GCC HMPI-BISHENG BISHENG GCC KML"
  ["Ubuntu20.04"]="BISHENG GCC KML"
  ["Ubuntu18.04"]="BISHENG GCC KML"
  ["KylinLinuxAdvancedServerV10_HMPI_GCC"]="${kylinv10_hmpi_gcc_name}"
  ["KylinLinuxAdvancedServerV10_HMPI_BISHENG"]="${kylinv10_hmpi_bisheng_name}"
  ["CentOSLinuxrelease7.6.1810_HMPI_GCC"]="${centos7_6_linux_hmpi_gcc_name}"
  ["CentOSLinuxrelease7.6.1810_HMPI_BISHENG"]="${centos7_6_hmpi_bisheng_name}"
)
# Minimum drive size
low_space=$(sed '/^LOW_SPACE=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
low_space_package_g=$(sed '/^LOW_SPACE_PACKAGE_G=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
# Math library name
boost_math_name="boostkit-kml"
boost_math_rpm_name=$(sed '/^LIBRARY_MATH_RPM_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
boost_math_deb_name=$(sed '/^LIBRARY_MATH_DEB_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
bisheng_compiler_name=$(sed '/^BISHENG_COMPILER_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
gcc_compiler_name=$(sed '/^GCC_COMPILER_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
kylinv10_hmpi_gcc_name=$(sed '/^KYLINV10_LINUX_HMPI_GCC_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
kylinv10_hmpi_bisheng_name=$(sed '/^KYLINV10_LINUX_HMPI_BISHENG_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
centos7_6_linux_hmpi_gcc_name=$(sed '/^CENTOS7.6_LINUX_HMPI_GCC_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
centos7_6_hmpi_bisheng_name=$(sed '/^CENTOS7.6_LINUX_HMPI_BISHENG_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
openeuler20_03_lts_hmpi_bisheng_name=$(sed '/^OPENEULER20.03_LTS_LINUX_HMPI_BISHENG_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
hyper_mpi_gcc_package_names=(${kylinv10_hmpi_gcc_name} ${centos7_6_linux_hmpi_gcc_name})
hyper_mpi_bisheng_package_names=(${kylinv10_hmpi_bisheng_name} ${centos7_6_hmpi_bisheng_name} ${openeuler20_03_lts_hmpi_bisheng_name})
# List of software to be installed
software_to_be_installed=$(sed '/^SOFTWARE_TO_BE_INSTALLED=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
# User-defined path rule
ALLOW_STRING=$(sed '/^ALLOW_STRING=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
# Success or failure status
SUCCESS=1
FAILED=0
# Color processing for terminal output
TIP_COLOR_FAILED=$(sed '/^TIP_COLOR_FAILED=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)     # red
TIP_COLOR_SUCCESS=$(sed '/^TIP_COLOR_SUCCESS=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)   # green
TIP_COLOR_WARNING=$(sed '/^TIP_COLOR_WARNING=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)   # yellow
TIP_COLOR_COMMAND=$(sed '/^TIP_COLOR_COMMAND=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)   # purple
TIP_COLOR_CHECKING=$(sed '/^TIP_COLOR_CHECKING=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf) # blue
TIP_COLOR_ECHO=$(sed '/^TIP_COLOR_ECHO=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)         # white
