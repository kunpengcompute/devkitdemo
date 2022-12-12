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


VIRTIO_DEV_BDF1=""
VIRTIO_DEV_BDF2=""
UP_PORT=""

get_bdf(){
	a=$(hinicadm3 info | grep VirtIO)
	b1=${a%%(*}
	VIRTIO_DEV_BDF1=${b1##*-}

	b2=${a%(*}
	VIRTIO_DEV_BDF2=${b2##*-}
	if [ "${VIRTIO_DEV_BDF1:4:1}" != ":" ] || [ "${VIRTIO_DEV_BDF2:4:1}" != ":" ] || \
    [ "${VIRTIO_DEV_BDF1:7:1}" != ":" ]|| [ "${VIRTIO_DEV_BDF2:7:1}" != ":" ] || \
    [ "${VIRTIO_DEV_BDF1:10:1}" != "." ] || [ "${VIRTIO_DEV_BDF2:10:1}" != "." ];then
		echo "[$(date +%Y-%m-%d\ %H:%M:%S)][ERROR] Get bdf failed. Check pci dev or hinicadm3 tool."
        exit -1
	fi
}

get_up_port(){
    UP_PORT=$(find /sys -name remove | grep ${VIRTIO_DEV_BDF1} | awk -F '/' '{print $(NF-2)}')
}

delete_dev_spec(){
    echo 1 >/sys/bus/pci/devices/$1/remove
    if [ $? -ne 0 ];then 
        echo "[$(date +%Y-%m-%d\ %H:%M:%S)][ERROR] Device [$1] remove err!"
        exit -1
    fi
}

delete_dev(){
    delete_dev_spec ${VIRTIO_DEV_BDF1}
    delete_dev_spec ${VIRTIO_DEV_BDF2}
}

check_dev(){
    ls /sys/bus/pci/devices/$1 >> /dev/null
    if [ $? -ne 0 ];then 
        echo "[$(date +%Y-%m-%d\ %H:%M:%S)][ERROR] Rescan [${UP_PORT}] err, $1 is not detected."
        exit -1
    fi
}

rescan_dev(){
    echo 1 > /sys/bus/pci/devices/${UP_PORT}/rescan
    if [ $? -ne 0 ];then 
        echo "[$(date +%Y-%m-%d\ %H:%M:%S)][ERROR] Rescan [${UP_PORT}] err!"
        exit -1
    fi
    check_dev ${VIRTIO_DEV_BDF1}
    check_dev ${VIRTIO_DEV_BDF2}
}

run_cmd(){
    $1 >> /dev/null
    if [ $? -ne 0 ];then
        echo "[$(date +%Y-%m-%d\ %H:%M:%S)][ERROR] Run cmd [$1] err!"
        exit -1
    else
        echo "[$(date +%Y-%m-%d\ %H:%M:%S)][INFO] $1."
    fi
}

reprobe_driver(){
    run_cmd 'modprobe -r virtio-net'
    run_cmd 'modprobe -r virtio-pci'
    run_cmd 'modprobe virtio-pci'
    run_cmd 'modprobe virtio-net'
}

hinicadm3 virtio_dev -i hinic0 -d net -s 0 -f 0
get_bdf
get_up_port
delete_dev
rescan_dev
reprobe_driver

echo "[$(date +%Y-%m-%d\ %H:%M:%S)][INFO] Finished."