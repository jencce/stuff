#!/usr/bin/python

import gtk
from datetime import datetime
import time
r = 1

class waterwindow:
	
	def destroy(self, widget, data=None):
		gtk.main_quit()

	def __init__(self):
		self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
		self.window.set_title("Water Please~~")
		self.window.set_border_width(10)
		self.window.set_default_size(600,400)
		self.window.set_position(gtk.WIN_POS_CENTER)
		self.window.set_focus_on_map(True)
		self.window.connect("destroy", gtk.main_quit)
		self.button = gtk.Button("make NOTES Please~~~")
		self.button.set_size_request(180,10)
		self.button.connect_object("clicked", gtk.Widget.destroy, self.window)
		self.bbox = gtk.HButtonBox()
		self.bbox.pack_start(self.button, False, False, 0)
		self.window.add(self.bbox)
		self.window.show_all()
		#self.button.show()
		#self.window.show()
		gtk.main()
		
while r:
	if datetime.now().hour == 17:
		r = 0
	ww = waterwindow()	
	time.sleep(60*60)
else:
	pass
