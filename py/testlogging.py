#! /usr/bin/python
''' to test loggin'''
import os
import sys
import string
import logging

fmt = '%(levelname)s:%(process)d:%(asctime)s:%(pathname)s:%(lineno)s:%(funcName)s:%(message)s'
	
def func12():
	''' to test funcname'''
	logging.basicConfig(filename='./sharkserver.log', format=fmt, level=logging.DEBUG)
	logging.debug("debug")
	logging.info("info")
	logging.error("error")
	logging.warning("warning")
	logging.exception("exception")

func12()
