#! /usr/bin/env python
class BaseBaseClass1:
	def f(self):
		print 'base base 1'
	
class BaseClass1(BaseBaseClass1):
	def f1(self):
		print 'base 1'
	
class BaseClass2:
	def f(self):
		print 'base 2'
	
class BaseClass3:
	def f(self):
		print 'base 3'
	
class BaseClass4:
	def f(self):
		print 'base 4'
	
class MyClass(BaseClass1, BaseClass2, BaseClass3, BaseClass4):
	__ver='1.0'
	def ff(self):
		self.f();
		# inner access to __ver is ok
		print self.__ver

mc=MyClass()
mc.ff()
print 'type of mc {}'.format(type(mc))
print 'mc.__class__ {}'.format(mc.__class__)

print dir(mc)
# __ver has been mangling to _MyClass__ver
# outside access denied
# the next print will fail
#print mc.__ver
print mc._MyClass__ver
