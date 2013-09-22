#! /bin/bash
var=1
path=""
function myfdisk()
{
	echo fdisk $1
}
for i in `ls /dev/sd[a-z]`
#for i in `seq 25`
do
	if [ $var -lt 10 ]
	then
		path=disk0$var
	else
		path=disk$var
	fi
	((var = var+1))
	if [ -e $i"1" ] 
	then
		echo "contied"
#		continue
	fi
	myfdisk $i"1"
	#echo $i
	echo path:$path
	if [ -e /mnt/$path ]
	then
		echo $path exist
	else
		echo $path not exist
	fi

	echo -e $i"1""\t"$path

done

#fdisk /dev/sdu <<EOF
#o
#n
#p
#1
#
#
#w
#EOF
