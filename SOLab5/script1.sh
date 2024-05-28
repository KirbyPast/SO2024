#!/bin/bash

dir=$1
if [ ! -d $dir ]
then
	echo "Eroare: nu exista fisierul!" >&2
	exit 1
fi

if [ ! -x $dir -o ! -r $dir ]
then
	echo "Eroare: nu aveti drepturi!" >&2
	exit 2
fi

./subfolder/script2.sh $dir

echo $?
