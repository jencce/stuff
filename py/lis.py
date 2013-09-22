#! /usr/bin/env python
import string
import sys
from datetime import datetime
from datetime import timedelta

def lis_dp(rs):
	ma=0
	d=[]
	for i in range(len(rs)):
		d.append(0)
		if rs[i].isdigit():
			rs[i] = int(rs[i], 10)
	
	for i in range(len(rs)):
		ans = d[i]
		for j in range(i):
			#print "i {0}, j {1}".format(i, j)
			# ------
			#if rs[i] > rs[j] and d[j] + 1 > ans:
			#	ans = d[j] + 1
			#if rs[i] <= rs[j] and d[j] > ans:
			#	ans = d[j]
			# -------
			# above code has logic error, add one when match
			# the first if, but it can be smaller the the 
			# former biggest one, which should not add one.
			#
			# the follow code has the right final answer, but not
			# every d[i] is correct, if d[i] means 0~i-1 longest
			# increase subsequence; it is correct if it means the
			# lis include rs[i]
			if rs[i] > rs[j] and d[j] > ans:
				ans = d[j]
		if i == 0:
			d[i] = 1
		else:
			#d[i] = ans
			d[i] = ans + 1
		if d[i] > ma:
			ma = d[i]

	print "dp " + repr(ma),

def bsearch(d, key, high):
	low = 0
	while low < high:
		mid = (low + high) / 2
		if key > d[mid]:
			low = mid + 1
		else:
			high = mid
	
	return low

def lis_bs(rs):
	low = 1
	high = len(rs)
	d = []
	k = 0 
	for i in range(high):
		d.append(0)
		if rs[i].isdigit():
			rs[i] = int(rs[i], 10)
	
	d[0] = rs[0]

	for i in range(1, high):
		#print "rs[i{0}] {1}, d[k{2}] {3}".format(i, rs[i], k, d[k])
		if rs[i] >= d[k]:
			k = k + 1
			d[k] = rs[i]
		else:
			#print "bs rs[i] " + repr(rs[i]) + " k " + repr(k)
			pos = bsearch(d, rs[i], k)
			d[pos] = rs[i]
			#print "pos " + repr(pos) + " i " + repr(i)
		#print d

	print "bs " + repr(k + 1),

def gettime():
	d = datetime.now()
	return timedelta(minutes = d.minute, seconds = d.second,\
	                        microseconds = d.microsecond)

def do_func(func, sp):
	t1 = gettime()
	func(sp)
	t2 = gettime()
	t3 = t2 -t1
	print "{0}s{1}ms elasped".format(\
		t3.seconds, t3.microseconds)

def main():
	fn = open("lisinput")
	for s in fn:
		print "==="
		if len(s.split()) == 0:
			continue
		print "len " + repr(len(s.split()))
		do_func(lis_bs, s.split())

if __name__ == '__main__':
	main()
