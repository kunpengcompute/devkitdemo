#!/usr/bin/bash

# 执行python脚本运行demo
current_dir=$(cd "$(dirname "$0")";pwd)
ip_pattern="^\([1-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)$"

get_local_ip(){
    local input_ip=$1
    local_ips=$(ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d "addr:")
    if [[ ${local_ips} =~ ${input_ip} ]]; then
        echo '在同一个机器上不需要配免密操作'
        return 1
    fi
}

create_pub_cert(){
    if [[ -f ${HOME}/.ssh/id_rsa.pub && -f ${HOME}/.ssh/id_rsa ]];then
        echo 'the rsa exits'
    else
        ssh-keygen -t rsa
        if [ $? != 0 ];then
            echo 'create rsa faild'
            exit 1
        fi
    fi
}

config_password_free(){
    local user=$1
    local ip=$2
    local port=$3
    ssh-copy-id -i ${HOME}/.ssh/id_rsa.pub $user@$ip -p $port
    if [ $? == 0 ]; then
        echo 'config password free success'
    else
        echo 'config password free faild'
        exit 1
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
    # 获取用户信息
    local type=$1
    local flag=1
    while [[ ${flag} == 1 ]]; do
        echo -n "input your ${type} ip: "
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

    if [[ ${type} == 'storage' || ${type} == 'compute_first' || ${type} == 'compute_second' ]]; then
        input_ip=${IP}
    else
        echo "input error"
        exit
    fi
}

get_user_connect_info_port(){
    # 获取用户信息
    local type=$1
    # ip
    local flag=1
    while [[ $flag == 1 ]]; do
        echo -n "input your ${type} port: "
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
    if [[ ${type} == 'storage' || ${type} == 'compute_first' || ${type} == 'compute_second' ]]; then
        input_port=${PORT}
    else
        echo "input error"
        exit
    fi
}

get_user_connect_info_username(){
    local type=$1
    # ip
    local flag=1
    while [[ $flag == 1 ]]; do
        echo -n "input your ${type} username: "
        read -r USERNAME
        if [ -z ${USERNAME} ]; then
            ((i++))
            continue
        else
            break
        fi
    done

    if [[ ${type} == 'storage' || ${type} == 'compute_first' || ${type} == 'compute_second' ]]; then
        input_username=${USERNAME}
    else
        echo "input error"
        exit
    fi
}

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
        echo "don't need config rsa"
    else
        check_password_free_status ${user}@${ip} ${port}
        if [[ $? != 0 ]];then
            create_pub_cert
            config_password_free ${user} ${ip} ${port}
        fi
    fi
}

# check env
echo "start check controll server..."
python3 ${current_dir}/../util/check_env_controll.py
if [[ $? == 0 ]];then
    echo 'The controll server env be checked success'
else
    echo 'The controll server env be checked success'
    exit 1
fi

echo "start check storage server..."
get_user_connect_info 'storage'
sed -i "s#port_storage=''#port_storage=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_storage=''#username_storage=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_storage=''#ip_storage=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
python3 ${current_dir}/../util/check_env_storage.py
if [[ $? == 0 ]];then
    echo 'The storage server env be checked success'
else
    echo 'The storage server env be checked success'
    exit 1
fi

echo "start check compute_first server..."
get_user_connect_info 'compute_first'
sed -i "s#port_compute_first=''#port_compute_first=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_first=''#username_compute_first=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_first=''#ip_compute_first=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
python3 ${current_dir}/../util/check_env_compute.py 'compute_first'
if [[ $? == 0 ]];then
    echo 'The compute_first server env be checked success'
else
    echo 'The compute_first server env be checked success'
    exit 1
fi

echo "start check compute_second server..."
get_user_connect_info 'compute_second'
sed -i "s#port_compute_second=''#port_compute_second=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_second=''#username_compute_second=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_second=''#ip_compute_second=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
python3 ${current_dir}/../util/check_env_compute.py 'compute_second'
if [[ $? == 0 ]];then
    echo 'The controll server env be checked success'
else
    echo 'The controll server env be checked success'
    exit 1
fi

nova_service_list(){
    host_names_1=$(openstack compute service list| grep 'nova-compute'| head -n 1| awk -F "|" '{print $4}' )
    host_name1=$(echo ${host_names_1}|sed s/[[:space:]]//g)
    sed -i "s#host_name_first=''#host_name_first=${host_name1}#g" $current_dir/../conf/demo_conf.cfg
    host_names_2=$(openstack compute service list| grep 'nova-compute' | tail -n 1| awk -F "|" '{print $4}')
    host_name2=$(echo ${host_names_2}|sed s/[[:space:]]//g) 
    sed -i "s#host_name_second=''#host_name_second=${host_name2}#g" $current_dir/../conf/demo_conf.cfg 

}

nova_service_list

create_vm(){
    local port_name=$1
    local vm_name=$2
    local ip=$3
    port_id=$(openstack port create --network test --vnic-type direct --binding-profile '{"capabilities": "switchdev","hw_type": "direct","max_queues": "1","n_rxq": "1"}' ${port_name} | grep "| id" | awk -F "|" '{print $3}')
    if [[ $? == 0 ]]; then
        port_id=$(echo ${port_id}|sed s/[[:space:]]//g)
        vm_id=$(openstack server create --image centos --flavor jen_flavor --nic port-id=${port_id} --availability-zone nova:${ip} ${vm_name} | grep "| id" | awk -F "|" '{print $3}')
        if [[ $? == 0 ]];then
            vm_id=$(echo ${vm_id}|sed s/[[:space:]]//g)
            echo "${vm_name} create success"
        else
            echo "${vm_name} create faild"
            exit 1
        fi
    fi
}

create_vm "port1" 'vm01' ${host_name1}
vm_id=$(echo ${vm_id}|sed s/[[:space:]]//g )
sed -i "s#port_first_id=''#port_first_id=${port_id}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#vm_first_id=''#vm_first_id=${vm_id}#g" $current_dir/../conf/demo_conf.cfg 

create_vm "port2" 'vm02' ${host_name2}
vm_id=$(echo ${vm_id}|sed s/[[:space:]]//g )
sed -i "s#port_second_id=''#port_second_id=${port_id}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#vm_second_id=''#vm_second_id=${vm_id}#g" $current_dir/../conf/demo_conf.cfg 

# 创建虚拟机后要等待
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
        echo "Failed to create the VM"
        exit 1
    fi
done

python3 ${current_dir}/../util/deal_demo.py
if [[ $? == 0 ]];then
    echo "The demo execution is complete."
fi
