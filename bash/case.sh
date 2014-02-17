#! /bin/bash
# No fall through

LS="1 2 3 4 5 56"

for i in $LS
do
	echo Now $i
	case $i in
		1) echo "1 here";;
		2) echo "2 here";;
		3) echo "3 here";;
		4) echo "4 here";;
	esac
done
