#!/bin/bash

# Install all applications needed for build
echo "Installing applications from Ubuntu repo"
sudo apt-get update
sudo apt-get install git make gcc g++ gawk screen genext2fs u-boot-tools libncurses-dev bzip2 fakeroot

# Install ARM compiler
sudo apt-get install gcc-arm-linux-gnueabi

# Download tools for compiling U-Boot environment
echo "Downloading tools for compiling u-boot"
cd $HOME
git clone git://github.com/raspberrypi/tools rpi-tools

# Decompress Codesourcery Compiler if in scripts directory
echo "Downloading Code Sourcery cross-compiler"
cd $HOME
wget https://sourcery.mentor.com/GNUToolchain/package12813/public/arm-none-linux-gnueabi/arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

echo "Unpacking Code Sourcery Compiler"
bzip2 -d arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
tar xvf arm-2014.05-29-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar

# Update path variable for /home/user/arm-2015.05/bin
echo "Adding toolchains to path"
echo 'export PATH=$PATH:$HOME/arm-2014.05/bin:$HOME/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin' >> $HOME/.bashrc
source $HOME/.bashrc

