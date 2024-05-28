#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Dati adancimea cautarii: " n
else
	n=$1
fi

shift

files=$(find ~ -mindepth $n -type f -perm u+rw)

if [ $? -eq 0 ]
then
	for fis in $files
	do
		file $fis | grep "Bourne-Again shell script"
		if [ $? -eq 0 ]
		then
			chmod a+x $fis
			echo $fis
			#find $fis -printf "%m"
#		else
			#find $fis -printf "%M"	
		fi
	done
else
	echo "The find command has failed!" >&2
	exit 2
fi

exit 0
