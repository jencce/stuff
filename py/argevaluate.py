#! /usr/bin/env python
i = 5
def f(arg = i): 
	print arg

i = 6

f()
f()

''' args are inited in the defining time '''

def f1(a, L=[100]):
	L.append(a)
	L.append(10)
	print L
	return L

#print f1(1)
f1(1)

def f1(a, L=[101]):
	print 'in ',
	print L
	L.append(a)
	L.append(10)
	print 'out ',
	print L
	return L

f1(2)
''' sequence call to func with mutable type args makes they
	accumulate to the follow calls'''
f1(3)
#print f1(2)
#print f1(3)

def f2(a, L=[]):
	'''if L is None:
	'''
	L = []
	L.append(a)
	return L

print f2(1)
print f2(2)

def f3(a, b=10, c='asd'):
	print a,b,c

f3(10)
f3(10,11,'asdsad')
#f3(b=22,10)  invalid, keyword args must follow positional args
f3(12,c='ppp',b=22)
f3(b=22,a=10)

print f2.__doc__
