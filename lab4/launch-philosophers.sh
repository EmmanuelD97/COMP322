#!/bin/bash

#Emmanoel Dermkrdichyan
#used to run lab4


execute=./dining-p
seats=$1
childpids=()

if [$seats -lt 0] ; then
	echo "number of seats must be possitive!"
	exit -1;
fi

handler(){
	for j in {0..$seats}
	do
		kill -TERM $childpids[i] 2>/dev/null
	done
	exit 0
}

trap 'handler' TERM

for i in {0..$seats}
do
	$execute $seats $i &
	childpids[$i]=$! &
	echo " " childpids[$i]
done
