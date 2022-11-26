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
ip_pattern="^\([1-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)$"

get_local_ip(){
    local input_ip=$1
    local_ips=$(ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}')
    if [[ ${local_ips} =~ ${input_ip} ]]; then
        echo 'No need to check the environment again because the two servers have the same IP address.'
        return 1
    fi
}

create_secret_key(){
    if [[ -f ${HOME}/.ssh/id_rsa.pub && -f ${HOME}/.ssh/id_rsa ]];then
        echo 'The public key already exists.'
    else
        ssh-keygen -t rsa
        if [ $? != 0 ];then
            echo 'Failed to create the key.'
            return 1
        fi
    fi
}

config_password_free(){
    local user=$1
    local ip=$2
    local port=$3
    ssh-copy-id -i ${HOME}/.ssh/id_rsa.pub $user@$ip -p $port
    if [ $? == 0 ]; then
        echo 'Password-free configuration succeeded.'
    else
        echo 'Failed to configure password-free.'
        return 1
    fi    
}

check_password_free_status(){
    local user_ip=$1
    local port=$2
    res=$(ssh ${user_ip} -p ${port} -o PreferredAuthentications=publickey -o StrictHostKeyChecking=no "date" | wc -l)
    if [[ $res == 1 ]]; then
        echo "Password-free has been configured for the remote server ${IP}."
    else
        echo "Password-free is not configured for the remote server ${IP}."
        return 1
    fi
}

get_user_connect_info_ip(){
    # Obtain the IP address entered by the user.
    local type="$1"
    while true; do
        echo -n "Input your ${type} ip: "
        read -r  IP
        if [[ $IP == y ]];then
            read -r  IP
        fi
        if echo ${IP} | grep -v ${ip_pattern} >/dev/null ;then
            echo "The enter IP address is invalid."
            continue
        fi
        if [ -z ${IP} ]; then
            continue
        else
            break
        fi
    done

    if [[ "${type}" ]]; then
        input_ip=${IP}
    else
        echo "Input error"
        exit
    fi
}

get_user_connect_info_port(){
    # Obtain the port entered by the user.
    local type="$1"
    while true; do
        echo -n "Input your ${type} port: "
        read -r PORT
        if [[ ! "${PORT}" =~ ^[1-9]+$ ]];then
            echo "The enter port is invalid."
            continue
        fi
        if [[ ${PORT} -gt 66535 ]] || [[ ${PORT} -lt 0 ]];then
            echo "Enter the correct port number. The port number cannot be greater than 65535"
            continue
        fi
        if [ -z ${PORT} ]; then
            continue
        else
            break
        fi
    done
    if [[ "${type}" ]]; then
        input_port=${PORT}
    else
        echo "Input error"
        exit
    fi
}

get_user_connect_info_username(){
    # Obtain the user name entered by the user.
    local type="$1"
    while true; do
        echo -n "Input your ${type} username: "
        read -r USERNAME
        if [ -z ${USERNAME} ]; then
            continue
        else
            break
        fi
    done

    if [[ "${type}" ]]; then
        input_username=${USERNAME}
    else
        echo "Input error"
        exit
    fi
}