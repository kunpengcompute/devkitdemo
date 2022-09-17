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
        self.net1 = net_first
        self.net2 = net_second
        self.bond_ip = bond_ip
        self.host_name1 = host_name_first

    def check_virtual_first_config_status(self):
        check_item = ["ACTIVE"]
        software = "virtual1"
        ssh_command = ["openstack", "server", "show", "{}".format(self.vm_id_first)]
        value = subprocess_command(ssh_command)
        status, check_result = common_result_check_not_contain(software, value, check_item)
        return status, check_result
        
    def reboot_virtual(self):
        ssh_command = ['openstack','server', 'reboot', '{}'.format(self.vm_id_first)]
        subprocess_command(ssh_command)
        status, check_result = self.check_virtual_first_config_status()
        print(check_result)
        if status:
            print("虚拟机生命周期管理重启成功")
        return status, check_result

    def openstack_server_migrate(self):
        print("热迁移虚拟机2")
        ssh_command = ['openstack','server migrate {} --live-migraation --host {} --shared-migration' \
            '--os-compute-api-version 2.30'.format(self.vm_id_second,self.host_name1)]
        subprocess_command(ssh_command)
        ssh_command = ['openstack', 'server', 'show', '{}'.format(self.vm_id_second)]
        value = subprocess_command(ssh_command)
        if value == 'fail':
            return False, 'migrate failed'
        if "ACTIVE" in value:
            print("热迁移虚拟机vm2 success")
            return True, 'migrate success'

    def nova_interface_detach(self):
        print("虚拟机进行热拔")
        ssh_command = ['nova','interface-detach', '{}'.format(self.vm_id_first), '{}'.format(self.port_id_first)]
        subprocess_command(ssh_command)
        ssh_command = ['openstack',"port", "show", "{}".format(self.port_id_first)]
        value = subprocess_command(ssh_command)
        if value == 'fail':
            return False, 'migrate failed'
        if "DOWN" in value:
            print("  虚拟机进行热拔成功")
            return True, 'migrate success'
        else:
            print("  虚拟机进行热拔失败")
            return False, 'migrate failed'


    def nova_interface_attach(self):
        print("虚拟机进行热插")
        ssh_command = ['nova','interface-attach', '{}'.format(self.vm_id_first), '--port-id={}'.format(self.port_id_first)]
        subprocess_command(ssh_command)
        ssh_command = ['openstack',"port", 'show', "{}".format(self.port_id_first)]
        value = '虚拟机进行热插失败'
        for i in range(10):
            value = subprocess_command(ssh_command)
            time.sleep(5)
            if "ACTIVE" in value:
                break
        if "ACTIVE" in value:
            print("  虚拟机进行热插成功")
            return True, 'migrate success'
        else:
            print("  虚拟机进行热插失败")
            return False, 'migrate failed'
    
    def check_virtual_first_status(self):
        """检查热迁移后虚拟机1的状态"""
        print("验证迁移后虚拟机1的状态")
        ssh_command = ['openstack','server', 'show', '{}'.format(self.vm_id_second)]
        value = subprocess_command(ssh_command)
        if value == 'fail':
            return False, 'mgigrate faild'
        if host_name_second in value:
            return True, 'mgigrate success'

    def ip2_ping_ip1(self):
        ssh_command = self.ssh_command.split(" ")
        ssh_command.append('ping {}'.format(self.bond_ip))
        value = subprocess_command(ssh_command)
        if value == 'fail':
            return False, 'ping ip failed'
        return True, 'ping ip success'

    def check_net_through(self):
        net = ''
        ssh_command = self.ssh_command.split(' ')
        ssh_command.append('tcpdump -i {} -ne | grep -i ICMP'.format(self.net1))
        value = subprocess_command(ssh_command)
        if value == 'fail':
            return False, value
        if self.bond_ip in value:
            ssh_command = [self.ssh_command, 'ifconfig {} down'.format(self.net1)]
            subprocess_command(ssh_command)

            ssh_command = [self.ssh_command, 'tcpdump -i {} -ne|grep -i ICMP'.format(self.net2)]
            net = subprocess_command(ssh_command)
            if net == 'fail':
                return False, value
            ssh_command = [self.ssh_command, 'ifconfig {} up'.format(self.net1)]
            subprocess_command(ssh_command)
        ssh_command = [self.ssh_command, 'tcpdump -i {} -ne|grep -i ICMP'.format(self.net2)]
        value = subprocess_command(ssh_command)
        if value == 'fail':
            return False, value
        if self.bond_ip in value:
            ssh_command = [self.ssh_command, 'ifconfig {} down'.format(self.net2)]
            subprocess_command(ssh_command)
            ssh_command = [self.ssh_command, 'tcpdump -i {} -ne|grep -i ICMP'.format(self.net1)]
            net = subprocess_command(ssh_command)
            if net == 'fail':
                return False, value
            ssh_command = [self.ssh_command, 'ifconfig {} up'.format(self.net1)]
            subprocess_command(ssh_command)
        if self.bond_ip in net.lower():
            return True, '备用网络口有流量通过'
        return False, '备用网络口没有流量通过'


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
    net_first = cfg.get('compute_first', 'net_first')
    net_second = cfg.get('compute_first', 'net_second')
    bond_ip = cfg.get('compute_first', 'bond_ip')


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
