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
kernel-devel gawk busybox device-mapper fcoe-utils mdadm lldpad nbd \
libiscsi iscsi-initiator-utils cifs-utils device-mapper-multipath \
dmraid cryptsetup lvm2 acl attr autoconf bc bind-utils btrfs-progs \
e2fsprogs-devel e4fsprogs gcc gdbm-devel gettext gfs2-utils git \
indent kernel-devel kernel libacl-devel libaio-devel libattr-devel \
libblkid-devel libcap libtool libuuid-devel ncurses-devel \
openssl-devel perl policycoreutils-python popt-devel psmisc \
pyOpenSSL python quota readline-devel rpm-build shadow-utils wget \
xfsdump xfs-kmod xfsprogs xfsprogs-devel xfsprogs-qa-devel screen 

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
if ! grep "up to date" /tmp/xl || ! test -d /var/lib/xfstests ; then
	make -j$cpucnt > /dev/null 2>&1
	make install > /dev/null 2>&1
fi
popd

pushd ltp
git pull --rebase > /tmp/ll 2>&1
if ! grep "up to date" /tmp/ll || ! test -e /opt/ltp/runltp ; then
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
	make -j$cpucnt > /dev/null 2>&1 && \
	make modules_install -j$cpucnt > /dev/null 2>&1 && \
	make install && grub2-set-default 0 && reboot
fi
popd

# ltp
for i in fs fs_ext4 ; do
	local logfile="${HOME}/ltp/`uname -r`-${i}.log"
	local outfile="${HOME}/ltp/`uname -r`-${i}.out"
	test -e $logfile && continue
	/opt/ltp/runltp -p -q -f $i -l $logfile -o $outfile -d $test_mnt
	if grep -wq FAIL $logfile ; then
		grep -w FAIL $logfile | mail -s "`uname -r` ltp fail" xzhou@redhat.com
	fi
done

# xfstests
pushd /var/lib/xfstests
cat > local.config  <<EOF
export EMAIL=xzhou@redhat.com
export TEST_DIR=$test_mnt
export TEST_DEV=$test_dev
export SCRATCH_DEV=$sct_dev
export SCRATCH_MNT=$sct_mnt
export FSTYP=ext4
EOF
if ! test -s $HOME/xfstests-`uname -r` ; then
	./check -g auto > $HOME/xfstests-`uname -r` 2>&1
	cat  $HOME/xfstests-`uname -r` | mail -s "`uname -r` xfstests fail" xzhou@redhat.com
fi
popd
