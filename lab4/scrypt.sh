#!/bin/bash

NC='\033[0m' # no color
GREEN='\033[0;32m'

echo -e "${GREEN}Don't forget enter your arg values! (max = 9)${NC}"

g++ program1.cpp -o p1.out
g++ program2.cpp -o p2.out 
./p2.out $1 $2 $3 $4 $5 $6 $7 $8 $9 

rm p1.out
rm p2.out
