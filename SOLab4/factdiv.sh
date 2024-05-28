#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Introduceti numarul 1: " n;
	read -p "Introduceti numarul 2: " m;
else
	n=$1;
	m=$2;
	shift;
	shift;
fi

function putere()
{
	echo "$1,$2"
	if [ $1 -eq 1 ]
	then
		echo "Adevarat"
	else if [ $1 -lt 1 ]
		then
			echo "Fals"
		else
			let m_1=$1/$2
			putere $m_1 $2
		fi
	fi
}

putere $n $m
