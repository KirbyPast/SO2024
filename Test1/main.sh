#!/bin/bash

ok=0
dir=$(dirname $0)/scripts
for i in "$dir"/*
do
	case $(basename $i) in
		listdir.sh) ok=1;;
	esac
done

if [ $ok -eq 0 ]
then
	echo "Nu exista folderul scripts sau scripul listdir!" 1>&2
	exit 1
fi

ok=0
okcompilat=0
for i in "$(dirname $0)/programs"/*
do
	case $(basename $i) in
		filetypeandsize.c) ok=1;;
		filetypeandsize) okcompilat=1;;
	esac
done

if [ $ok -eq 0 ]
then
	echo "Nu exista folderul programs sau nu exista programul c sau nu aveti drepturi de citire asupra lor!" 1>&2
	exit 2
else
	fis=$(dirname $0)programs/filetypeandsize.c
	if [ -r $fis ]
	then
		echo "Nu am drepturi de citire!"
		exit 2
	fi
fi

if [ $okcompilat -eq 0 ]
then
	gcc -Wall $(dirname $0)programs/filetypeandsize.c -o $(dirname $0)programs/filetypeandsize
	error=$(gcc -Wall $(dirname $0)programs/filetypeandsize.c -o $(dirname $0)programs/filetypeandsize 2>&1 | grep -c -E "warning|error")
	if [ $error -eq 0 ]
	then
		echo "am compilat"
	else
		echo "Au aparut erori la compilarea programului!"
		exit 3
	fi
fi

if [ ! -d $1 -o ! -r $1 ]
then
	echo "Nu este director sau nu am drepturi de citire!"
	exit 4
fi

linecnt=$($(dirname $0)/scripts/listdir.sh $1 | grep -c ':')
let x=($linecnt-5)
if [ $linecnt -gt 20 ]
then
	$(dirname $0)/scripts/listdir.sh $1 | sed 's/:/---/' | head -n 20 | tail -n 15
else
	$(dirname $0)/scripts/listdir.sh $1 | sed 's/:/---/' | tail -n $x
fi
