#!/bin/bash

declare -a v

if [ $# -eq 0 ]
then
	read -p "Lungimea secventei de nr: " n 
	for i in $(seq 1 $n)
	do
		read -p "Dati al $i-lea numar: " v[$i]
	done
	read -p "Dati numarul p: " p
	for i in $(seq 1 $n)
	do
		if ((${v[$i]}<=$p))
		then
			echo -n "${vec[$i]} "
		fi
	done
else
	pivot=${*: -1}
	for i in $(seq 1 $(($# - 1)))
	do
		if (($1 <= pivot))
		then
			echo -n "$1 "
		fi
		shift
	done
fi
