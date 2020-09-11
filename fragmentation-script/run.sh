#!/bin/bash

#-------------------Step-1-----------------------
# Allocate kernel and user memory one after the other a few times.
for i in {1..25}
do
	insmod alloc-module.ko iteration=2
	rmmod alloc-module.ko
	./a.out 0
	echo "Iteration $i Completed Successfully" 
done

#---------------------------Step-2-------------------------------
# Populate page-cache by reading files into memory
./cache.sh
./cache.sh
