#!/bin/bash

for i in {0..27}
do
	gpio $1 $i $2
done
