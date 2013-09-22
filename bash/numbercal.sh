#!/bin/bash

f1=1
f2=3
((ret=$f1+$f2))
echo $?
echo $ret
ret=$(($f2-$f1))
echo $?
echo $ret
