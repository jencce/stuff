#! /usr/bin/env python
import sys

print "argc: {0}".format(sys.argv.__len__())
print "argc: {0}".format(len(sys.argv))
print "arg0:{0}, arg1:{1}, arg2:{2}.".format(sys.argv[0],sys.argv[1],sys.argv[2])

data = 'qwe123hust'
print data[0:6]
