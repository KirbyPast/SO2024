#!/bin/bash

w -h | tr -s " " | cut -d" " -f1,8 | sort -t " " -k 1
