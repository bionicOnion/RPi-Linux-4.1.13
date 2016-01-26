#!/bin/bash

# Variables
START_DIR=`pwd`
LINUX_DIR=/home/cameron/Documents/CSE522/linux-4.1.13

# cd $LINUX_DIR

KERNEL=kernel7

# Develop Configurations
echo "Generating build configuration..."
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig

make menuconfig

echo "Performing kernel compilation..."
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 zImage modules dtbs

# Change directory back to original
cd $START_DIR

echo "Completed"
