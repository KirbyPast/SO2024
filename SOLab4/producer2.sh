#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Introduceti n: " n;
	read -p "Introduceti m: " m;
else
	n=$1;
	shift;
	m=$1;
	shift;
fi

if [ $n -gt 0 -a $m -gt 0 ]
then
	./consumer2.sh $n $m
else if [ $n -lt 0 -a $m -lt 0 ]
	then
		let n_1=-$n
		let m_1=-$m
		./consumer2.sh $n_1 $m_1
	else if [ $n -lt 0 -a $m -gt 0 ]
		then
			./consumer2.sh $n $m
		else if [ $n -gt 0 -a $m -lt 0 ]
			then
				./consumer2.sh $m $n
			fi
		fi
	fi
fi

echo "Codul de iesire al lui consumer2.sh este: $?"
