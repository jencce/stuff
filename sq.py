#! /usr/bin/env python
import sqlite3;

conn = sqlite3.connect('zx.db')

c = conn.cursor()

c.execute('select * from mm')

print c.fetchall()

conn.commit()

conn.close()

print 'ok'
