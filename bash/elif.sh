#! /bin/bash
if [ -d /raoot ]
then
	echo root exists
elif [ -e /tmp ]
# 'else if' is wrong syntax
then
	echo tmp exists
fi
