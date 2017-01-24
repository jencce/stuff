#! /usr/bin/python

class Carbon(object):

	def __init__(self, f):
		print "carbon"

class Water(object):

	def __init__(self, f):
		print "water"

@Carbon
@Water
class Suger(object):

	def __init__(self, f):
		print "suger"

sg = Suger("a") # failed

def p0(f):
	print "p0"
	return f

def p1(f):
	print "p1"
	return f

@p0
@p1
def p2(f):
	print "p2"
	print f
# print p1 p0

print 1
p2 # nothing
print 2
p2("a") # p1 p0 1 2 p2 a
