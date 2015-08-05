#! /bin/bash -x

CFGS="allyes allmod rand"
RAND_ROUNDS=20
#CFGS="rand"
cpucnt=$(grep -c processor /proc/cpuinfo)

yum install -y rpm-build automake autoconf libtool libgssglue-devel \
net-tools asciidoc elfutils-libelf-devel zlib-devel binutils-devel \
newt-devel python-devel hmaccalc  perl-ExtUtils-Embed net-tools xmlto \
asciidoc hmaccalc python-devel newt-devel elfutils-devel binutils-devel \
audit-libs-devel numactl-devel pciutils-devel pesign bc mailx openssl \
openssl-devel xmlto ncurses-devel

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

pushd $1

if git status -uno ; then
	# in git repo
	git pull --rebase
	if git tag | grep next ; then
		# -next repo
		TTAG=`git tag | grep next | tail -1`
		git checkout -b $TTAG
	else
		TTAG=`git tag | sort -V | tail -1`
		git checkout -b $TTAG
	fi
else
	TTAG=`basename $1`
fi

TDATE=${TTAG}-$(date +%F-%R)

if [ ! -d configs/${TDATE} ]; then
	mkdir -p ../configs/${TDATE}
fi

if [ ! -d errs/${TDATE} ]; then
	mkdir -p ../errs/${TDATE}
fi

for cfg in $CFGS
do
	if [ $cfg != "rand" ]; then
		make ${cfg}config
		make -j${cpucnt} > /dev/null 2>&1
		if [ $? -ne 0 ]; then
			cp .config ../configs/${TDATE}/config${cfg}
			make > ../errs/${TDATE}/err${cfg} 2>&1
			cat ../errs/${TDATE}/err${cfg} | mail -s "${TDATE} ${cfg}config make error" xzhou@redhat.com
		fi
	else
		for i in `seq 1 $RAND_ROUNDS`
		do
			echo randtest $i
			make randconfig
			make -j${cpucnt} > /dev/null 2>&1
			if [ $? -ne 0 ]; then
				cp .config ../configs/${TDATE}/randconfig${i}
				make > ../errs/${TDATE}/randerr${i} 2>&1
				cat ../errs/${TDATE}/randerr${i} | mail -s "${TDATE} randconfig $i make error" xzhou@redhat.com
			fi
		done
	fi
done

popd
