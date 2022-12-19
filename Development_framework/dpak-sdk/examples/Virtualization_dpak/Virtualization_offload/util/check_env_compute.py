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

from configparser import ConfigParser
from check_env_remote import CheckDemoEnvironment


def check_compute_server(ip, username, port):
    """
    Verifying the Compute Node Environment
    1.Ensure that the operating system version.
    2.Ensure that the huge page has been configured.
    3.Ensure that the firewall is disabled.
    4.Ensure that the hinic3 driver has been installed.
    5.Ensure that the OVS is running.
    """

    compute = CheckDemoEnvironment(ip, username, port)
    check_list = {
        compute.check_remote_os:(),
        compute.check_cat_fstab:(),
        compute.check_status_remote_firewalld:(),
        compute.check_status_hinic3:(),
        compute.ovs_vsctl_show:()
    }
    for function, args in check_list.items():
        status, result = function(*args)
        if not status:
            break
    return status, result


if __name__ == "__main__":
    compute_num = sys.argv[1]
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))), 
                          "conf/demo_conf.cfg"))
    # compute server 
    if compute_num == 'compute_first':
        port_compute = cfg.get('compute_first', 'port_compute_first')
        username_compute = cfg.get('compute_first', 'username_compute_first')
        ip_compute = cfg.get('compute_first', 'ip_compute_first')
    else:
        port_compute = cfg.get('compute_second', 'port_compute_second')
        username_compute = cfg.get('compute_second', 'username_compute_second')
        ip_compute = cfg.get('compute_second', 'ip_compute_second')
    status, result = check_compute_server(ip_compute, username_compute, port_compute)
    if status:
        sys.exit(0)
    sys.exit(1)
