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
    compute = CheckDemoEnvironment(ip, username, port, 'remote')
    status, result = compute.check_vm_ping(eth0_ip)

    return status, result


if __name__ == "__main__":
    compute_num = sys.argv[1]
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "conf/demo_conf.cfg"))
    # compute 
    if compute_num == 'compute_first':
        port_compute = cfg['compute_first'].get('port_compute_first')
        username_compute = cfg['compute_first'].get('username_compute_first')
        ip_compute = cfg['compute_first'].get('ip_compute_first')
        eth0_ip = cfg.get('compute_second','eth0_ip_second')
    else:
        port_compute = cfg['compute_second'].get('port_compute_second')
        username_compute = cfg['compute_second'].get('username_compute_second')
        ip_compute = cfg['compute_second'].get('ip_compute_second')
        eth0_ip = cfg.get('compute_first','eth0_ip_first')
    status, result = check_compute_server(ip_compute,username_compute, port_compute)
    if status:
        sys.exit(0)
    sys.exit(1)
