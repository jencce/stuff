#! /usr/bin/env python
i = 5

def f(arg = i): 
	print arg

f() # 5

i = 6

f() # 5

''' args are inited in the defining time '''

def f1(a, L=[100]):
	L.append(a)
	L.append(10)
	print L
	return L

f1(1)   # [100, 1, 10]

def f1(a, L=[101]):
	print 'in ',
	print L
	L.append(a)
	L.append(10)
	print 'out ',
	print L
	return L

f1(2) # in [101] out [101, 2, 10]
''' sequence call to func with mutable type args makes they
	accumulate to the follow calls'''
f1(3) # in [101, 2, 10] out [101, 2, 10, 3, 10]

def f2(a, L=[]):
	'''if L is None:
	'''
	L = []
	L.append(a)
	return L

print f2(1) # [1]
print f2(2) # [2]

def f3(a, b=10, c='asd'):
	print a,b,c

f3(10) # 10,10,asd
f3(10,11,'asdsad') # 10,11,asdasd
#f3(b=22,10)  invalid, keyword args must follow positional args
f3(12,c='ppp',b=22) # 12,22,ppp
f3(b=22,a=10) # 10,22,asd

print f2.__doc__
