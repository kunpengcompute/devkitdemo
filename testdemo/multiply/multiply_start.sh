#!/bin/bash

for i in {1..200}
do
	taskset -c 1 ./multiply
done
