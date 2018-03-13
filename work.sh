#!/bin/bash

if [ "$1" = "commit" ];
then
	git add . -A --ignore-errors
	git commit -m [auto]$2 || true
	exit 0
fi

if [ "$1" = "push" ];
then
	git push origin master
	exit 0
fi



root=$(pwd)
alldir=$(ls | grep -E  "+[0-9]+")


for dir in $alldir
	do
		cd ./$dir
		echo 已经进入 $dir
		make  $1  || true
		cd $root
	
	done
