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

import sys

from check_env_remote import CheckDemoEnvironment


def check_control_server():
    """
    Verifying the Control Node Environment
    1.Ensure that the operating system version.
    2.Ensure that the firewall is disabled.
    3.Ensure that the neutron is deployed.
    4.Ensure that the nova is deployed.
    5.Ensure that the cephis deployed.
    6.Ensure that the openstack version.
    """

    control = CheckDemoEnvironment()
    check_list = {
        control.check_local_os:(),
        control.check_status_firewalld:(),
        control.check_openstack_neutron:(),
        control.check_openstack_nova:(),
        control.check_openstack_ceph:(),
        control.check_openstack_version:()
    }
    for function, args in check_list.items():
        status, result = function(*args)
        if not status:
            break
    return status, result


if __name__ == "__main__":

    status, result = check_control_server()
    if status:
        sys.exit(0)
    sys.exit(1)
