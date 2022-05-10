#!/bin/bash

g++ program1.cpp -o p1.out
g++ program2.cpp -o p2.out 
./p2.out $1 $2 $3 $4 $5 $6 $7 $8 $9 

rm p1.out
rm p2.out
