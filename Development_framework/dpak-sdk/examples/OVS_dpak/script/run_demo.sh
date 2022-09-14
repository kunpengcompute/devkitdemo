#!/usr/bin/bash
current_dir=$(cd "$(dirname "$0")";pwd)

get_local_ip(){
    local input_ip
    local_ips=$(ifconfig -a| grep inet | grep -v 127.0.0.1| grep -v inet6| awk '{print $2}'|tr -d "addr:")
    if [[ ${local_ips} =~ ${input_ip} ]];then
        echo '在同一个机器上需要配置免密操作'
        return 1
    fi
}

create_pub_cert(){
    if [ -f ${HOME}/.ssh/id_rsa.pub && -f ${HOME}/.ssh/id_rsa ];then
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
    if [[] $res == 1 ]];then
        echo 'password free is ok'
    else
        echo 'password free is not ok'
        return 1
    fi
}

get_user_connect_info_ip(){
    local type=$1
    local flag=1
    while [[ $flag == 1 ]];do
        read -p "input your ${type} ip: " IP
        if [ -Z ${IP} ];then
            ((i++))
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
        read -p "input your ${type} port: " PORT
        if [ -Z ${PORT} ];then
            ((i++))
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
        read -p "input your ${type} username: " USERNAME
        if [ -Z ${USERNAME} ];then
            ((i++))
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
    get_user_connect_info_username ${type}
    get_user_connect_info_ip  ${type}
    get_user_connect_info_port ${type}
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
echo '请确保配置虚拟机可以带宽ssh的管理口ip，然后再按照文档部署业务口'
echo "start check first virtual machine server..."
get_user_connect_info 'first_virtual'
sed -i "s#port_compute_first=''#port_compute_first=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_first=''#username_compute_first=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_first=''#ip_compute_first=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
get_vm_eth0_ip ${input_username} ${input_ip} ${input_port}
sed -i "s#eth0_ip_first=''#eth0_ip_first=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
python3 ${current_dir}/../util/check_env_compute.py  'compute_first'
if [[ $? == 0 ]];then
    echo 'VM1 can ping vm2'
else
    echo "VM1 don't can ping vm2"
    exit 1
fi

echo "start check second virtual machine server..."
get_user_connect_info 'second_virtual'
sed -i "s#port_compute_second=''#port_compute_second=${input_port}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#username_compute_second=''#username_compute_second=${input_username}#g" $current_dir/../conf/demo_conf.cfg 
sed -i "s#ip_compute_second=''#ip_compute_second=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
password_free_check ${input_username} ${input_ip} ${input_port}
get_vm_eth0_ip ${input_username} ${input_ip} ${input_port}
sed -i "s#eth0_ip_second=''#eth0_ip_second=${input_ip}#g" $current_dir/../conf/demo_conf.cfg 
python3 ${current_dir}/../util/check_env_compute.py 'compute_second'
if [[ $? == 0 ]];then
    echo 'VM2 can ping VM1'
else
    echo "VM2 don't can ping VM1"
    exit 1
fi

python3 ${current_dir}/../util/deal_demo.py
if [[ $? == 0 ]];then
    echo "demo 执行完成"
fi





