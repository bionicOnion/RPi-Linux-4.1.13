#!/bin/bash

KERNEL=kernel7

cd ..

echo "Cleaning"
make clean

echo "Generating Configuration"
make bcm2709_defconfig
make menuconfig

echo "Performing the build"
date >> time.txt
make -j4 zImage modules dtbs
date >> time.txt

cd cse522

