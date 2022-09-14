#!/usr/bin/bash
current_dir=$(cd "$(dirname "$0")";pwd)
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
    ssh ${user_ip} -p ${port}
    if [ $? ==0 ];then
        echo 'password free is ok'
    else
        echo 'password free is not ok'
        exit 1
    fi
}

storage=$1
compute_first=$2
compute_second=$3
storage_list=($echo ${storage} | tr ',' ''))
storage_list=($echo ${storage} | tr ',' ''))
storage_list=($echo ${storage} | tr ',' ''))



