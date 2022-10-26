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
    get_user_connect_info_ip  "${type}"
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
                echo -e "\e[1;33mManually configure password-free or run the ${current_dir}/config_passwd_free.sh on the target server.\e[0m"
                exit 
            fi
            config_password_free ${user} ${ip} ${port}  
            if [[ $? != 0 ]];then
                echo -e "\e[1;33mManually configure password-free or run the ${current_dir}/config_passwd_free.sh on the target server.\e[0m"
                exit
            fi  
        fi
    fi
}

get_system_network(){
  # The network list is displayed, prompting users to select a network.
  network_list=$(openstack network list | grep -v "Name" | grep '|' | awk -F '|' '{print $3}')
  echo -e "\e[1;34mSelect a network from the list below.\e[0m"
  i=1
  for network in $network_list;do
    echo $i ${network}
    ((i++))
  done
  len_network_list=$(echo ${network_list} | awk -F ' ' '{print NF}')
  while true
  do
    echo -n 'Enter the sequence number from the network list as the server network: '
    read -r network_number
    
    if echo $network_number | grep -E '^[0-9]+$' >/dev/null;then
        if [[ $network_number -le ${len_network_list} ]] && [[ $network_number -ge 1 ]];then
            user_choose_network=$(echo ${network_list} | cut -d ' ' -f $network_number)
            break
        fi
    fi
    echo -e "\e[1;31mIncorrect input information.\e[0m"
    continue
  done
  echo -e "\e[1;32mThe selected network is ${user_choose_network}.\e[0m"
}

get_system_flavor(){
  # The flavor list is displayed, prompting users to select a flavor.
  flavor_list=$(openstack flavor list | grep -v "Name" | grep '|' | awk -F '|' '{print $3}')
  echo -e "\e[1;34mSelect a flavor from the list below.\e[0m"
  i=1
  for flavor in $flavor_list;do
    echo $i ${flavor}
    ((i++))
  done
  len_flavor_list=$(echo ${flavor_list} | awk -F ' ' '{print NF}')
  while true
  do
    echo -n 'Enter the sequence number from the flavor list as the server flavor: '
    read -r flavor_number
    
    if echo $flavor_number | grep -E '^[0-9]+$' >/dev/null;then
        if [[ $flavor_number -le ${len_flavor_list} ]] && [[ $flavor_number -ge 1 ]];then
            user_choose_flavor=$(echo ${flavor_list} | cut -d ' ' -f $flavor_number)
            break
        fi
    fi
    echo -e "\e[1;31mIncorrect input information.\e[0m"
    continue
  done
  echo -e "\e[1;32mThe selected flavor is ${user_choose_flavor}.\e[0m"
}

get_system_image(){
  # The image list is displayed, prompting users to select a image.
  image_list=$(openstack image list | grep -v "Name" | grep '|' | awk -F '|' '{print $3}')
  echo -e "\e[1;34mSelect an image from the list below.\e[0m"
  i=1
  for image in $image_list;do
    echo $i ${image}
    ((i++))
  done
  len_image_list=$(echo ${image_list} | awk -F ' ' '{print NF}')
  while true
  do
    echo -n 'Enter the sequence number from the image list as the server image: '
    read -r image_number
    
    if echo $image_number | grep -E '^[0-9]+$' >/dev/null;then
        if [[ $image_number -le ${len_image_list} ]] && [[ $image_number -ge 1 ]];then
            user_choose_image=$(echo ${image_list} | cut -d ' ' -f $image_number)
            break
        fi
    fi
    echo -e "\e[1;31mIncorrect input information.\e[0m"
    continue
  done
  echo -e "\e[1;32mThe selected image is ${user_choose_image}.\e[0m"
}

get_system_info(){
  # Interact with the user to select the network, image and flavor.
  get_system_network
  get_system_flavor
  get_system_image
}

nova_service_list(){
    # Obtaining the compute node name.
    host_names_1=$(openstack compute service list| grep 'nova-compute'| head -n 1| awk -F "|" '{print $4}' )
    host_name1=$(echo ${host_names_1}|sed s/[[:space:]]//g)
    sed -i "s#host_name_first=.*#host_name_first=${host_name1}#g" $current_dir/../conf/demo_conf.cfg
    host_names_2=$(openstack compute service list| grep 'nova-compute' | tail -n 1| awk -F "|" '{print $4}')
    host_name2=$(echo ${host_names_2}|sed s/[[:space:]]//g) 
    sed -i "s#host_name_second=.*#host_name_second=${host_name2}#g" $current_dir/../conf/demo_conf.cfg 

}

create_vm(){
    # Obtainning the port id and creating the VM.
    local port_name=$1
    local vm_name=$2
    local ip=$3
    port_id=$(openstack port create --network ${user_choose_network} --vnic-type direct --binding-profile '{"capabilities": "switchdev","hw_type": "direct","max_queues": "1","n_rxq": "1"}' ${port_name} | grep "| id" | awk -F "|" '{print $3}')
    if [[ $? == 0 ]]; then
        port_id=$(echo ${port_id}|sed s/[[:space:]]//g)
        vm_id=$(openstack server create --image ${user_choose_image} --flavor ${user_choose_flavor} --nic port-id=${port_id} --availability-zone nova:${ip} ${vm_name} | grep "| id" | awk -F "|" '{print $3}')
        if [[ $? == 0 ]];then
            vm_id=$(echo ${vm_id}|sed s/[[:space:]]//g)
            echo -e "\e[1;32mSucceeded in creating VM ${vm_name}.\e[0m"
        else
            echo -e "\e[1;31mFailed to create a ${vm_name}.\e[0m"
            exit 1
        fi
    fi
}

delete_vm(){
  local vm_id_list="$1"
  for vm_id in ${vm_id_list[@]};do
    openstack server delete ${vm_id}
    if [[ $? == 0 ]]; then
      echo -e "\e[1;32mSucceeded in deleting the VM ${vm_id}.\e[0m"
    else
      echo -e "\e[1;31mFailed to delete the VM ${vm_id}.\e[0m"
    fi
  done
}

delete_vm_port(){
  local port_id_list="$1"
  for port_id in ${port_id_list[@]};do
    openstack port delete ${port_id}
    if [[ $? == 0 ]]; then
      echo -e "\e[1;32mSucceeded in deleting the VM port ${port_id}.\e[0m"
    else
      echo -e "\e[1;31mFailed to delete the VM port ${port_id}.\e[0m"
    fi
  done
}

clean_env(){
  delete_vm "${vm_id_1} ${vm_id_2}"
  delete_vm_port "${port_1} ${port_2}"
}

# Check the environment configuration of the control node.
echo -e "\e[1;34mStarted to check the control node environment ...\e[0m"
python3 ${current_dir}/../util/check_env_controll.py
if [[ $? == 0 ]];then
    echo -e "\e[1;32mCompleted the environment check of the control node.\e[0m"
else
    echo -e "\e[1;31mFailed to check the environment of the control node.\e[0m"
    exit 1
fi

# Check the environment configuration of the storage node.
echo -e "\e[1;34mStarted to check the storage node environment ...\e[0m"
get_user_connect_info 'storage'
sed -i "s#port_storage=.*#port_storage=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_storage=.*#username_storage=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_storage=.*#ip_storage=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
python3 ${current_dir}/../util/check_env_storage.py
if [[ $? == 0 ]];then
    echo -e "\e[1;32mCompleted the environment check of the storage node.\e[0m"
else
    echo -e "\e[1;31mFailed to check the environment of the storage node.\e[0m"
    exit 1
fi

# Check the environment configuration of the first compute node.
echo -e "\e[1;34mStart to check the environment of the first compute node ...\e[0m"
get_user_connect_info 'first compute'
sed -i "s#port_compute_first=.*#port_compute_first=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_first=.*#username_compute_first=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_first=.*#ip_compute_first=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
python3 ${current_dir}/../util/check_env_compute.py 'compute_first'
if [[ $? == 0 ]];then
    echo -e "\e[1;32mCompleted the environment check of first compute node.\e[0m"
else
    echo -e "\e[1;31mFailed to check the environment of the first compute node.\e[0m"
    exit 1
fi

# Check the environment configuration of the first second compute node.
echo -e "\e[1;34mStart to check the environment of the second compute node ...\e[0m"
get_user_connect_info 'second compute'
sed -i "s#port_compute_second=.*#port_compute_second=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_second=.*#username_compute_second=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_second=.*#ip_compute_second=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
python3 ${current_dir}/../util/check_env_compute.py 'compute_second'
if [[ $? == 0 ]];then
    echo -e "\e[1;32mCompleted the environment check of the second compute node.\e[0m"
else
    echo -e "\e[1;31mFailed to check the environment of the second compute node.\e[0m"
    exit 1
fi

nova_service_list
get_system_info

echo -e "\e[1;34mStarted to create two VMs.\e[0m"
create_vm "port_test_1" 'vm_test_1' ${host_name1}
port_1=${port_id}
vm_id_1=$(echo ${vm_id}|sed s/[[:space:]]//g )
sed -i "s#port_first_id=.*#port_first_id=${port_1}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#vm_first_id=.*#vm_first_id=${vm_id_1}#g" $current_dir/../conf/demo_conf.cfg 

create_vm "port_test_2" 'vm_test_2' ${host_name2}
port_2=${port_id}
vm_id_2=$(echo ${vm_id}|sed s/[[:space:]]//g )
sed -i "s#port_second_id=.*#port_second_id=${port_2}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#vm_second_id=.*#vm_second_id=${vm_id_2}#g" $current_dir/../conf/demo_conf.cfg 
echo -e "\e[1;33mIt may take a few minutes to refresh the VM status, please wait...\e[0m"

# Wait for the VM status to be refreshed after the VM is created.
flag=1
i=0
while [[ $flag == 1 ]]; do
    res=$(openstack server list | grep ${vm_id})
    if [[ $res =~ "ACTIVE" ]]; then
        break
    fi
    sleep 1
    ((i++))
    if [[ $i -gt 20 ]]; then
        echo -e "\e[1;31mFailed to create the VM.\e[0m"
        clean_env
        exit 1
    fi
done

# Live migrating VM.
echo -e "\e[1;34mStarted to execute the demo of the live migrating VM ...\e[0m"
python3 ${current_dir}/../util/deal_demo_hot_migrate_vm.py
if [[ $? == 0 ]];then
  echo -e "\e[1;32mThe demo execution is complete.\e[0m"
fi

# Hot remove the network port.
echo -e "\e[1;34mStarted to execute the demo of hot removing the network port ...\e[0m"
python3 ${current_dir}/../util/deal_demo_detach_vm.py
if [[ $? == 0 ]];then
  echo -e "\e[1;32mThe demo execution is complete.\e[0m"
fi

# Hot insert the network port.
echo -e "\e[1;34mStarted to execute the demo of hot inserting the network port ...\e[0m"
python3 ${current_dir}/../util/deal_demo_attach_vm.py
if [[ $? == 0 ]];then
  echo -e "\e[1;32mThe demo execution is complete.\e[0m"
fi

# Restart VM.
echo -e "\e[1;34mStart to execute the demo of restarting for VM lifecycle management ...\e[0m"
python3 ${current_dir}/../util/deal_demo_reboot_vm.py
if [[ $? == 0 ]];then
  echo -e "\e[1;32mThe demo execution is complete.\e[0m"
fi

# Clear the environment.
clean_env