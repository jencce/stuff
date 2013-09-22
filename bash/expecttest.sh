#! /bin/bash
spawn sudo ls
expect "*password*"
send "ls"
