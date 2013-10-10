#!/usr/bin/python

import sys
import csv

if len(sys.argv) != 2:
	print('Usage: ' + sys.argv[0] + ' <CSVFILE>')
	quit(-1)

with open(sys.argv[1]) as csvfile:
	csvreader = csv.reader(csvfile)
	numCandidates = 0
	for row in csvreader:
		if row[0].isdigit():
			numCandidates = numCandidates + 1
	print(numCandidates)
