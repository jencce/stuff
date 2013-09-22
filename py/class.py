#! /usr/bin/env python

class myclass:
	"zx class"
	i = 11111
	def f(self):
		return 'hw'
	def pi(self):
		print "pi i={0}".format(self.i)
	def si(self, v):
		self.i = v
	def pp(self):
		print 'pp',
		self.pi()
	
x = myclass()
x.f()
x.pi()
x.si(9)
x.pi()
x.pp()

print '============================'

class newclass(myclass):
	j = 2222
	def pj(self):
		print "j={0}".format(self.j)
	def sj(self, v):
		self.j = v

	# override baseclass method with same name
	# nothing to do with args, but can fail 
	# compatibility
	def pi(self):
		print 'new pi {}'.format(self.i)

y = newclass()
y.f()
y.pi()
y.pj()
y.sj(9)
y.pj()
y.pp()
myclass.pi(y)

print "========================="
z=myclass()
z.pp()
