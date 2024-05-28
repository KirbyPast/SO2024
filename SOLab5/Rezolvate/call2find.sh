#!/bin/bash

#Citire

if [ $# -eq 0 ]
then
	read -p "Dati path-ul catre fisier: " path
	[ -z $path ] && path="."
else
	path=$1
fi

if [ ! -d $path -o ! -r $path ]
then
	echo "Eroare path!"
	exit 1
fi

lista=$(find $path -type f -name "*.sh")

for fis in $lista
do
	echo $fis
	grep -n '#' $fis | grep -v '$#' | grep -v '#!'
done

exit 0 #Succes! 
