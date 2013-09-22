#! /usr/bin/env python

import sys
import string
'''
try:
	with open("first.py") as f:
		for s in f:
			print s,
except IOError as (errno, strerror):
	print "IOError({0}):{1}.".format(errno,strerror)
	raise
'''

fn = open('conf')
for s in fn:
	#print s.split(s," ",10)[0]
	#print s.split()
	print string.split(s, " ", 2)

'''
print "file name"
filename = raw_input(">")
print "fn "+filename
txt = open(filename)
print txt.read()
'''
