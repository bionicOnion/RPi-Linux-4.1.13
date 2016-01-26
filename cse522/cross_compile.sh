#!/bin/bash

CSV_NAME="cse522_build_times.csv"

DATE_STRING=`date +"%m-%d-%y-%H-%M"`
KERNEL=kernel7

mkdir -p logs

# Generate CSV to keep track of build times
if [ ! -f logs/$CSV_NAME ]; then
    touch logs/$CSV_NAME
    echo "\"Start Time\",\"End Time\",\"Elapsed Seconds\"" >> "logs/"$CSV_NAME
fi

cd ..

echo "Cleaning"
make clean

echo "Performing Cross Compile"
START_SECONDS=`date +"%s"`
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4 zImage modules dtbs | tee "cse522/logs/"$DATE_STRING"_cross_compile.log" 2>&1
END_SECONDS=`date +"%s"`

# Calculate Elapsed Seconds
ELAPSED_SECONDS=`expr $END_SECONDS - $START_SECONDS`

START_DATE=`date --date="@$START_SECONDS"`
END_DATE=`date --date="@$START_SECONDS"`

# Export time to the csv
echo "\"$START_DATE\",\"$END_DATE\",\"$ELAPSED_SECONDS\"" >> "cse522/logs/"$CSV_NAME

echo "Start: "$START_SECONDS", End: "$END_SECONDS", Elapsed: "$ELAPSED_SECONDS

cd cse522
