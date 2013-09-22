#! /usr/bin/env python
import time, os.path
from string import Template

photofiles = ['img_1222.jpg', 'img_222.jpg', 'ima_333.jpg']

class BatchRename(Template):  #Template subclass
	delimiter = '%'

fmt = raw_input('Enter rename style (%d-date %n-seq %f-format): ')

t = BatchRename(fmt)
date = time.strftime('%d%b%y')

for i, filename in enumerate(photofiles):
	base, ext = os.path.splitext(filename)
	#print "base:{0},ext:{1}.".format(base, ext)
	newname = t.substitute(d=date,n=i,f=ext)
	print '{0}-->{1}'.format(filename,newname)
