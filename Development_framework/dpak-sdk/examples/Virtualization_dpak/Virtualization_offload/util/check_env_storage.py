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


def check_storage_server(ip, username, port):
    """
    Verifying the storage Node Environment
    1.Ensure that the operating system version.
    2.Ensure that the firewall is disabled.
    """
    storage = CheckDemoEnvironment(ip, username, port)

    check_list = {
        storage.check_remote_os:(),
        storage.check_status_remote_firewalld:(),
    }
    for function, args in check_list.items():
        status, result = function(*args)
        if not status:
            break

    return status, result


if __name__ == "__main__":
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))), 
                          "conf/demo_conf.cfg"))
    port_storage = cfg.get('storage', 'port_storage')
    username_storage = cfg.get('storage', 'username_storage')
    ip_storage = cfg.get('storage', 'ip_storage')

    status, result = check_storage_server(ip_storage, username_storage, port_storage)
    if status:
        sys.exit(0)
    sys.exit(1)
