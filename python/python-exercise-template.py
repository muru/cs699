#!/usr/bin/python3

import sys
import csv
import re
import itertools
import math

def add_sum_marks(row):
	num_row = [int(x) if x.isdigit() else 0 for x in row]
	row.append(sum(num_row[1:-1]))
	return row

def add_test_num(row, index, test_count):
	if row[index].isdigit():
		test_count[index] = test_count[index] + 1

def avg(X):
	return math.fsum(X) / len(X)

def variance(X):
	X2 = [x*x for x in X]
	return avg(X2) - avg(X)

def std_dev(X):
	return math.sqrt(variance(X))

def covariance(X, Y):
	XY = [x * y for (x, y) in zip(X, Y)]
	return avg(XY) - avg(X) * avg(Y)

def correlation(X, Y):
	return covariance(X, Y) / (std_dev(X) * std_dev(Y))

if len(sys.argv) != 2:
	print('Usage: ' + sys.argv[0] + ' <CSVFILE>')
	quit(-1)

with open(sys.argv[1]) as csvfile:
	csvreader	= csv.reader(csvfile)
	data		= list(csvreader)  #[row for row in csvreader]
# Ex 1/2
	number		= re.compile('^\d+$')
	header		= [row for row in data if not re.match(number, row[0])]
	titles		= [row for row in header if 'Sno' in row][0]
	candidates	= [row for row in data if re.match(number, row[0])]
	print(len(candidates))
# Ex 3/4
	selection	= list(filter(lambda row: int(row[-1]) == 1, candidates))
	print(len(selection))
# Ex 5
	headerindex =  {v:i for i, v in enumerate(titles)}
	for label, index in headerindex.items():
		print(label, index)
# Ex 6
	cand_extra	= list(map(add_sum_marks, candidates))
	cand_extra.sort(key = lambda row: row[-1], reverse = True)
	outfile		= open('mmarks.csv','w')
	csvwriter	= csv.writer(outfile,quoting=csv.QUOTE_ALL)
	csvwriter.writerows(cand_extra)
# Ex 7
	OverallM	= cand_extra[:]
	top20pM		= OverallM[:len(OverallM)//5]
	bot40pM		= OverallM[3*len(OverallM)//5:]
	matrices	= {'OverallM': OverallM, 'top20pM': top20pM, 'bot40pM': bot40pM}
	print(top20pM)
	print(bot40pM)
# Ex 8
	#test_counts = {'OverallM' : [0]*len(titles), 'top20pM' : [0]*len(titles), 'bot40pM' : [0]*len(titles)}
	test_counts = {group: ([0]*len(titles))[:] for group in matrices}
	for label in headerindex:
		if label not in ['Sno','Sel']:
			for group in matrices:
				for entry in matrices[group]:
					add_test_num(entry, headerindex[label], test_counts[group])
	print(test_counts)
# Ex 9
	total_marks = [entry[-1] for entry in cand_extra]
	is_selected = [entry[-1] for entry in candidates]
	print(correlation(total_marks, is_selected))

# 7.
#	OverallM = candidates[:]
#	top20pM = OverallM[:len(OverallM)/5]
#	bot40pM = OverallM[3*len(OverallM)/5:]
#	print(top20pM)
#	print(bot40pM)

# 6.
#	map(add_sum_marks, candidates)
#	candidates.sort(key = lambda row: row[-1])
#	outfile = open('mmarks.csv','w')
#	csvwriter = csv.writer(outfile)
#	csvwriter.writerows(candidates)

# 5.
#	header = filter(lambda row: 'Sno' in row, csvreader)
#	headerindex =  {v:i for i, v in enumerate(header[0])}
#	for headerentry, index in headerindex.items():
#		print(headerentry, index)

# 4.
#	selection = filter(lambda row: int(row.pop()) == 1, candidates)
#	print(len(selection))

# 3.
#	isSelected = map(lambda row: int(row.pop()), candidates)
#	print(sum(isSelected))

# 2.
#	number = re.compile('^\d+$')
#	candidates = [row for row in csvreader if re.match(number, row[0]) != None]
#	print(len(candidates))

# 1.
#	numCandidates = 0
#	for row in csvreader:
#		if row[0].isdigit():
#			numCandidates = numCandidates + 1
