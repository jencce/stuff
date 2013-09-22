#! /usr/bin/python
import os
import sys
import string

cnt = 10
idstr = "asdf"
cmdstrings = "sar -n DEV 1 10 > /tmp/sarnDEV-", \
			 "iostat 1 10 > /tmp/iostat-",	\
			 "sar -B 1 10 > /tmp/sarB-",	\
			 "sar -P ALL 1 10 > /tmp/sarPALL-",	\
			 "vmstat 1 10 > /tmp/vmstat-"

css = "sar -n DEV", "iostat", "sar -B", "sar -P ALL", "vmstat"

cfs = "/tmp/sarnDEV-","/tmp/iostat-","/tmp/sarB-","/tmp/sarPALL-",	"/tmp/vmstat-"

cmd_dict = dict(zip(css, cfs))
print cmd_dict

for cs in css:
	ns = '{0} 1 {1} > {2}{3}'.format(cs, repr(cnt), cmd_dict[cs], idstr)
	print ns
