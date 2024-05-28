#!/bin/bash

read -a vec

sum=0
for num in ${vec[@]}
do
	let sum+=num**3
done

if [ $sum -eq 0 ]
then
	echo kaboom
	exit 1
fi

echo $sum
exit 0
