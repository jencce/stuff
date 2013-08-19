#! /bin/bash
declare -a params=( -a -l -al )
declare -a dirs=( . / /boot )

function do_case()
{
	if [ $# -ne 2 ]
	then
		echo two arg needed
	fi

	echo arg $1 $2

	rd=$RANDOM

	./ls $1 $2 > /tmp/zxls$1$rd
	\ls $1 $2 > /tmp/ls$1$rd
	
	diff /tmp/{zx,}ls$1$rd > /dev/null  2>&1
	if [ ! $? -eq 0 ]
	then
		echo case $1 $2 failed rd=$rd
		#diff -u /tmp/{zx,}ls$1$rd
	fi
}

for par in ${params[*]}
do
	for dir in ${dirs[*]}
	do
		#echo $par $dir
		do_case "$par" "$dir"
	done
done

