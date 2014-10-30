#! /bin/bash
HOME=/home/zx

if test -z "$LTP_BIG_DEV"; then
	fallocate -l 10GiB ${HOME}/ext4big.img
	bd=`losetup -f`
	losetup $bd ${HOME}/ext4big.img
	export LTP_BIG_DEV=$bd
fi

if test $# -eq 0; then
	TESTS="fs fs_ext4"
else
	TESTS="$@"
fi

for i in ${TESTS}; do
	echo "/opt/ltp/runltp -p -q -f $i -l ${HOME}/test/ltp/`uname -r`-${i}.log -o ${HOME}/test/ltp/`uname -r`-${i}.out -d /mnt/ext4"
	/opt/ltp/runltp -p -q -f $i -l ${HOME}/test/ltp/`uname -r`-${i}.log -o ${HOME}/test/ltp/`uname -r`-${i}.out -d /mnt/ext4
done
