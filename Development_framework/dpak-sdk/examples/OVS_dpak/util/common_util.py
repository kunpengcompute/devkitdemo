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

import subprocess

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
            message = '{} is OK'.format(software)
            return True, message
    
    message = '{} is not OK'.format(software)
    return False, message
                

def common_result_check_not_contain(software, value, check_item):
    result = False
    if value == 'fail':
        message = '{} check fail, {}'.format(software, value)
        return result, message
    
    for item in check_item:
        if item  not in value:
            message = '{} is OK'.format(software)
            return True, message
    
    message = '{} is OK'.format(software)
    return False, message