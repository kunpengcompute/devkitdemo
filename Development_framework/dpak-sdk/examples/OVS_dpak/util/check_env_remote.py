import re
from common_util import subprocess_command, common_result_check_contain, common_result_check_not_contain


class CheckDemoEnvironme:

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
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The os os openEuler release 20.03 (LTS-SP1)")                                                        
        return status,check_result
    
    def check_cat_fstab(self):
        """huge mount"""
        check_item = ["/mnt/huge"]
        software = "huge"
        ssh_command = ['cat', '/etcfstab']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                        common_result_check_contain)
        if status:
            print("  The hugepage is config success") 
        return status,check_result

    def check_status_firewalld(self):
        check_item = ["incative"]
        software = "firewalld"
        ssh_command = ['systemctl', 'status', 'firewalld']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The firewalld status is incatiive")                                                  
        return status,check_result

    def check_status_hinic3(self):
        check_item = ["hisdk3","hiovs3","hinic3"]
        software = "hinic3"
        ssh_command = ['lsmod']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print('  The SP680 OVS and live migration drivers are installed')
        return status,check_result

    def ovs_vsctl_show(self):
        check_item = ["dpdk","vf"]
        software = "ovs_vsctl_show"
        ssh_command = ['ovs-vsctl','show']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print('  The ovs configuration is normal')
        return status,check_result

    def check_vf_status(self):
        check_item = ["VirtIO"]
        software = "VF device"
        ssh_command = ['hinicadm3','info']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print('  The VF device has been created.')
        return status,check_result
    
    def check_openv_switch_status(self):
        """检查openvswitch服务状态"""
        check_item = ['running']
        software = 'The openswitch service'
        ssh_command = ['service','openvswitch','status']
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command, common_result_check_contain)
        if status:
            print('  The openswitch server is running')
        return status,check_result
    
    def check_vm_ping(self, ip):
        """虚拟机之间互ping连接"""
        check_item = ['1 received']
        software = 'VM ping'
        ssh_command = ['ping','-c 1', ip]
        ssh_command = self.deal_ssh_command(ssh_command,self.ssh_command,self,self.flag)
        status, check_result = self.check_result_contain(check_item, software,ssh_command, common_result_check_contain)
        if status:
            print('  VM1 can ping vm2')
        return status,check_result

