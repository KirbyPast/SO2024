#!/bin/bash

if [ $# -lt 1 ]
then
	exit 2;
else
	fis=$1;
fi

#$(dirname $0 | sed 's/'scripts'/'programs'/' )/filetypeandsize $fis
~/.../filetypeandsize $fis
if [ $? -eq 1 ]
then
	for sub in "$fis"/*
	do
		echo $sub
		$0 $sub
	done
fi
