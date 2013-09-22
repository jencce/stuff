#! /usr/bin/env python
import sys

def ask_ok(prompt='pls', retry=4, complaint='Yes or No'):
	while True:
		ok = raw_input(prompt)
		if ok in ('y', 'yes'):
			return True
		if ok in ('n','no'):
			return False
		retry = retry - 1
		if retry < 0:
			raise IOError('ref')
		print complaint

#ask_ok()

print sys.argv
print sys.argv[0]
print sys.argv[1]
print sys.argv[2]
