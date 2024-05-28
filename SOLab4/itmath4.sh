#!/bin/bash

declare -a v;

#CITIRE

if [ $# -eq 0 ]
then
	read -p "Introduceti numarul de elemente: " n;
	while [ $n -gt 0 ]
	do
		read -p "Introduceti un numar: " x;
		let n=$n-1;
		let v[$x]=v[$x]+1;
	done
else
	while [ $# -gt 0 ]
	do
		let v[$1]=v[$1]+1;
		shift;
	done
fi

maxi=0;
c=0;

for i in ${!v[@]}
do
	c=${i};
	if [ ${v[$i]} -gt $maxi ]
	then
		maxi=${v[$i]};
		imax=$c
	fi
done

echo "Numarul care apare de cele mai multe ori este $imax, si apare de ${v[$imax]} ori."
