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
RELEASE=202011

CURDIR = ${shell pwd}
FLAGS = -g -DDEBUG -W -Wall -fPIC -std=gnu99 -march=armv8-a -fsigned-char
GLIBS = -lm -lrt -pthread -lz

PATK: file_lock.o ksw.o interface.o 
	gcc -o PATK $(FLAGS) ./file_lock.o ./ksw.o ./interface.o $(GLIBS)

file_lock.o: file_lock.c
	gcc -o file_lock.o $(FLAGS) -c file_lock.c

ksw.o: ksw.c
	gcc -o ksw.o $(FLAGS) -c ksw.c

interface.o: interface.s
	@gcc -c ./interface.s

clean:
	@rm -rf *.o PATK
