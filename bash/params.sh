#! /bin/bash

echo $#

echo $0
echo $1

if [ $# -lt 2 ]
then
	echo "at least 2 args needed"
	exit
else
	echo -n '"$@": '
	for i in "$@"
	do
		echo $i
	done
	echo -n '"$*": '
	for i in "$*"
	do
		echo $i
	done
fi

