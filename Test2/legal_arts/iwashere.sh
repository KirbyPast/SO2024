#!/bin/bash

if [ $# -eq 3 ]
then
	echo "Not enough arguments/Invalid usage!!"
	exit 2
else
	fis=$1
fi

if [ -f $fis ]
then
	$(dirname $0)/performances/filegraffiti $fis $(hostname)
fi
if [ -d $fis ]
then
	for i in "$fis"/*
	do
#		if [ -f $i ]
#		then
			$0 $i
#		fi
	done
fi
