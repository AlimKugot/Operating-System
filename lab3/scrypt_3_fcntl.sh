#!/bin/bash

g++ -pthread program3_fcntl.cpp -o executable_program3_fcntl.out
./executable_program3_fcntl.out

rm -f executable_program3_fcntl.out
