#! /bin/bash
expect -c 'spawn su root -c ls
	expect  "*assword*" { send "456789\r" } 
	expect eof
	'
