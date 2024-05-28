#!/bin/bash

i=0
for parametru in $@
do
	let i=i+1
	echo "param_$i = $parametru"
done
