#!/bin/bash

#CITIRE ARGUMENTE

declare -a v
i=0
if [ $# -eq 0 ]
then
	read -p "Specificati numarul de elemente ale secventei: " n;
	cnt=$n;
	while [ $cnt -gt 0 ]
	do
		read -p "Introduceti un numar: " v[$i] 
		let i=i+1;
		let cnt=cnt-1;
	done
else
	while [ $# -gt 0 ]
	do
		v[$i]=$1;
		shift;
		let i=i+1;
	done
fi

#CAUTARE MINIM/MAXIM

min=${v[$1]};
max=${v[$1]};
for i in ${v[@]}
do
	if [ $i -gt $max ]
	then
		max=$i;
	fi
	if [ $i -lt $min ]
	then
		min=$i;
	fi
done

echo "Minimul este $min, iar maximul este $max" 
