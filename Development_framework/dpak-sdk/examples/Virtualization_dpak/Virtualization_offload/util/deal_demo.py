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

import time
from common_util import subprocess_command, common_result_check_contain, ReadConfig


class DealDemo:
    def __init__(self, ip=None, username=None,port=None):
        self.ssh_command = 'ssh -p {} {}@{}'.format(port,username,ip)
        self.port = port
        self.ip = ip
        self.username = username
        self.vm_id_first = ReadConfig.vm_first_id
        self.vm_id_second = ReadConfig.vm_second_id
        self.port_id_first = ReadConfig.port_first_id
        self.port_id_second = ReadConfig.port_second_id
        self.host_name1 = ReadConfig.host_name_first

    @staticmethod
    def check_run_demo_status(ssh_command, check_status):
        """Check the command execution status"""
        value = 'fail'
        for i in range(20):
            value = subprocess_command(ssh_command)
            time.sleep(3)
            if check_status in value:
                break
        return value

    def check_virtual_first_config_status(self):
        check_item = ["REBOOT"]
        software = "virtual1"
        ssh_command = ["openstack", "server", "show", "{}".format(self.vm_id_first)]
        value = self.check_run_demo_status(ssh_command, "REBOOT")
        status, check_result = common_result_check_contain(software, value, check_item)
        return status, check_result
        
    def reboot_virtual(self):
        """VM lifecyle management--restart"""
        ssh_command = ['openstack','server', 'reboot', '{}'.format(self.vm_id_first)]
        subprocess_command(ssh_command)
        status, check_result = self.check_virtual_first_config_status()
        print(check_result)
        if status:
            print("  Start restaring the VM.")
            print("  The VM has been restarted.")
            print("  Succeeded in restarting the VM for lifecyle successfully.")
        return status, check_result

    def openstack_server_migrate(self):
        """live migration the VM"""
        print("Executing the Demo for VM migration demo.")
        ssh_command = ['openstack','server migrate {} --live-migraation --host {} --shared-migration' \
            '--os-compute-api-version 2.30'.format(self.vm_id_second,self.host_name1)]
        subprocess_command(ssh_command)
        # Check the VM status after live migration.
        ssh_command = ['openstack', 'server', 'show', '{}'.format(self.vm_id_second)]
        value = self.check_run_demo_status(ssh_command, "ACTIVE")
        if "ACTIVE" in value:
            print("  VM {} is alive after live migration.".format(self.vm_id_second))
            print("  Succeeded in live migrating the second VM.")
            return True, 'Succeeded in live migrating the second VM.'
        else:
            print("  Failed to live migrate the second VM. The migration timed out.")
            return False, 'Failed to live migrate the second VM. The migration timed out.'

    def nova_interface_detach(self):
        """hot remove the passthrough network port"""
        print("Executing the Demo for hot removing the Passthrough network port.")
        ssh_command = ['nova','interface-detach', '{}'.format(self.vm_id_first), '{}'.format(self.port_id_first)]
        subprocess_command(ssh_command)
        # Check the port status after hot remove.
        ssh_command = ['openstack',"port", "show", "{}".format(self.port_id_first)]
        value = self.check_run_demo_status(ssh_command, "DOWN")
        if "DOWN" in value:
            print("  The passthrough network port {} is down.".format(self.port_id_first))
            print("  Succeeded in hot removing passthrough network port.")
            return True, 'Succeeded in hot removing passthrough network port.'
        else:
            print("  Failed to hot remove the passthrough network port. The hot-removal demo timed out.")
            return False, 'Failed to hot remove the passthrough network port. The hot-removal demo timed out.'

    def nova_interface_attach(self):
        """hot insert the passthrough network port"""
        print("Executing the Demo for hot inserting the Passthrough network port.")
        # Wait for 20 seconds after the VM is hot insert.
        time.sleep(20)
        ssh_command = ['nova','interface-attach', '{}'.format(self.vm_id_first), '--port-id={}'.format(self.port_id_first)]
        subprocess_command(ssh_command)
        # Check the Port status after hot insert.
        ssh_command = ['openstack',"port", 'show', "{}".format(self.port_id_first)]
        value = self.check_run_demo_status(ssh_command, "ACTIVE")
        if "ACTIVE" in value:
            print("  The passthrough network port {} is active.".format(self.port_id_first))
            print("  Succeeded in hot inserting the passthrough network port.")
            return True, 'Succeeded in hot inserting the passthrough network port.'
        else:
            print("  Failed to hot insert the passthrough network port. The hot-insertion demo timed out.")
            return False, 'Failed to hot insert the passthrough network port. The hot-insertion demo timed out.'
