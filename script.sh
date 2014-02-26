#!/bin/csh

for time in 1 30 60 90 120
do 
	for procs in 1 2 5 10 15 20 100 200 500 1000 
	do
		./assign2 -p $procs -t $time
	done
done
