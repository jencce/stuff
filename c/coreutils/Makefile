# Makefile
progs =  ls arch uname logname hostid yes link unlink whoami

all: $(progs)

$(progs) : % : %.c
	cc $< -o $@

sels: ls.c
	@if [ -d /selinux ];\
	then\
		echo "cc -o ls ls.c -DSELINUX -lselinux";\
		cc -o ls ls.c -DSELINUX -lselinux;\
	else\
		echo "cc -o ls ls.c";\
		cc -o ls ls.c;\
	fi

debug: ls_debug

ls_debug: ls.c
	@if [ -d /selinux ];\
	then\
		echo "cc -g -o ls -DLS_DEBUG -DSELINUX ls.c -lselinux";\
		cc -g -o ls -DLS_DEBUG -DSELINUX ls.c -lselinux;\
	else\
		echo "cc -g -o ls -DLS_DEBUG ls.c";\
		cc -g -o ls -DLS_DEBUG ls.c;\
	fi

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
#rm -f ls arch a.out uname logname hostid yes *link
	rm -f $(progs)

.PHONY: todo
todo:
	head -n 20 TODO
