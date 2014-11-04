#!/usr/bin/python

import sys
import os
import pygtk
try:
	import gtk
	import gtk.glade
except:
	sys.exit(1)

class pyglade:
	""" hello world python gtk glade"""
	
	def __init__(self):
		#Set the glade file
		self.gladefile = "../glade/textview.glade"
		self.wTree = gtk.glade.XML(self.gladefile)

		#Get the main window
		self.window = self.wTree.get_widget("window1")
		if (self.window):
			self.window.connect("destroy", gtk.main_quit)

		self.textview = self.wTree.get_widget("textview1")
		if (self.textview):
			#self.testview.connect("destroy", gtk.main_quit)
			self.tbuf = gtk.TextBuffer()
			f_str = os.popen("fortune").read()
			self.tbuf.set_text(f_str)
			self.textview.set_buffer(self.tbuf)

		def destroy(self, *args):
			gtk.main_quit()

if __name__ == "__main__":
	w = pyglade()
	w.window.show_all()
gtk.main()
