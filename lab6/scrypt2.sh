#!/bin/bash

g++ -pthread program2.cpp -o p2.out -lrt
./p2.out
rm p2.out
