#! /usr/bin/python
import os
import sys
import string
import syslog

syslog.openlog("SHARK", facility=syslog.LOG_USER)
syslog.syslog(syslog.LOG_DEBUG, "xtest")
syslog.closelog()
