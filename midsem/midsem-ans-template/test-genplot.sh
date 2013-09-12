#!/bin/bash

# This test script is intended to be run from the same directory as
# genplot.sh to test it

rm -rf "testgenplot"
mkdir "testgenplot"

cp "./genplot.sh" "thrpt-data.txt" "testgenplot/"
cd "testgenplot/"
chmod 444 "thrpt-data.txt"
export X_LABEL="Time in ms"
export TIME_UNIT="ms"
./genplot.sh "Test Title"
if [ -f "thrpt-plot.eps" ]; then
    echo "File thrpt-plot.eps generated in directory testgenplot"
    echo "Expected plot title is $TITLE"
    echo "Expected x label is $X_LABEL"
    echo "Expected x-axis unit is ms"
    echo -n "Hit enter to view eps and check..."
    read dummy
    echo "Invoking evince on thrpt-plot.eps"
    evince "thrpt-plot.eps"
else
    echo "Something seems to be wrong; no thrpt-plot.eps generated"
fi
