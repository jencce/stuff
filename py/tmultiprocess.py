#! /usr/bin/python

import multiprocessing
import os

def dx(par):
	print "mount-o asd" + repr(par)
	print "pid" + repr(os.getpid())


a = multiprocessing.Process(target = dx, args=("asd",),)
print a._kwargs
print a.pid
print a._args
a.start()
print a.pid
a.join()

