#!/bin/bash


function compilare()
{
	output=$(dirname $1)/$(basename $1 .c)
	gcc $1 -o $output -Wall
}

function afisare()
{
	echo "Continutul fisierului $1 este: "
	cat $1
}

if [ $# -eq 0 ]
then
	read -p "Introduceti path-ul catre folder: " dir
else
	dir=$1;
	shift;
fi

if [ ! -d $dir -o ! -r $dir ]
then
	echo "Eroare! path-ul nu este un folder sau nu aveti drepturi!"
	exit 1;
fi

for i in $(ls -A $dir)
do
	
	case $i in
		*.txt  ) echo "fisier text: $dir/$i" ; afisare $dir/$i;;
		*.c  ) echo "fisier sursa: $dir/$1" ; compilare $dir/$i;;
	esac
done
