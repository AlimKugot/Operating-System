#!/bin/bash

g++ -pthread program1.cpp -o p1.out -lrt
./p1.out
rm p1.out
