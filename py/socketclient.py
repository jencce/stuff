#! /usr/bin/env python
import socket
cs = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cs.connect(('127.0.0.1',7777))

fn = open('sar', 'w')
while True:
	data = cs.recv(1024)
	if data == 'EOF':
		break
	fn.write(data)

fn.close()
print 'done'

