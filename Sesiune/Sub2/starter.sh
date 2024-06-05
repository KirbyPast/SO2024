#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./starter.sh [path]"
    exit 2
fi

./workers/worker2 &

fis=$(realpath $1)

sleep 3

./manager/supervisor $fis

exit 0
