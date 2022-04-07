#!/bin/bash

#g++ program1.cpp -o p1.out
g++ program2.cpp -o p2.out 
./p2.out $1 $2 $3

rm p1.out
rm p2.out

# Author 
printf '_%.0s' {1..100}
echo -e "@author github.com/AlimKugot/Operating-System"
