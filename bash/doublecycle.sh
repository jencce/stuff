#! /bin/bash

for ver in 2 3 4;
do
	for proto in tcp udp;
	do
		if [ $ver -eq 2 ];
		then
			continue
		fi
		echo $ver $proto
	done
done
