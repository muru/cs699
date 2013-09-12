#!/bin/bash

# Run this script from the same directory as iitb-papers.sh to test
# it; note that the tests are only a guideline and may not be complete

# Marks: 5

echo "Expected output of ./iitb-papers.sh papers-example1.csv"
echo "-------------------- Output start (not part of output) --------------------"
echo "2"
echo "-------------------- Output end (not part of output) --------------------"

echo "Actual output of ./iitb-papers.sh papers-example1.csv"
echo "-------------------- Output start (not part of output) --------------------"
./iitb-papers.sh "papers-example1.csv"
echo "-------------------- Output end (not part of output) --------------------"

echo "Expected output of ./iitb-papers.sh papers-example2.csv"
echo "-------------------- Output start (not part of output) --------------------"
echo "1"
echo "-------------------- Output end (not part of output) --------------------"

echo "Actual output of ./iitb-papers.sh papers-example2.csv"
echo "-------------------- Output start (not part of output) --------------------"
./iitb-papers.sh "papers-example2.csv"
echo "-------------------- Output end (not part of output) --------------------"

echo "Expected output of ./iitb-papers.sh papers-example3.csv"
echo "-------------------- Output start (not part of output) --------------------"
echo "3"
echo "-------------------- Output end (not part of output) --------------------"

echo "Actual output of ./iitb-papers.sh papers-example3.csv"
echo "-------------------- Output start (not part of output) --------------------"
./iitb-papers.sh "papers-example3.csv"
echo "-------------------- Output end (not part of output) --------------------"
