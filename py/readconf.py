#! /usr/bin/env python
import socket
import time

config_file = open("conf")
for conf_item in config_file:
	print 'item ' + conf_item,
	find_clientip = conf_item.find("clientip")
	if find_clientip != -1:
		client_ip_list = conf_item.split()
		tmp_list = conf_item.split()
		print client_ip_list
		client_ip_list.reverse()
		client_ip_list.pop()
		print client_ip_list
		while client_ip_list.__len__():
			print client_ip_list.pop()
		print tmp_list
