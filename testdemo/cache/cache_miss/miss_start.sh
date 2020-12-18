#!/bin/bash

for i in {1..20};
do
	taskset -c 1 ./cache_miss;
done
