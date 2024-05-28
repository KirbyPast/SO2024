#!/bin/bash
dir=$1
function parcurgere_director ()
{
    for fis in $(ls -A $1)
    do
        # Construim calea până la intrarea $fis din directorul $1
        cale=$1/$fis   # Este suficient doar atât, căci argumentul primului apel nu va putea fi cuvântul vid niciodată! (Motivul: a se vedea primul if din script)
        # Procesăm calea respectivă în funcție de tipul ei, i.e. subdirector/fișier fifo.
	if [ -d $cale ] ; then
		parcurgere_director $cale
        elif [ -f $cale ] ; then
            file $cale | grep "Bourne-Again shell script" >/dev/null 2>&1 
		if [ $? -eq 0 ]
		then
			md5=$(md5sum $cale)
			echo $md5
			let nrbash+=1
		else
			wc -l $cale>&2
		fi
        fi
    done
}
parcurgere_director $dir  
echo  "Numarul total de scripturi bash parcurse: $nrbash"
if [ $nrbash -eq 0 ]
then
	exit -1
else
	exit 0
fi
echo "$?"
