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

# 读取配置文件信息作为安装脚本全局变量使用
current_dir=$(cd $(dirname $0); pwd)
# os支持
support_os_names=$(sed '/^SUPPORT_OS_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)

declare -A soft_support_lists=(["openEuler20.03(LTS-SP1)"]="BISHENG GCC KML"
                                ["openEuler20.03(LTS-SP2)"]="BISHENG GCC KML"
                                ["openEuler20.03(LTS-SP3)"]="BISHENG GCC KML"
                                ["KylinLinuxAdvancedServerV10"]="HMPI_GCC HMPI_BISHENG BISHENG GCC"
                                ["UnionTechOSServer20"]="BISHENG GCC"
                                ["CentOSLinuxrelease7.6.1810"]="HMPI_GCC HMPI_BISHENG BISHENG GCC KML"
                                ["Ubuntu20.04"]="BISHENG GCC KML"
                                ["Ubuntu18.04"]="BISHENG GCC KML"
                                ["KylinLinuxAdvancedServerV10_HMPI_GCC"]="${kylinv10_hmpi_gcc_name}"
                                ["KylinLinuxAdvancedServerV10_HMPI_BISHENG"]="${kylinv10_hmpi_bisheng_name}"
                                ["CentOSLinuxrelease7.6.1810_HMPI_GCC"]="${centos7_6_linux_hmpi_gcc_name}"
                                ["CentOSLinuxrelease7.6.1810_HMPI_BISHENG)"]="${centos7_6_hmpi_bisheng_name}"
)
# 磁盘最小要求
low_space=$(sed '/^LOW_SPACE=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
# 数学库名称
boost_math_name="boostkit-kml"
boost_math_rpm_name=$(sed '/^LIBRARY_MATH_RPM_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
boost_math_deb_name=$(sed '/^LIBRARY_MATH_RPM_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
bisheng_compiler_name=$(sed '/^BISHENG_COMPILER_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
gcc_compiler_name=$(sed '/^GCC_COMPILER_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
kylinv10_hmpi_gcc_name=$(sed '/^KYLINV10_LINUX_HMPI_GCC_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
kylinv10_hmpi_bisheng_name=$(sed '/^KYLINV10_LINUX_HMPI_BISHENG_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
centos7_6_linux_hmpi_gcc_name=$(sed '/^CENTOS7.6_LINUX_HMPI_GCC_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
centos7_6_hmpi_bisheng_name=$(sed '/^CENTOS7.6_LINUX_HMPI_BISHENG_NAME=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
# 需要安装的软件总表
software_to_be_installed=$(sed '/^SOFTWARE_TO_BE_INSTALLED=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
# 自定义路径规则
ALLOW_STRING=$(sed '/^ALLOW_STRING=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)
# 成功或者失败的状态
SUCCESS=1
FAILED=0
# 终端输入的颜色处理
TIP_COLOR_FAILED=$(sed '/^TIP_COLOR_FAILED=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)       # 红色
TIP_COLOR_SUCCESS=$(sed '/^TIP_COLOR_SUCCESS=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)     # 绿色
TIP_COLOR_WARNING=$(sed '/^TIP_COLOR_WARNING=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)     # 黄色
TIP_COLOR_OSCHECK=$(sed '/^TIP_COLOR_OSCHECK=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)     # 紫色
TIP_COLOR_CHECKING=$(sed '/^TIP_COLOR_CHECKING=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)   # 蓝色
TIP_COLOR_ECHO=$(sed '/^TIP_COLOR_ECHO=/!d;s/.*=//;s/\r//' ${current_dir}/const.conf)           # 白色

