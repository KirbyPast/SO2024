#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./starter.sh [path]"
    exit 1
fi



fis=$(realpath $1)

./subordinates/worker1 &

sleep 2

./coordonator/supervisor $fis

exit 0
