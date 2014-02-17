#! /bin/bash

echo $$ TEST: $TEST

if [ $# -gt 0  ] && [ $1 -eq 1 ]
then
	exit
fi

export TEST=lock
sh newshell.sh 1
