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

current_dir=$(cd "$(dirname "$0")";pwd)
source ${current_dir}/common_func.sh 

get_user_connect_info(){
    local type=$1
    get_user_connect_info_ip  ${type}
    get_user_connect_info_port ${type}
    get_user_connect_info_username ${type}
}

password_free_check(){
    local user=$1
    local ip=$2
    local port=$3
    get_local_ip ${ip}
    if [ $? == 1 ]; then
        echo "No password-free configuration is required."
    else
        check_password_free_status ${user}@${ip} ${port}
        if [[ $? != 0 ]];then
            create_secret_key
            if [[ $? == 0 ]];then
                config_password_free ${user} ${ip} ${port}
            fi
        fi
    fi
}


get_user_connect_info 'target'
password_free_check ${input_username} ${input_ip} ${input_port}