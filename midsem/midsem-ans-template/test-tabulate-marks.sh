#!/bin/bash

# Run this script from the same directory as tabulate-marks.sh to test
# your solution

# Marks: 2

echo "Expected output of ./tabulate-marks.sh | sort"
echo "-------------------- Output start (not part of output) --------------------"
cat <<EOF
cpnew.sh:# Marks: 4
genplot.sh:# Marks: 6
iitb-papers.sh:# Marks: 5
tabulate-marks.sh:# Marks: 2
zrn.sh:# Marks: 2
EOF
echo "-------------------- Output end (not part of output) --------------------"

echo "Actual output of ./tabulate-marks.sh | sort"
echo "-------------------- Output start (not part of output) --------------------"
./tabulate-marks.sh | sort
echo "-------------------- Output end (not part of output) --------------------"

echo "Expected number of code lines of ./tabulate-marks.sh"
echo "-------------------- Output start (not part of output) --------------------"
echo "1"
echo "-------------------- Output end (not part of output) --------------------"

echo "Actual number of code lines of ./tabulate-marks.sh"
echo "-------------------- Output start (not part of output) --------------------"
sed '/^[ \t]*$/d' ./tabulate-marks.sh | sed '/^[ \t]*#/d' | wc -l
echo "-------------------- Output end (not part of output) --------------------"

