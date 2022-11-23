#!/usr/bin/bash

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
current_dir=$(cd $(dirname $0);pwd)
driver_dir=${current_dir}/../driver 

exit_or_not(){
    if [ $(echo $?) -ne 0 ];then
        echo "--------------$1 build failed-------------------"
        exit -1
    else
        echo "--------------$1 build success-------------------"
    fi
}

rpm -ivh ${driver_dir}/hinicadm*.rpm --force

rpm -e $(rpm -qa | grep hinic3) --nodeps
rpm -e $(rpm -qa | grep hiovs) --nodeps
rpm -e $(rpm -qa | grep hisdk) --nodeps


rpm -ivh ${driver_dir}/hisdk*.rpm --force
exit_or_not "sdk"

modprobe hiudk3
exit_or_not "modprobe udk"

modprobe hisdk3
exit_or_not "modprobe sdk"


rpm -ivh ${driver_dir}/hinic*.rpm --force
exit_or_not "nic"

rpm -ivh ${driver_dir}/hiovs*.rpm --force
exit_or_not "ovs"

[[ -f "/etc/modprobe.d/blacklist.conf" ]] && rm -f "/etc/modprobe.d/blacklist.conf"
echo "softdep hinic3 pre:virtio" > /etc/modprobe.d/sp680.conf

cd ${driver_dir}
hinicadm3 updatefw -i hinic0 -f ./Hinic3_flash.bin -x 0
exit_or_not "firmaware"