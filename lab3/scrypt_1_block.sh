#!/bin/bash

g++ -pthread program1_block.cpp -o executable_program1.out
./executable_program1.out
rm executable_program1.out
