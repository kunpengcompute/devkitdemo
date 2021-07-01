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

prog=func001 func002 func003 func004
cleanfile=hello *.o *.out
CC=gcc
CFLAGS = -Wconversion -Wint-to-pointer-cast

all:${prog}

func001: func001.o
	${CC} ${CFLAGS} -o $@ func001.c

func002: func002.o
	${CC} ${CFLAGS} -o $@ func002.c

func003: func003.o
	${CC} ${CFLAGS} -o $@ func003.c

func004: func004.o
	${CC} ${CFLAGS} -o $@ func004.c

.PHONY: clean
clean:
	rm -f ${cleanfile} ${prog}
