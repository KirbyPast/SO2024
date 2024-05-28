#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Dati calea de intrare catre programul care va fi executat: " spmd
else
	spmd=$1
fi

if [ ! -f $spmd -o ! -x $spmd ]
then
	echo "Eroare path"
	exit 1 # Fail!
fi

shift

if [ $# -eq 0 ]
then
	read -p "Specificati de cate ori sa se execute programul: " n
else
	n=$1
fi

if [ ! $n -gt 0 ]
then
	echo "Eroare: numarul de instante trebuie sa fie mai mare decat 1!"
	exit 2 # Fail!
fi

shift

declare -a val

for i in $(seq 1 $n)
do
	if [ $# -eq 0 ]
		then
			read -p "Dati argumentul val[$i]: " val[$i]
		else
			val[$i]=$1
			shift
	fi
done

for i in $(seq 1 $n)
do
	$spmd ${val[$i]} &
done

echo "Started!"

wait

echo "Finished!"
