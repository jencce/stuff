# Makefile

all: prog

prog: ls.c
	@if [ -d /selinux ];\
	then\
		echo "cc -o ls ls.c -DSELINUX -lselinux";\
		cc -o ls ls.c -DSELINUX -lselinux;\
	else\
		echo "cc -o ls ls.c";\
		cc -o ls ls.c;\
	fi

debug: ls.c
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

.PHONY: clean
clean:
	rm -f ls
