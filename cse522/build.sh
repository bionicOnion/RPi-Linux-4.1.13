#!/bin/bash

KERNEL=kernel7

cd ../linux

echo "Cleaning"
make clean

echo "Performing the build"
date >> time.txt
make -j4 zImage modules dtbs
date >> time.txt

cd ../cse522

