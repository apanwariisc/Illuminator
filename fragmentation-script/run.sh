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
# Populate page-cache by reading files into memory. This is an important step
# to start process execution in a fragmentated state. There needs to be sufficient
# file(s) in this directory (or symbolic links to files) so that the entire memory
# gets populated with OS page cache.
# Load files mutiple times to warmup the cache.
./cache.sh
./cache.sh
