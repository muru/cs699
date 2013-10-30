#!/bin/bash

for i in `seq 1 3`
do
    ./trace-analysis.py anonP$i.csv > anonP$i.dat
done
