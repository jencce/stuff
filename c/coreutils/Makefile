# Makefile
all: prog

prog: ls arch
ls: ls.c
	@if [ -d /selinux ];\
	then\
		echo "cc -o ls ls.c -DSELINUX -lselinux";\
		cc -o ls ls.c -DSELINUX -lselinux;\
	else\
		echo "cc -o ls ls.c";\
		cc -o ls ls.c;\
	fi

arch: arch.c
	cc arch.c -o arch

debug: ls_debug arch_debug

ls_debug: ls.c
	@if [ -d /selinux ];\
	then\
		echo "cc -g -o ls -DLS_DEBUG -DSELINUX ls.c -lselinux";\
		cc -g -o ls -DLS_DEBUG -DSELINUX ls.c -lselinux;\
	else\
		echo "cc -g -o ls -DLS_DEBUG ls.c";\
		cc -g -o ls -DLS_DEBUG ls.c;\
	fi

arch_debug: arch.c
	cc -g arch.c -o arch

.PHONY: test
test:
	/bin/bash test.sh

.PHONY: lint
lint:
	@type splint > /dev/null 2>&1;\
	if [ $? -e 0 ];\
	then\
		splint ls.c -posix-lib -nullassign | head -n 20;\
	else\
		echo "no lint program found";\
	fi

.PHONY: clean
clean:
	rm -f ls arch a.out
