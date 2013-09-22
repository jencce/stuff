#! /usr/bin/python
''' to test built-in function filter map reduce'''

def cat(x):
	if x > 4:
		return 1
	else:
		return 0

print filter(cat, range(1, 10))

def cub(x):
	return x*x*x

print map(cub, range(1, 10))

def multi(x, y):
	return x+y

print reduce(multi, range(1, 10))

