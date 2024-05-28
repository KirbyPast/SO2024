#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Introduceti primul numar: " n;
	read -p "Introduceti al doilea numar: " m;
else
	n=$1;
	m=$2;
fi

x=$n
y=$m
let n=0;
until [ $m -eq 0 ]
do
	let n=$n+$x;
	let m=$m-1;
done

echo "$x * $y = $n"
