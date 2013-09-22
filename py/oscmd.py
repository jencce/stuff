#! /usr/bin/python
import os
import sys
import string

idstring = "lasd"
cmdstrings = "sar -n DEV 1 10 > /tmp/sarnDEV-", \
			 "iostat 1 10 > /tmp/iostat-",	\
			 "sar -B 1 10 > /tmp/sarB-",	\
			 "sar -P ALL 1 10 > /tmp/sarPALL-",	\
			 "vmstat 1 10 > /tmp/vmstat-"

for cs in cmdstrings:
	ns = cs + idstring
	print ns
	#ret = os.system(ns)
	#if ret != 0:
	#	print ns+"  error"
	#	exit()

os.system('hostname > /tmp/hn')
of = open('/tmp/hn')
print string.rstrip(of.readline(), '\n')
of.close()

os.system('mkdir -p ~/.zzx/')
# python does not know ~
#open('~/.zzx/')
