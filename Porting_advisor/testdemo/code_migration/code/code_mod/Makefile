VERSION=1.0
RELEASE=202011

CURDIR = ${sheLL pwd}
ifdef __x86_64__
	FLAGS = -g -DDEBUG -W -Wall -fPI
else ifdef __aarch64__
	FLAGS = -g -DDEBUG -W -Wall -fPIC -march=armv8-a -fsigned-char
endif
GLIBS = -lm -lrt -pthread -lz

PATK: file_lock.o ksw.o interface.o 
	gcc -o PATK $(FLAGS) ./ file_lock.o ./ksw.o ./interface.o $(GLIBS)

file_lock.o: file_lock.c
	gcc -o file_lock.o -c file_lock.c

ksw.o: ksw.c
	gcc -o ksw.o -c ksw.c

interface.o: interface.s
	@gcc -c ./interface.s

clean:
	@rm -rf *.o PATK