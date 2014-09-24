#! /bin/bash

CFGS="allyes allmod rand"
RAND_ROUNDS=20
#CFGS="rand"

if [ $# -ne 1 ]; then
	echo "Usage: $0 <linux-dir>|<linux-gz>"
	exit
fi

if [ ! -d $1 ]; then
	echo "$1 not directory"
	tar zxf $1
	if [ $? -ne 0 ]; then
		echo "Usage: $0 <linux-dir>|<linux-gz>"
		exit
	fi

	set `echo $1 | cut -d . -f 1`

	if [ ! -d $1 ]; then
		echo "Dir not match"
		exit
	fi
fi

TDATE=`basename $1`

if [ ! -d configs/${TDATE} ]; then
	mkdir -p configs/${TDATE}
fi

if [ ! -d errs/${TDATE} ]; then
	mkdir -p errs/${TDATE}
fi

pushd $1

for cfg in $CFGS
do
	if [ $cfg != "rand" ]; then
		make ${cfg}config
		make -j4 > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			cp .config ../configs/${TDATE}/config${cfg}
			make > ../errs/${TDATE}/err${cfg} 2>&1
		fi
	else
		for i in `seq 1 $RAND_ROUNDS`
		do
			echo randtest $i
			make randconfig
			make -j4 > /dev/null 2>&1
			if [ $? -ne 0 ]; then
				cp .config ../configs/${TDATE}/randconfig${i}
				make > ../errs/${TDATE}/randerr${i} 2>&1
			fi
		done
	fi
done

popd
