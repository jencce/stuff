#! /usr/bin/python
import os 
import sys 
import string

cmdstrings = "sar -n DEV 1 10 > /tmp/sarnDEV-", \
			 "iostat 1 10 > /tmp/iostat-",	\
			 "sar -B 1 10 > /tmp/sarB-",	\
			 "sar -P ALL 1 10 > /tmp/sarPALL-",	\
			 "vmstat 1 10 > /tmp/vmstat-"

ss = 'a b  c d e'
ll = ss.split()
tt = tuple(ll)

print cmdstrings[3]
print type(cmdstrings)
print type(ss)
print type(ll)
print type(tt)
print tt
print len(tt)
print tt[2]
print len(tt)

tt2 = tuple(ss)
print tt2
print len(tt2)
print tt2[0]
print tt2[2]
print tt2[8]
print tt2[9]
l = len(tt2)
print tt2[l-1]

for i in tt2:
	print i
print 'aaaa'
for i in tt2:
	print i
