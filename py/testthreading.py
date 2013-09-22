#! /usr/bin/env python
import threading

class A(threading.Thread):
	def run(self):
		while(1):
			pass

a=A()
a.start()
b=A()
b.start()
c=A()
c.start()
d=A()
d.start()
print "back start"
a.join()
b.join()
c.join()
d.join()
print "main waiting"
