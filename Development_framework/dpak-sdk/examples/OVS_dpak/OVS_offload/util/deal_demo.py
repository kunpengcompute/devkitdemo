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

import os
import sys
import re
import time
from multiprocessing import Process
from configparser import ConfigParser
from common_util import subprocess_command



class DealDemo:
    """
    1.ping the two VMS from each other.
    2.check whether the flow table is uninstalled.
    3.keep pinging the flow table and check whether the number of packets in the flow table keeps increasing.
    """
    def __init__(self, ip=None, username=None,port=None):
        self.ssh_command = 'ssh -p {} {}@{}'.format(port,username,ip)
        self.port = port
        self.ip = ip

    def ip2_ping_ip1(self,ip):
        ssh_command = self.ssh_command.split(' ')
        ssh_command.append('ping {}'.format(ip))
        subprocess_command(ssh_command)

    def uninstall_icmp(self):
        """ICMP protocol offload """
        check_result = 'Failed to perform the ICMP protocol offload.'
        status = False
        ssh_command = ['ovs-appctl' ,'hwoff/dump-hwoff-flows']
        value = subprocess_command(ssh_command)
        nums_1 = re.findall(r'packets\(\d+\)',value)
        if nums_1:
            num_1 = re.findall(r'\d+',nums_1[0])[0]
        else:
            return status, check_result
        time.sleep(5)
        ssh_command = ['ovs-appctl' ,'hwoff/dump-hwoff-flows']
        value = subprocess_command(ssh_command)
        nums_2 = re.findall(r'packets\(\d+\)',value)
        if nums_2:
            num_2 = re.findall(r'\d+',nums_2[0])[0]
        else:
            return status, check_result
        if num_2 > num_1:
            check_result = 'ICMP protocol offload successfully executed.'
            status = True
        return status, check_result


def check_demo_server(ip,username,port,eth0_ip):
    deal_demo = DealDemo(ip, username, port)
    deal_demo.ip2_ping_ip1(eth0_ip)


if __name__ == "__main__":
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))), 
                          "conf/demo_conf.cfg"))
    # compute information
    port_compute_first = cfg['compute_first'].get('port_compute_first')
    username_compute_first = cfg['compute_first'].get('username_compute_first')
    ip_compute_first = cfg['compute_first'].get('ip_compute_first')
    eth0_ip_first =  cfg['compute_first'].get('eth0_ip_first')
    port_compute_second = cfg['compute_second'].get('port_compute_second')
    username_compute_second = cfg['compute_second'].get('username_compute_second')
    ip_compute_second = cfg['compute_second'].get('ip_compute_second')
    eth0_ip_second =  cfg['compute_second'].get('eth0_ip_second')
    process_list = []
    process_1 = Process(target=check_demo_server,args=(ip_compute_first,username_compute_first,port_compute_first,eth0_ip_second))
    process_2 = Process(target=check_demo_server,args=(ip_compute_second,username_compute_second,port_compute_second,eth0_ip_first))
    process_list.append(process_1)
    process_list.append(process_2)
    for process in process_list:
        process.start()
    deal_demo = DealDemo()
    status = False
    check_result = 'timeout'
    for i in range(120):
        status, check_result = deal_demo.uninstall_icmp()
        if status:
            break
        time.sleep(1)
    for process in process_list:
        process.terminate()
    print(check_result)
    if status:
        sys.exit(0)
    sys.exit(1)
