#! /usr/bin/python
import threading
import os

class ppss(threading.Thread):
	
	cs = ""
	def __init__(self, cstring):
		threading.Thread.__init__(self)
		self.cs = cstring

	def func(self):
		if self.cs == "":
			print 'cs null'
			exit()

		ret = os.system(self.cs)
		if ret != 0:
			print 'ex error'
			exit()

	def run(self):
		print 'thread running', self.cs
		self.func()

def main():
	idstring = "lasd"
	cmdstrings = "sar -n DEV 1 10 > /tmp/sarnDEV-", \
				 "iostat 1 10 > /tmp/iostat-",	\
				 "sar -B 1 10 > /tmp/sarB-",	\
				 "sar -P ALL 1 10 > /tmp/sarPALL-",	\
			 	"vmstat 1 10 > /tmp/vmstat-"

	ps = {}
	for cs in cmdstrings:
		ns = cs + idstring
		print ns

		ps[cs] = ppss(ns);
		ps[cs].start()

	for cs in cmdstrings:
		ps[cs].join()

	print 'end'

if __name__ == '__main__':
	main()

