#! /usr/bin/env python

class B:
	pass

class C(B):
	pass

class D(C):
	pass

# print B C D in order
for c in [B, C, D]:
	try:
		raise c()
	except D:
		print 'D'
	except C:
		print 'C'
	except B:
		print 'B'

# print B D D in order
for c in [B, C, D]:
	try:
		raise c, c()
	except C:
		print 'D'
	except D:
		print 'C'
	except B:
		print 'B'
