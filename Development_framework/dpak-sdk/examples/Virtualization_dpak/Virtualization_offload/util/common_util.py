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
import subprocess
from configparser import ConfigParser


def subprocess_command(ssh_command):
    try:
        proc = str(subprocess.Popen(ssh_command, shell=False, stdout=subprocess.PIPE,stderr=subprocess.PIPE).stdout.read(),
                    encoding='utf-8').strip()
    except Exception as err:
        return str(err)
    return proc

def common_result_check_contain(software, value, check_item):
    result = False
    if value == 'fail':
        message = '{} check fail, {}'.format(software, value)
        return result, message
    
    for item in check_item:
        if item in value:
            message = 'Check {} succeeded'.format(software)
            return True, message
    
    message = 'Failed to check {},{}'.format(software, value)
    return False, message
                

def common_result_check_not_contain(software, value, check_item):
    result = False
    if value == 'fail':
        message = '{} check fail, {}'.format(software, value)
        return result, message
    
    for item in check_item:
        if item not in value:
            message = 'Check {} succeeded'.format(software)
            return True, message
    
    message = 'Failed to check {},{}'.format(software, value)
    return False, message


class ReadConfig:
    # Obtain the server information by loading the configuration file.
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))), 
                          "conf/demo_conf.cfg"))
    # first compute
    port_compute_first = cfg.get('compute_first', 'port_compute_first')
    username_compute_first = cfg.get('compute_first', 'username_compute_first')
    ip_compute_first = cfg.get('compute_first', 'ip_compute_first')
    vm_first_id = cfg.get('compute_first', 'vm_first_id')
    port_first_id = cfg.get('compute_first', 'port_first_id')
    host_name_first = cfg.get('compute_first', 'host_name_first')
    # second compute
    port_compute_second = cfg.get('compute_second', 'port_compute_second')
    username_compute_second = cfg.get('compute_second', 'username_compute_second')
    ip_compute_second = cfg.get('compute_second', 'ip_compute_second')
    vm_second_id = cfg.get('compute_second', 'vm_second_id')
    port_second_id = cfg.get('compute_second', 'port_second_id')
    host_name_second = cfg.get('compute_second', 'host_name_second')
