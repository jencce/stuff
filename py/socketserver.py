#! /usr/bin/env python
import socket
import time
sersock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sersock.bind(('',7777))
sersock.listen(5)
cs,addr = sersock.accept()

fn = open('/tmp/sar')
while True:
	data = fn.read(1024)
	if not data:
		break
	while len(data) > 0:
		sended = cs.send(data)
		data = data[sended:]

time.sleep(2)
cs.send('EOF')
fn.close()


print addr
cs.close()
