#!/bin/bash
#abc
if [ $# -le 0 ]
then
    read -p "Primul numar: " a
else
    a=$1
fi

if [ $# -le 1 ]
then
    read -p "Al doilea numar: " b
else
    b=$2
fi

function euclid ()
{
a=$1
b=$2
if ((a!=b))
then
    #if [ $a -gt $b ]
    if ((a>b))
    then
	let a_1=$a-$b
	euclid $a_1 $b
	return $?
    else
	let b_1=$b-$a
	euclid $a $b_1
	return $?
    fi
else
    return $1
fi
}
echo -n "euclid($a,$b)="
euclid $a $b
echo "$?"
