#!/bin/bash
if [ ! -d $1 ]
then
	echo "Nu este director!" >2
	exit 2
fi

dir=$(dirname $0)/scripts
ok=0
for i in "$dir"/*
do
	case $(basename $i) in
		listdir.sh) ok=1;;
	esac
done

if [ $ok -eq 0 ]
then
	echo "NU exista bash"
	exit 2
fi

chmod 700 $dir/listdir.sh
ok=0; okcomp=0;
dir=$(dirname $0)/programs
for i in "$dir"/*
do
	case $(basename $i) in
		alphanumeric.c) ok=1;;
		alphanumeric) okcomp=1;;
	esac
done

if [ $ok -eq 0 ]
then
	echo "NU exista c"
	exit 3
fi

if [ $okcomp -eq 0 ]
then
	gcc -Wall $dir/alphanumeric.c -o $dir/alphanumeric
fi

dir=$(dirname $0)/scripts

$dir/listdir.sh $1 | grep ":alphanumeric" | head -5 | cut -d ":" -f 1
exit 0;
