#!/bin/sh
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

# compile
current_dir=$(cd $(dirname $0); pwd)

cd $current_dir/../CA
mkdir build
cd build
cmake ..
make

# install
lib_name="libsecret-vote-ca.so"
cd ..
if [ ! -d "../lib" ]; then
  mkdir -p ../lib
fi

rm -rf ../lib${lib_name}
cp build/${lib_name} ../lib/

# clean
rm -rf build
