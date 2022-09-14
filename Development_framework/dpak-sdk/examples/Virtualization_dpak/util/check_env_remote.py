import re
from subprocess import STARTF_USESTDHANDLES
from common_util import subprocess_command, common_result_check_contain, common_result_check_not_contain


class CheckDemoEnvironme:

    def __init__(self, ip=None, username=None, port=None):
        self.ssh_command = 'ssh -p {} {}@{}'.format(port, username, ip)
        self.port = port
        self.ip = ip
        self.username = username
    
    @staticmethod
    def check_result_contain(check_item, software, ssh_command, function):
        value = subprocess_command(ssh_command)
        status, check_result = function(software, value, check_item)
        print(check_result)
        return status, check_result

    def check_local_os(self):
        check_item = ['openEuler release 20.03 (LTS-SP1)']
        software = "os_system"
        ssh_command = ['cat', '/etc/system-release']
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The os-release is openEuler release 20.03 (LTS-SP1) .")
        return status,check_result
    
    def check_remote_os(self):
        """check os"""
        check_item = ['openEuler release 20.03 (LTS-SP1)']
        software = "os_system"
        ssh_command = self.ssh_command.split(' ')
        ssh_command.append('cat /etc/system-release')
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The os-release is openEuler release 20.03 (LTS-SP1). ")
        return status, check_result

    def check_cat_fstab(self):
        """huge mount"""
        check_item = ["/mnt/huge"]
        software = "huge"
        ssh_command = self.ssh_command.split(' ')
        ssh_command.append('cat /etcfstab')
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The hugepage is config success.")
        return status,check_result

    def check_status_firewalld(self):
        check_item = ["incative"]
        software = "firewalld"
        ssh_command = ['systemctl', 'status', 'firewalld']
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The firewalld status is inactive.")
        return status,check_result

    def check_status_remote_firewalld(self):
        check_item = ["incative"]
        software = "firewalld"
        ssh_command = self.ssh_command.split(' ')
        ssh_command.append('systemctl status firewalld')
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("  The firewalld status is inactive. ")                                                         
        return status,check_result

    
    def check_status_hinic3(self):
        check_item = ["hisdk3", "mgmt_migration_","hiovs3","hinic3"]
        software = "hinic3"
        ssh_command = self.ssh_command.split(' ')
        ssh_command.append('lsmod| grep hinic3')
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("   the SP680 OVS and live migration drivers are installed.")
        return status,check_result

    def check_openstack_neutron(self):
        check_item = ['X']
        software = "neutron"
        ssh_command = ['openstack', 'network agent list']
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("   Neutron configurations are normal.")
        return status,check_result
    
    def check_openstack_nova(self):
        check_item = ['disabled']
        software = "nova"
        ssh_command = ['openstack', 'compute service list']
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("   the Nova configurations is normal.")
        return status,check_result
    
    def check_openstack_ceph(self):
        check_item = ['disabled']
        software = "ceph"
        ssh_command = ['openstack', 'volume service list']
        status, check_result = self.check_result_contain(check_item, software,ssh_command,
                                                         common_result_check_contain)
        if status:
            print("   the ceph configurations is normal.")
        return status,check_result

    def check_openstack_version(self):
        ssh_command = ["openstack", '--version']
        value = subprocess_command(ssh_command)
        pat = re.compile(r'\d+')
        echo = pat.findall(value)
        status = True
        check_result = "openstack version is ok."
        if echo[0] == "3":
            if echo[1] >= '19':
                return status, check_result
        elif echo[0] == '4':
            if echo[1] == '0':
                if echo[2] <= '2':
                    return status, check_result
        else:
            status = False
            check_result = "openstack version is not ok."
            print(check_result)
            return status, check_result

    def ovs_vsctl_show(self):
        check_item = ['br-dpdk', 'br-tun', 'br-int']
        software = 'ovs'
        ssh_command = self.ssh_command.split(' ')
        ssh_command.append('ovs-vsctl show')
        status, check_result = self.check_result_contain(check_item, software, ssh_command, common_result_check_contain)

        if status:
            print("   the ovs configuration is normal.")
        return status, check_result