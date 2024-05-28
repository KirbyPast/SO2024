#!/bin/bash

function factorial()
{
	if [ $1 -le 1 ]
	then
		echo $2;
	else
		let n_1=$1-1
		let prod=$1*$2
		factorial $n_1 $prod
	fi
}


if [ $# -eq 0 ]
then
	read -p "Introduceti numarul: " n;
else
	n=$1;
	shift;
fi

factorial $n 1

