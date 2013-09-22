#! /usr/bin/env python
import string

ss = "qwe123lasd" 
print len(ss) 
ss = "ab,cde,efgh,asdsd"
se = "cd"
sd = ","
print ss
print 'ffab ',  ss.find('ab')
print 'ff, ',  ss.find(',')
sf = ss[ss.find(sd) + 1:]
print sf
sf = ss.find(se)
print sf

idstring = raw_input("input idstring")
cmdstrings = "sar -n DEV 1 10 > /tmp/sarnDEV-", \
			"iostat 1 10 > /tmp/iostat-",  \
			"sar -B 1 10 > /tmp/sarB-",    \
			"sar -P ALL 1 10 > /tmp/sarPALL-", \
			"vmstat 1 10 > /tmp/vmstat-"

for cs in cmdstrings:
	print cs[cs.find("/tmp"):]+idstring

print cs
print cs.split()
