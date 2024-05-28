#!/bin/bash

if [ $# -eq 0 ]
then
	echo "Dati path-ul catre fisier: " path

#ups
#abcbcbc
	[ -z $path ] && path="." #daca nu introducem path folosim directorul curent
