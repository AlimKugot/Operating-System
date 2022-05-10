#!/bin/bash

g++ -pthread trylock.cpp -o executable_trylock.out
./executable_trylock.out
rm executable_trylock.out


