#!/bin/bash
echo $?
if [ $# -eq 0 ]
then
	read -p "Introduceti path-ul fisierului .c: " fis;
else
	fis=$1;
fi

answer=n;
nano -c $fis;

while [ "$answer" = "n" ]
do
	read -p "Doriti sa lansati compilatorul? Y/N: " answer;
	case $answer in
		y  ) gcc -Wall $fis -o $(basename $fis .c); 
		     err=$(gcc -Wall $fis -o $(basename $fis .c) 2>&1 | grep -c 'error') ;;
		n  ) nano -c $fis;;
	esac
	if [ $err -eq 0 -a "$answer" = "y" ]
	then
		answer=y;
	else
		answer=n;
	fi
done

read -p "Doriti sa executati programul? Y/N: " answer;
case $answer in
	y  ) ./$(basename $fis .c);;
esac
