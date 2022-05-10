#!/bin/bash

g++ -pthread timedlock.cpp -o executable_timedlock.out
./executable_timedlock.out
rm executable_timedlock.out
