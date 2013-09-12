#!/bin/bash

# Run this script from the same directory as cpnew.sh
# This script will generate a test case for cpnew.sh and run cpnew.sh
# against it

# Marks: 4

rm -rf "testsrc"
rm -rf "testdst"

mkdir "testsrc"
mkdir "testdst"

echo "Only in src" > "testsrc/a.txt"

echo "Older in dst" > "testdst/b.txt"
sleep 1
echo "Newer in src" > "testsrc/b.txt"

echo "Older in src" > "testsrc/c.txt"
sleep 1
echo "Newer in dst" > "testdst/c.txt"

./cpnew.sh "testsrc" "testdst"

echo "Expected output of ls testdst | sort"
echo "-------------------- Output start (not part of output) --------------------"
echo "a.txt"
echo "b.txt"
echo "c.txt"
echo "-------------------- Output end (not part of output) --------------------"

echo "Actual output of ls testdst | sort"
echo "-------------------- Output start (not part of output) --------------------"
ls "testdst" | sort
echo "-------------------- Output end (not part of output) --------------------"

echo "Expected output of cat testdst/a.txt testdst/b.txt testdst/c.txt"
echo "-------------------- Output start (not part of output) --------------------"
echo "Only in src"
echo "Newer in src"
echo "Newer in dst"
echo "-------------------- Output end (not part of output) --------------------"

echo "Actual output of cat testdst/a.txt testdst/b.txt testdst/c.txt"
echo "-------------------- Output start (not part of output) --------------------"
cat testdst/a.txt testdst/b.txt testdst/c.txt
echo "-------------------- Output end (not part of output) --------------------"
