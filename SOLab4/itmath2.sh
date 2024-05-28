#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Introduceti numarul de numere: " n;
else
	n=$1;
	shift;
fi

x=$n;
declare -a v
i=1;

while [ $x -gt 0 ]
do
	let x=x-1;
	if [ $# -eq 0 ]
	then
		read -p "Introduceti al $i-lea numar: " v[$i]
	else
		v[$i]=$1;
	fi
	shift;
	let i=i+1;
done

for i in ${v[@]}
do
	let sum=sum+i;
done

rez= echo "$sum/$n" | bc -l

echo "$rez"
