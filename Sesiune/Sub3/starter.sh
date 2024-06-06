#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./starter.sh [path]"
    exit 1
fi

gcc -Wall master/supervisor.c -o master/supervisor
gcc -Wall slaves/worker1.c -o slaves/worker1
gcc -Wall slaves/worker2.c -o slaves/worker2

fis=$(realpath $1)

./slaves/worker2 &

sleep 3

./master/supervisor $fis

exit 0