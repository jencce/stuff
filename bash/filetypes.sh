#!/bin/bash
# Display how many file types and files of each type in a dir.

dir=${1:-"."}

declare -A statarray

for f in `find $dir -type f -print` ; do
	tf=$(file -b $f | cut -d, -f1)
	let statarray["$tf"]++
done

for i in "${!statarray[@]}" ; do
	echo $i: ${statarray["$i"]}
done
