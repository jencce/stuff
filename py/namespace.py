#! /usr/bin/env python

gv = 1

def f(v):
	return v+1

def f1(v):
	v = v + 1

def f2(v):
	global gv
	gv = v

def p(v):
	# make global var in func
	global zx
	zx = 1 + v
	print v

f(gv)
p(gv)
f1(gv)
p(gv)
f2(9)
p(gv)
gv = 2
f(gv)
p(gv)
f1(gv)
p(gv)
f2(9)
p(gv)
print 'aaaaa'
p(11)
print zx
