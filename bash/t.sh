export result=0
f0()
{
	echo f0
	f1
}

f1()
{
	echo f1
	f2
	(((result++)); echo f1f2)
}

f2()
{
	echo f2
	f3
	((result++))
}

f3()
{
	echo f3
	i=0; while caller $i > /tmp/ff; do i=$((i+1));  echo -e "\t`cat /tmp/ff`"; done
	#. ./calltrace.sh
}


f0
echo result=$result
