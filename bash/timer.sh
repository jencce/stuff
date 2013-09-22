#! /bin/bash
func() {
#( 
	trap "echo oh;exit" USR1 &&
	while :
	do
		sleep 1
	done
#)&
}
func &
echo $!
sleep 2 && kill -USR1 $! &
