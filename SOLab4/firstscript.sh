#!/bin/bash

for param in $@
do
	let i=i+1
	echo "param_$i = $param"
done


