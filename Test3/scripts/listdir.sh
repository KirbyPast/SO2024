#!/bin/bash

if [ $# -lt 2 -a $# -gt 2 ]
then
	echo "Gresit la argumente!"
	exit 2
fi

if [ ! -d $1 -o ! -r $1 ]
then
	echo "Nu este director sau nu am drepturi de citire!" >2
	exit 2
fi
echo > forbidden.log
for i in "$1"/*
do
	if [ ! -r $i ]
	then
		echo $(basename $i) >> forbidden.log
	fi
	if [ -f $i ]
	then
		$(dirname $0 | tr -d "scripts")programs/alphanumeric $i
	fi
done


