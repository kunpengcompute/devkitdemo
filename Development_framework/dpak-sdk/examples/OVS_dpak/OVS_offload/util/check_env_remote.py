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


from common_util import subprocess_command, common_result_check_contain


class CheckDemoEnvironment:

    def __init__(self, ip=None, username=None,port=None,flag=None):
        self.ssh_command = 'ssh -p {} {}@{}'.format(port,username,ip)
        self.port = port
        self.ip = ip
        self.username = username
        self.flag = flag
    
    @staticmethod
    def check_result_contain(check_item,software,ssh_command,function):
        value = subprocess_command(ssh_command)
        status, check_result = function(software,value,check_item)
        print(check_result)
        return status, check_result

    @staticmethod
    def deal_ssh_command(ssh_cmd,ssh_bash,flag):
        if flag == 'remote':
            ssh_cmd_ = ssh_bash.split(' ')
            ssh_cmd_.extend(ssh_cmd)
        else:
            ssh_cmd_ = ssh_cmd
        return ssh_cmd_

    def check_os(self):
        check_item = ['openEuler release 20.03 (LTS-SP1)']
        software = "os_system"
        ssh_command = ['cat', '/etc/system-release']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The OS is openEuler release 20.03 (LTS-SP1).")                                                        
        return status,check_result
    
    def check_cat_fstab(self):
        """huge mount"""
        check_item = ["/mnt/huge"]
        software = "huge"
        ssh_command = ['cat', '/etc/fstab']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                        common_result_check_contain)
        if status:
            print("  The hugepage is config success") 
        return status,check_result

    def check_status_firewalld(self):
        check_item = ["inactive"]
        software = "firewalld"
        ssh_command = ['systemctl', 'status', 'firewalld']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The firewall status is inactive.")                                                  
        return status,check_result

    def check_status_hinic3(self):
        check_item = ["hisdk3","hiovs3","hinic3"]
        software = "hinic3"
        ssh_command = ['lsmod']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print('  Succeeded in installing the SP680 OVS live migration drivers.')
        return status,check_result

    def ovs_vsctl_show(self):
        check_item = ["dpdk","vf"]
        software = "ovs_vsctl_show"
        ssh_command = ['ovs-vsctl','show']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print('  OVS configurations are normal.')
        return status,check_result

    def check_vf_status(self):
        check_item = ["VirtIO"]
        software = "VF device"
        ssh_command = ['hinicadm3','info']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print('  The VF device has been created.')
        return status,check_result
    
    def check_openv_switch_status(self):
        """check the openvswitch service status"""
        check_item = ['running']
        software = 'The openswitch service'
        ssh_command = ['service','openvswitch','status']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command, common_result_check_contain)
        if status:
            print('  The openswitch server is running')
        return status,check_result
    
    def check_vm_ping(self, ip):
        """ping between VMs"""
        check_item = ['1 received']
        software = 'VM network status'
        ssh_command = ['ping','-c 1', ip]
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command, common_result_check_contain)
        return status,check_result

