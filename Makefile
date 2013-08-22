# Makefile

all: prog

prog: ls.c
	cc -o ls ls.c

se: ls.c
	cc -o ls ls.c -DSELINUX -lselinux

debug: ls.c
	cc -g -o ls -DLS_DEBUG ls.c

sedebug: ls.c
	cc -g -o ls -DLS_DEBUG -DSELINUX ls.c -lselinux 

.PHONY: test
test:
	/bin/bash test.sh

.PHONY: clean
clean:
	rm -f ls
