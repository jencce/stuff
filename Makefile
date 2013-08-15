# Makefile

all: prog

prog: ls.c
	cc -o ls ls.c -lselinux 

debug: ls.c
	cc -g -o ls -DLS_DEBUG ls.c -lselinux 

.PHONY: clean
clean:
	rm -f ls
