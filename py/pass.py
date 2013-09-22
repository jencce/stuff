#! /usr/bin/python

idstring = "lasd"

if True:
	print idstring
	print 2
	pass

# dir prints nothing
dir()
print 3

# path needed to imply directory package
# to be searched when importing
__path__ = ['.', './package/']

import zx
zx.funczx()
idstring
