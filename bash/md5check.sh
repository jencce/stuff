#! /bin/bash

DIRS="/bin /sbin /usr/bin /etc"
#DIRS="/usr/local/bin"

function mdck () { 
	for i in $DIRS
	do
		for j in `ls $i`
		do
			if [ -e $i/$j ]
			then
				echo -n `stat -c "%A %U %G" $i/$j 2>/dev/null` >> $1
				echo -ne "\t" >> $1
				echo `md5sum $i/$j 2>/dev/null` >> $1
			fi
		done
	done
}


if [ ! -f ~/.md5ck ]
then
	echo > ~/.md5ck
	mdck ~/.md5ck
	sudo chattr +i ~/.md5ck
fi

echo "" > /tmp/md5ck
mdck /tmp/md5ck

diff ~/.md5ck /tmp/md5ck > /dev/null 2>&1
if [ $? -ne 0 ]
then
	echo "system alterd"
	diff ~/.md5ck /tmp/md5ck
else
	echo "cked"
fi
