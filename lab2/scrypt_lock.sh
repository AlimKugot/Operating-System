#!/bin/bash

g++ -pthread lock.cpp -o executable_lock.out
./executable_lock.out
rm executable_lock.out


