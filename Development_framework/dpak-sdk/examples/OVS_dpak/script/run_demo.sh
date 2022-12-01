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
    local type="$1"
    get_user_connect_info_ip "${type}"
    get_user_connect_info_port "${type}"
    get_user_connect_info_username "${type}"
}

password_free_check(){
    local user=$1
    local ip=$2
    local port=$3
    get_local_ip ${ip}
    if [ $? == 1 ]; then
        echo "Don't need config rsa."
    else
        check_password_free_status ${user}@${ip} ${port}
        if [[ $? != 0 ]];then
            create_secret_key
            if [[ $? != 0 ]];then
                echo -e "\e[1;33mManually configure password-free on the target server or run the ${current_dir}/config_passwd_free.sh on the target server.\e[0m"
                exit 
            fi
            config_password_free ${user} ${ip} ${port}  
            if [[ $? != 0 ]];then
                echo -e "\e[1;33mManually configure password-free on the target server or run the ${current_dir}/config_passwd_free.sh on the target server.\e[0m"
                exit
            fi  
        fi
    fi
}

get_system_network_interface(){
  # the network port that supports transmission
   local user=$1
   local ip=$2
   local port=$3
  network_interface_list=$(ssh -p ${port} $user@$ip ip a | grep -v lo | awk -F": " '/^[^ ]/{print$2}')
  echo -e "\e[1;34mSelect the network port that supports transmission from the following list:\e[0m"
  i=1
  for network_interface in $network_interface_list;do
    echo $i ${network_interface}
    ((i++))
  done
  len_network_interface_list=$(echo ${network_interface_list} | awk -F ' ' '{print NF}')
  while true
  do
    echo -n 'Enter the sequence number from the network interface list: '
    read -r network_number
    
    if echo $network_number | grep -E '^[0-9]+$' >/dev/null;then
        if [[ $network_number -le ${len_network_interface_list} ]] && [[ $network_number -ge 1 ]];then
            user_choose_network_interface=$(echo ${network_interface_list} | cut -d ' ' -f $network_number)
            break
        fi
    fi
    echo -e "\e[1;31mIncorrect input information.\e[0m"
    continue
  done
  echo -e "\e[1;32mThe selected network interface is ${user_choose_network_interface}.\e[0m"
}

get_vm_eth0_ip(){
    local user=$1
    local ip=$2
    local port=$3
    network_ip=$(ssh -p ${port} $user@$ip ip -4 addr show ${user_choose_network_interface} | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
}
# check env
echo -e "\e[1;34mStart to check the controll node environment ...\e[0m"
python3 ${current_dir}/../util/check_env_controll.py 'local'
if [[ $? == 0 ]];then
    echo -e "\e[1;32mCheck the the controll node environment.\e[0m"
else
    echo -e "\e[1;31mFailed to check the controll node environment.\e[0m"
    exit 1
fi

get_user_connect_info 'second host'
get_local_ip ${input_ip}
if [[ $? == 1 ]];then
   echo -e "\e[1;32mThe IP addresses of the two active nodes are the same. You do not need to check the environment again. \e[0m"
else
    sed -i "s#port_host_second=.*#port_host_second=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
    sed -i "s#username_host_first=.*#username_host_first=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
    sed -i "s#ip_host_first=.*#ip_host_first=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
    password_free_check ${input_username} ${input_ip} ${input_port} 
    python3 ${current_dir}/../util/check_env_controll.py 'remote' 
    if [[ $? == 0 ]];then
        echo -e "\e[1;32mCheck the environment of the second host node.\e[0m"
    else
        echo -e "\e[1;31mFailed to check the environment of the second host node.\e[0m"
        exit 1
    fi
fi


echo ''
echo -e "\e[1;33mEnsure that the IP address of the management port that can provide the bandwidth for the VM is SSH\e[0m"
echo -e "\e[1;33mIf no configuration is performed, run the following command to configuration:\e[0m"
echo -e "\e[1;33mexample: virsh attach-interface vm1 --type bridge --source virbr0 --model virtio --current\e[0m"
echo ''


echo -e "\e[1;34mStarted to check first virtual machine server...\e[0m"
get_user_connect_info 'first virtual'
sed -i "s#port_compute_first=.*#port_compute_first=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_first=.*#username_compute_first=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_first=.*#ip_compute_first=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
get_system_network_interface ${input_username} ${input_ip} ${input_port}
get_vm_eth0_ip ${input_username} ${input_ip} ${input_port}
sed -i "s#eth0_ip_first=.*#eth0_ip_first=${eth0_ip}#g" $current_dir/../conf/demo_conf.cfg 


echo -e "\e[1;34mStarted to check second virtual machine server...\e[0m"
get_user_connect_info 'second virtual'
sed -i "s#port_compute_second=.*#port_compute_second=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_second=.*#username_compute_second=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_second=.*#ip_compute_second=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
get_system_network_interface ${input_username} ${input_ip} ${input_port}
get_vm_eth0_ip ${input_username} ${input_ip} ${input_port}
sed -i "s#eth0_ip_second=.*#eth0_ip_second=${eth0_ip}#g" $current_dir/../conf/demo_conf.cfg 

# Check vm1 ping vm2
python3 ${current_dir}/../util/check_env_compute.py  'compute_first'
if [[ $? == 0 ]];then
    echo -e "\e[1;32m  VM1 and VM2 networks are connected.\e[0m"
else
    echo -e "\e[1;31m  VM1 and VM2 networks can't be connected.\e[0m"
    exit 1
fi

# Check vm2 ping vm1
python3 ${current_dir}/../util/check_env_compute.py 'compute_second'
if [[ $? == 0 ]];then
    echo -e "\e[1;32m  VM2 and VM1 networks are connected.\e[0m"
else
    echo -e "\e[1;31m  VM2 and VM1 networks can't be connected.\e[0m"
    exit 1
fi

echo -e "\e[1;33mIt may take a few minutes to run the ICMP uninstallation demo, please wait...\e[0m"
# Simulating traffic sending between VMs takes a long time.
python3 ${current_dir}/../util/deal_demo.py
if [[ $? == 0 ]];then
  echo -e "\e[1;32mThe demo execution is complete.\e[0m"
fi
