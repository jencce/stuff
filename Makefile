# Makefile

all: prog

prog: ls.c
	cc -o ls ls.c

debug: ls.c
	cc -g -o ls -DLS_DEBUG ls.c

.PHONY: clean
clean:
	rm -f ls
