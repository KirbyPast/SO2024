#!/bin/bash


dir=$(dirname $0)
is=0;
for i in "$dir"/*
do
	case $(basename $i) in
		legal_arts) is=1 ;;
	esac
done

if [ $is -eq 0 ]
then
	echo "nu exista legal_arts" >2
	exit 1
fi


dir=$(dirname $0)/legal_arts/performances
is=0
for i in "$dir"/*
do
	case $(basename $i) in
		filegraffiti) is=1;;
	esac
done
if [ $is -eq 0 ]
then
	gcc -Wall $dir/filegraffiti.c -o $dir/filegraffiti
fi

if [ ! -d $1 -o ! -r $1 ]
then
	echo "nu am drepturi" >2 
	exit 4
fi

dir=$(dirname $0)/legal_arts
$dir/iwashere.sh $1 | sort -r | tr -d '/'

