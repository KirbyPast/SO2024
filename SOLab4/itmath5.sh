#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Combinari de cate?: " n;
	read -p "Luate cate?: " k;
else
	n=$1;
	k=$2;
fi

if [ $k -gt $n ]
then
	echo "Error!: Combinari de n luate cate k, unde n > k";
	exit 1;
fi

i=1;
factNumit=1;
factNumar=1;

while [ $i -lt $k ]
do
	let factNumit=$factNumit*$i;
	let i=$i+1;
done
let factNumit=$factNumit*$i;
let i=$n-$k+1;
while [ $i -lt $n ]
do
	let factNumar=$factNumar*$i;
	let i=$i+1;
done

let factNumar=$factNumar*$i;
echo "$factNumar/$factNumit"| bc ;
