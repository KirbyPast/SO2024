#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./starter.sh [path]"
    exit 0
fi

gcc -Wall  app/worker1.c -o app/worker1
gcc -Wall  app/supervisor.c -o app/supervisor
gcc -Wall  app/components/worker2.c -o app/components/worker2

./app/components/worker2 &

sleep 2

fis=$(realpath $1)

./app/worker1 $fis