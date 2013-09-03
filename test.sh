#! /bin/bash
declare -a params=( -a -l -R -al -aR -lR -alR )
declare -a dirs=( . ~/c /boot /opt ~/hadoop-1.0.4 )

function do_case()
{
	if [ $# -ne 2 ]
	then
		echo two arg needed
	fi

	#echo arg1 $1 arg2 $2

	rd=$(($RANDOM % 100))

	./ls $1 $2 > /tmp/zxls$rd
	\ls $1 $2 > /tmp/ls$rd
	
	diff /tmp/{zx,}ls$rd > /dev/null  2>&1
	if [ ! $? -eq 0 ]
	then
		echo case $1 $2 failed rd=$rd
		#diff -u /tmp/{zx,}ls$1$rd
	else
		rm -f /tmp/{zx,}ls$rd
	fi
}

for dir in ${dirs[*]}
do
	#echo $par $dir
	do_case " " "$dir"
done

for par in ${params[*]}
do
	for dir in ${dirs[*]}
	do
		#echo $par $dir
		do_case "$par" "$dir"
	done
done

