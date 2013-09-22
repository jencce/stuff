#!/bin/bash

var='1 2'

if [ $var = '1 2' ]
then
	echo 'yes'
else
	echo 'no'
fi

if [ "$var" = '1 2' ]
then
	echo 'yes'
else
	echo 'no'
fi

if [ ${var} = '1 2' ]
then
	echo 'yes'
else
	echo 'no'
fi

a=1
if [ $a = 1 ]
then
	echo 'yes'
else
	echo 'no'
fi
