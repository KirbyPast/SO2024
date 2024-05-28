#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Introduceti path-ul catre fisierul de scris: " fis;
else
	fis=$1;
	shift;
fi

echo > $fis;

let nr_op=0;

while(true)
do
	read -p "Introduceti primul numar: " a;
	read -p "Introduceti al doilea numar: " b;
	read -p "Introduceti operatorul: " op;
	if [ "$op" = "q" ]
	then
		break;
	fi
	case "$op" in
		"+"  ) let rez=a+b ;;
		"-"  ) let rez=a-b ;;
		"*"  ) let rez=a*b ;;
		"/"  ) rez=$(echo "$a / $b" | bc -l) ;;
	esac
	let nr_op++;
	echo "$nr_op: $a $op $b = $rez" >> $fis;
done



