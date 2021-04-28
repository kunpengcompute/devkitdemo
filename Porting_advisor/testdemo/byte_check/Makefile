# Copyright 2020 Huawei Technologies Co., Ltd
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
VERSION=1.0
RELEASE=202104

CURDIR = ${shell pwd}
FLAGS = -g -DDEBUG -W -Wall -fPIC -lstdc++
GLIBS = -lm -lrt -pthread -lz

PATK: bytecheck.o
	gcc -o PATK $(FLAGS) ./bytecheck.o $(GLIBS)

bytecheck.o: bytecheck.cpp
	gcc -o bytecheck.o -c bytecheck.cpp

clean:
	@rm -rf *.o PATK
