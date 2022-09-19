#!/usr/bin/bash
current_dir=$(cd "$(dirname "$0")";pwd)
ip_pattern="^\([1-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)\.\([0-9]\|[1-9][0-9]\|1[0-9][0-9]\|2[0-4][0-9]\|25[0-5]\)$"

get_local_ip(){
    local input_ip
    local_ips=$(ifconfig -a| grep inet | grep -v 127.0.0.1| grep -v inet6| awk '{print $2}'|tr -d "addr:")
    if [[ ${local_ips} =~ ${input_ip} ]];then
        echo 'You do not need to configure password-free processing on then same computer'
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
    if [ $? == 0 ];then
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
    local type=$1
    local flag=1
    while [[ $flag == 1 ]];do
        echo -n "input your ${type} ip: "
        read -r IP
        if [[ $IP == y ]];then
            read -r  IP
        fi
        if echo ${IP} | grep -v ${ip_pattern} >/dev/null ;then
            echo "The enter IP address is invalid."
            continue
        fi
        if [ -z ${IP} ];then
            continue
        else
            break
        fi
    done
    if [[ $type == 'second_host' ]] || [[ $type == 'first_virtual' ]] || [[ $type == 'second_virtual' ]];then
        input_ip=${IP}
    else
        echo "input error"
        exit 
    fi
}

get_user_connect_info_port(){
    local type=$1
    local flag=1
    while [[ $flag == 1 ]];do
        echo -n  "input your ${type} port: "
        read -r PORT
        if [[ ! "${POrt}" =~ ^[1-9]+$ ]];then
            echo "The enter port is invalid."
            continue
        fi
        if [[ ${PORT} -gt 66535 ]];then
            echo "Enter the correct port number. The port number cannot be greater than 65535"
            continue
        fi
        if [ -z ${PORT} ];then
            continue
        else
            break
        fi
    done
    if [[ $type == 'second_host' ]] || [[ $type == 'first_virtual' ]] || [[ $type == 'second_virtual' ]];then
        input_port=${PORT}
    else
        echo "input error"
        exit
    fi
}

get_user_connect_info_username(){
    local type=$1
    local flag=1
    while [[ $flag == 1 ]];do
        echo -n "input your ${type} username: "
        read -r USERNAME
        if [ -z ${USERNAME} ];then
            continue
        else
            break
        fi
    done
    if [[ $type == 'second_host' ]] || [[ $type == 'first_virtual' ]] || [[ $type == 'second_virtual' ]];then
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
    check_password_free_status ${user}@${ip} ${port}
    if [[ $? != 0 ]];then
        create_pub_cert
        config_password_free ${user} ${ip} ${port}
    fi
}

get_vm_eth0_ip(){
    local user=$1
    local ip=$2
    local port=$3
    eth0_ip=$(ssh -p ${port} $user@$ip ip -4 addr show eth0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
}
# check env
echo "start check controll server..."
python3 ${current_dir}/../util/check_env_controll.py 'local'
if [[ $? == 0 ]];then
    echo 'The controll server env be checked success'
else
    echo 'The controll server env be checked success'
    exit 1
fi

get_user_connect_info 'second_host'
get_local_ip ${input_ip}
if [[ $? == 1 ]];then
   echo "The two host ip is same,don't check env"
else
    sed -i "s#port_host_second=''#port_host_second=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
    sed -i "s#username_host_first=''#username_host_first=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
    sed -i "s#ip_host_first=''#ip_host_first=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
    password_free_check ${input_username} ${input_ip} ${input_port} 
    python3 ${current_dir}/../util/check_env_controll.py 'remote' 
    if [[ $? == 0 ]];then
        echo 'The controll server env be checked success'
    else
        echo 'The controll server env be checked success'
        exit 1
    fi
fi
echo ''
echo 'Ensure that the IP address of the management port that can provide the bandwidth for the VM is SSH'
echo 'If no configuration is performed, run the following command to configuration:'
echo 'example: virsh attach-interface vm1 --type bridge --source virbr0 --model virtio --current'
echo ''
echo "start check first virtual machine server..."
get_user_connect_info 'first_virtual'
sed -i "s#port_compute_first=''#port_compute_first=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_first=''#username_compute_first=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_first=''#ip_compute_first=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
get_vm_eth0_ip ${input_username} ${input_ip} ${input_port}
sed -i "s#eth0_ip_first=''#eth0_ip_first=${eth0_ip}#g" $current_dir/../conf/demo_conf.cfg 


echo "start check second virtual machine server..."
get_user_connect_info 'second_virtual'
sed -i "s#port_compute_second=''#port_compute_second=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_second=''#username_compute_second=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_second=''#ip_compute_second=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
get_vm_eth0_ip ${input_username} ${input_ip} ${input_port}
sed -i "s#eth0_ip_second=''#eth0_ip_second=${eth0_ip}#g" $current_dir/../conf/demo_conf.cfg 

# check vm1 ping vm2
python3 ${current_dir}/../util/check_env_compute.py  'compute_first'
if [[ $? == 0 ]];then
    echo 'The network between VM1 and VM2 is normal and can be pinged'
else
    echo "The network between VM1 and VM2 is disconnected and cannot be pinged"
    exit 1
fi

# check vm2 ping vm1
python3 ${current_dir}/../util/check_env_compute.py 'compute_second'
if [[ $? == 0 ]];then
    echo 'The network between VM2 and VM1 is normal and can be pinged'
else
    echo "The network between VM2 and VM1 is disconnected and cannot be pinged"
    exit 1
fi

python3 ${current_dir}/../util/deal_demo.py
if [[ $? == 0 ]];then
    echo "The demo execution is complete."
fi
