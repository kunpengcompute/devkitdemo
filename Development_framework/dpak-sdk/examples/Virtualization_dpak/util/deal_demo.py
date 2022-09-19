import os
import sys

from configparser import ConfigParser
import time
from common_util import subprocess_command, common_result_check_contain,common_result_check_not_contain


class DealDemo:
    def __init__(self, ip=None, username=None,port=None):
        self.ssh_command = 'ssh -p {} {}@{}'.format(port,username,ip)
        self.port = port
        self.ip = ip
        self.username = username
        self.vm_id_first = vm_first_id
        self.vm_id_second = vm_second_id
        self.port_id_first = port_first_id
        self.port_id_second = port_second_id
        self.host_name1 = host_name_first

    @staticmethod
    def check_run_demo_status(ssh_command, check_status):
        value = 'fail'
        for i in range(20):
            value = subprocess_command(ssh_command)
            time.sleep(3)
            if check_status in value:
                break
        return value

    def check_virtual_first_config_status(self):
        check_item = ["ACTIVE"]
        software = "virtual1"
        ssh_command = ["openstack", "server", "show", "{}".format(self.vm_id_first)]
        value = self.check_run_demo_status(ssh_command, "ACTIVE")
        status, check_result = common_result_check_not_contain(software, value, check_item)
        return status, check_result
        
    def reboot_virtual(self):
        ssh_command = ['openstack','server', 'reboot', '{}'.format(self.vm_id_first)]
        subprocess_command(ssh_command)
        status, check_result = self.check_virtual_first_config_status()
        print(check_result)
        if status:
            print("  VM Lifecyle Management Restarted successfully")
        return status, check_result

    def openstack_server_migrate(self):
        print("Run the live migration VM 2 demo.")
        ssh_command = ['openstack','server migrate {} --live-migraation --host {} --shared-migration' \
            '--os-compute-api-version 2.30'.format(self.vm_id_second,self.host_name1)]
        subprocess_command(ssh_command)
        ssh_command = ['openstack', 'server', 'show', '{}'.format(self.vm_id_second)]
        value = self.check_run_demo_status(ssh_command, "ACTIVE")
        if "ACTIVE" in value:
            print("  VM2 live migrated successfully")
            return True, 'migrate success'
        else:
            print("  Failed to live migrated VM2")
            return False, 'migrate faild'

    def nova_interface_detach(self):
        print("Run the hot removed VM 1 demo.")
        ssh_command = ['nova','interface-detach', '{}'.format(self.vm_id_first), '{}'.format(self.port_id_first)]
        subprocess_command(ssh_command)
        ssh_command = ['openstack',"port", "show", "{}".format(self.port_id_first)]
        value = self.check_run_demo_status(ssh_command, "DOWN")
        if "DOWN" in value:
            print("  The VM is hot removed successfully")
            return True, 'migrate success'
        else:
            print("  Failed to hot remove the VM")
            return False, 'migrate failed'


    def nova_interface_attach(self):
        print("Run the hot-insert VM 1 demo.")
        ssh_command = ['nova','interface-attach', '{}'.format(self.vm_id_first), '--port-id={}'.format(self.port_id_first)]
        subprocess_command(ssh_command)
        ssh_command = ['openstack',"port", 'show', "{}".format(self.port_id_first)]
        value = self.check_run_demo_status(ssh_command, "ACTIVE")
        if "ACTIVE" in value:
            print("  The VM is hot-inserted successfully.")
            return True, 'migrate success'
        else:
            print("  Failed to hot-insert the VM")
            return False, 'migrate failed'
    
    def check_virtual_first_status(self):
        """Check the status of VM1 after the live migration"""
        print("Check the status of VM1 after the live migration")
        ssh_command = ['openstack','server', 'show', '{}'.format(self.vm_id_second)]
        value = subprocess_command(ssh_command)
        if value == 'fail':
            return False, 'mgigrate faild'
        if host_name_second in value:
            return True, 'mgigrate success'


def check_demo_server(ip,username,port):
    deal_demo = DealDemo(ip, username, port)
    check_list = {
        deal_demo.reboot_virtual:(),
        deal_demo.openstack_server_migrate:(),
        deal_demo.check_virtual_first_status: (),
        deal_demo.nova_interface_detach:(),
        deal_demo.nova_interface_attach:()
    }
    for function, args in check_list.items():
        status, result = function(*args)
        if not status:
            break
    return status, result

    

if __name__ == "__main__":
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))),"conf/demo_conf.cfg"))
    # compute
    port_compute_first = cfg.get('compute_first', 'port_compute_first')
    username_compute_first = cfg.get('compute_first', 'username_compute_first')
    ip_compute_first = cfg.get('compute_first', 'ip_compute_first')
    vm_first_id = cfg.get('compute_first', 'vm_first_id')
    port_first_id = cfg.get('compute_first', 'port_first_id')
    host_name_first = cfg.get('compute_first', 'host_name_first')


    port_compute_second = cfg.get('compute_second', 'port_compute_second')
    username_compute_second = cfg.get('compute_second', 'username_compute_second')
    ip_compute_second = cfg.get('compute_second', 'ip_compute_second')
    vm_second_id = cfg.get('compute_second', 'vm_second_id')
    port_second_id = cfg.get('compute_second', 'port_second_id')
    host_name_second = cfg.get('compute_second', 'host_name_second')
    status, result = check_demo_server(ip_compute_first,username_compute_first,port_compute_first)
    print(result)
    if status:
        sys.exit(0)
    sys.exit(1)
