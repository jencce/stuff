#! /usr/bin/env python

def av(vs):
	"""retunr av of vss.

	>>> print av([20,30,40]) #space before print needed
	30.0
	"""
	return sum(vs,0.0) / len(vs)

#print av([20,30,40])

import doctest
print doctest.testmod()
