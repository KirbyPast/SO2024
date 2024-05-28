#!/bin/bash

function compilare()
{
	basefis= basename $1 .c
	g++ $dir/$1 -o $basefis -Wall
}

if [ $# -eq 0 ]
then
	read -p "Introduceti path-ul catre fisier: " dir;
else
	dir=$1;
	shift;
fi

if [ ! -d $dir -o ! -r $dir ]
then
	echo "Eroare! Fisierul nu este un folder, sau nu aveti drepturi de citire!"
	exit 1;
fi 

for fis in $( ls $dir )
do
	case $fis in
		*.c  ) compilare $fis ;;
	esac
done

