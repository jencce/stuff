#! /bin/bash
declare -a params=( -a -A -l -R -al -aR -lR -alR )
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
	if test -e $dir;
	then
		do_case " " "$dir"
	fi
	#(trap "echo Timeout; exit" USR1 && do_case " " "$dir") &
	#(sleep 30 && kill -USR1 $! > /dev/null 2>&1) &
	#sleep 1
done

for par in ${params[*]}
do
	for dir in ${dirs[*]}
	do
		#echo $par $dir
		if test -e $dir;
		then
			do_case "$par" "$dir"
		fi
		#(trap "echo Timeout; exit" USR1 && do_case "$par" "$dir") &
		#(sleep 30 && kill -USR1 $! > /dev/null 2>&1) &
		#sleep 1
	done
done

# multifiles test case
function mftest {
	cd
	/home/zx/git/coreutils/ls  work/ mail/  neiwangbackup/ not /tmp/zxls59 /home/zx/git/sharkserver.log mods dead.letter /home/zx/c/stoaa > /tmp/zxlsmf
	\ls  work/ mail/  neiwangbackup/ not /tmp/zxls59 /home/zx/git/sharkserver.log mods dead.letter /home/zx/c/stoaa > /tmp/lsmf

	diff /tmp/{zx,}lsmf > /dev/null  2>&1
	
	if [ ! $? -eq 0 ]
	then
		echo multifiles case failed, rd=mf
	else
		rm -f /tmp/{zx,}lsmf
	fi
}
