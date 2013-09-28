#!/bin/bash

GITPATH=/home/zx/git/linux-next
TTPATH=/media/new/test

cd $GITPATH

# pull git repo
git pull --rebase
if [ ! $? -eq 0 ]; then
	wall "git pull error"
fi

latesttag=`git tag -l next* | tail -n 1`
latest=`git tag -l next* | tail -n 1 | tr -d '-'`

echo The latest tag is ${latesttag}

# uname cmp to make sure the lastest kernel
# is running
echo `uname -r` | grep $latesttag
if [ $? -eq 0 ]; then
	echo "up to date"
	exit
fi

# latest image already installed
ls /boot | grep ${latesttag}
if [ $? -eq 0 ]; then
	echo "up to date"
	exit
fi

# make sure the latest next srctar is ready
if [ -d ${TTPATH}/${latest} ]; then
	echo the latest ${latest} src is ready
else
	echo "get the latest src tar"
	git archive --prefix=${latest}/ \
		-o ${TTPATH}/linux-${latest}.tar\
		${latesttag}

	if [ ! $? -eq 0 ]; then
		echo "git archive error"
		exit
	else
		echo archive ${latest} done
	fi
fi 

cd ${TTPATH}

# tar xf srctar
if [ -e linux-${latest}.tar -a ! -d ${latest} ]; then
	tar xf linux-${latest}.tar
	if [ ! $? -eq 0 ]; then
		echo "tar xf error"
		exit
	else
		echo "tar xf latest done"
	fi
fi
	
cd ${latest}

# compile and install
function cmp() {
	if [ -e ./vmlinux ]; then
		echo already compiled
		exit
	fi

	if [ ! -e .config ]; then
		cp -f ../ttconfig .config
		cp -f ../ttconfig .config.old
		if [ ! $? -eq 0 ]; then
			echo "cp ttconfig error"
			exit
		fi
	fi

	echo compile begin in `pwd`
	make oldconfig
	make -j4 2>&1 > ./makerr
	if [ ! $? -eq 0 ]; then
		echo "make error"
		cat ./makerr
		exit
	fi

	make modules_install -j4 2>&1 > ./mierr
	if [ ! $? -eq 0 ]; then
		echo "module install error"
		cat ./mierr
		exit
	fi

	make install 2>&1 > ./inserr
	if [ ! $? -eq 0 ]; then
		echo "install error"
		cat ./inserr
		exit
	fi

	sed -i '/^default/s/[1-9]/0/' /boot/grub/menu.lst
	if [ ! $? -eq 0 ]; then
		echo "sed error"
		exit
	fi

	echo rebooting
	reboot
}
cmp
