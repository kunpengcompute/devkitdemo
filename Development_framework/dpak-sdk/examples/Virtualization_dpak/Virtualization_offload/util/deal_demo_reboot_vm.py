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

from deal_demo import DealDemo
from common_util import ReadConfig


ip_compute_first = ReadConfig.ip_compute_first
username_compute_first = ReadConfig.username_compute_first
port_compute_first = ReadConfig.port_compute_first
deal_demo = DealDemo(ip_compute_first, username_compute_first, port_compute_first)

status, result = deal_demo.reboot_virtual()

if status:
    sys.exit(0)
sys.exit(1)
