#!/bin/bash

CSE522_DIR=`pwd`
CSV_NAME="cse522_build_times.csv"
LINUX_ROOT="$CSE522_DIR/../linux"
LOGS_DIR="$CSE522_DIR/logs"
BUILD_LOG_TEMPLATE="cross_compile"

DATE_STRING=`date +"%m-%d-%Y-%H-%M"`
KERNEL=kernel7

# build the log directory
mkdir $LOGS_DIR >> /dev/null 2>&1

# create the file for logs
BUILD_LOG=$LOGS_DIR"/"$DATE_STRING"_"$BUILD_LOG_TEMPLATE".log"
touch $BUILD_LOG

# Print banner
clear
echo "---------------------------------------------------------" | tee -a $BUILD_LOG 2>&1
echo "            Cross Compile - $DATE_STRING                 " | tee -a $BUILD_LOG 2>&1
echo "---------------------------------------------------------" | tee -a $BUILD_LOG 2>&1
echo "Linux Root: "$LINUX_ROOT                                   | tee -a $BUILD_LOG 2>&1
echo "Logs Root: "$LOGS_DIR                                      | tee -a $BUILD_LOG 2>&1
echo "CSV Location: "$LOGS_DIR"/"$CSV_NAME                       | tee -a $BUILD_LOG 2>&1
echo ""                                                          | tee -a $BUILD_LOG 2>&1

# Generate CSV to keep track of build times
cd $CSE522_DIR
if [ ! -f $LOGS_DIR/$CSV_NAME ]; then
    touch $LOGS_DIR/$CSV_NAME
    echo "\"Start Time\",\"End Time\",\"Elapsed Seconds\",\"Build Log\"" >> $LOGS_DIR/$CSV_NAME
fi

echo "Cleaning Build Tree"
cd $LINUX_ROOT
make clean

echo "Performing Cross Compile"
START_SECONDS=`date +"%s"`
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j8 zImage modules dtbs | tee -a $BUILD_LOG 2>&1
END_SECONDS=`date +"%s"`

# Calculate Elapsed Seconds
ELAPSED_SECONDS=`expr $END_SECONDS - $START_SECONDS`
START_DATE=`date --date="@$START_SECONDS"`
END_DATE=`date --date="@$START_SECONDS"`

# Export time to the csv

echo "Exporting Records - See "$LOGS_DIR"/"$CSV_NAME
echo "\"$START_DATE\",\"$END_DATE\",\"$ELAPSED_SECONDS\",\"$BUILD_LOG\"" >> $LOGS_DIR"/"$CSV_NAME

echo "Start: "$START_SECONDS", End: "$END_SECONDS", Elapsed: "$ELAPSED_SECONDS

# return to origin directory
cd $CSE522_DIR
