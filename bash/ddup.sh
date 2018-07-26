#!/bin/bash
ls -lS . | awk 'BEGIN { getline; getline; name=$9; size=$5; }
			{
				name1=$9; size1=$5;
				if (size1==size) {
					"md5sum "name1 | getline; sum1=$1;
					"md5sum "name  | getline; sum=$1;
					if (sum==sum1) {
						print name1;
						print name;
					}
			  	}
				name=name1; size=size1;
			}' | sort -u > dupfiles

cat dupfiles | xargs -I {} md5sum {} | sort | uniq -w 32 | awk '{print $2}' | sort -u > uniqfiles

echo Removing
comm dupfiles uniqfiles -2 -3 | tee /dev/stderr | xargs rm
echo Done
