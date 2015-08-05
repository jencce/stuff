#! /bin/bash -x

if test $# -ne 4 && test $# -ne 0 ; then
	echo './xx $test_dev $sct_dev $test_mnt $sct_mnt'
	echo 'or'
	echo './xx'
	exit 0
fi

HOME=/home/
cpucnt=$(grep -c processor /proc/cpuinfo)
mkdir -p $HOME/ltp

yum install -y rpm-build automake autoconf libtool libgssglue-devel \
net-tools asciidoc elfutils-libelf-devel zlib-devel binutils-devel \
newt-devel python-devel hmaccalc  perl-ExtUtils-Embed net-tools xmlto \
asciidoc hmaccalc python-devel newt-devel elfutils-devel binutils-devel \
audit-libs-devel numactl-devel pciutils-devel pesign bc mailx openssl \
openssl-devel xmlto ncurses-devel xfslibs-dev uuid-dev libtool e2fsprogs \
automake gcc libuuid1 quota attr libattr1-dev libacl1-dev libaio-dev \
xfsprogs libgdbm-dev fio dbench bison automake autoconf libuuid-devel \
kernel-devel gawk 

useradd fsgqa

if test $# -eq 0 ; then
	mkdir -p /mnt/tst
	mkdir -p /mnt/sct
	export test_mnt=/mnt/tst
	export sct_mnt=/mnt/sct
	fallocate -l 10GiB ${HOME}/test.img
	fallocate -l 10GiB ${HOME}/sct.img
	export test_dev=`losetup -f`
	losetup $test_dev ${HOME}/test.img
	export sct_dev=`losetup -f`
	losetup $sct_dev ${HOME}/sct.img
	mkfs.ext4 -q $test_dev
	mkfs.ext4 -q $sct_dev
	mount $test_dev $test_mnt
	mount $sct_dev $sct_mnt

	trap "umount -d $test_mnt; umount -d $sct_mnt; exit" 0 1 2 3 9 15
fi

if test $# -eq 4 ; then
	export test_dev=$1
	export sct_dev=$2
	export test_mnt=$3
	export sct_mnt=$4

	umount $test_dev
	mkfs.ext4 -q $test_dev
	mount $test_dev $test_mnt

	umount $sct_dev
	mkfs.ext4 -q $sct_dev
	mount $sct_dev $sct_mnt
fi

if ! mountpoint $test_mnt || ! mountpoint $sct_mnt ; then
	echo "no partition mounted"
	exit 0
fi

if ! test -d linux-next ; then
	git clone git://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git
fi

if ! test -d ltp ; then
	git clone https://github.com/linux-test-project/ltp.git
	pushd ltp
	make autotools > /dev/null 2>&1
	./configure > /dev/null 2>&1
	make -j$cpucnt > /dev/null 2>&1
	make install > /dev/null 2>&1
	popd
fi

if ! test -d xfstests ; then
	git clone git://oss.sgi.com/xfs/cmds/xfstests
	pushd xfstests
	make -j$cpucnt > /dev/null 2>&1
	make install > /dev/null 2>&1
	popd
fi

pushd xfstests
git pull --rebase > /tmp/xl 2>&1
if ! grep "up to date" /tmp/xl ; then
	make -j$cpucnt > /dev/null 2>&1
	make install > /dev/null 2>&1
fi
popd

pushd ltp
git pull --rebase > /tmp/ll 2>&1
if ! grep "up to date" /tmp/ll ; then
	make autotools > /dev/null 2>&1
	./configure > /dev/null 2>&1
	make -j$cpucnt > /dev/null 2>&1
	make install > /dev/null 2>&1
fi
popd

pushd linux-next
git pull --rebase > /tmp/lpl 2>&1
ltag=$(git tag | grep next | sort -V | tail -1)
if ! grep "up to date" /tmp/lpl || ! [[ `uname -r` =~ $ltag ]] ; then
	rm -f .config .config.old
	echo -e "y\n1\n$(yes | head -n 1000)" | make oldconfig
	make -j$cpucnt > /dev/null 2>&1
	make modules_install -j$cpucnt > /dev/null 2>&1
	make install && reboot
fi
popd

# ltp
for i in fs fs_ext4 ; do
	echo "/opt/ltp/runltp -p -q -f $i -l ${HOME}/ltp/`uname -r`-${i}.log -o ${HOME}/ltp/`uname -r`-${i}.out -d $test_mnt"
	/opt/ltp/runltp -p -q -f $i -l ${HOME}/ltp/`uname -r`-${i}.log -o ${HOME}/ltp/`uname -r`-${i}.out -d $test_mnt
done

# xfstests
