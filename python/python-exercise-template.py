#!/usr/bin/python3

import sys
import csv
import re
import math

def add_sum_marks(row):
	num_row = [int(x) if x.isdigit() else 0 for x in row]
	# row.append(sum(num_row[1:-1]))
	return sum(num_row[1:-1])

def add_test_num(row, index, test_count):
	if row[index].isdigit():
		test_count[index] = test_count[index] + 1

def avg(X):
	return math.fsum(X) / len(X)

def variance(X):
	X2 = [x**2 for x in X]
	return avg(X2) - (avg(X)**2)

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

# Ex 1/2
with open(sys.argv[1]) as csvfile:
	csvreader	= csv.reader(csvfile)
	data		= list(csvreader)  #[row for row in csvreader]
	number		= re.compile('^\d+$')
	header		= [row for row in data if not re.match(number, row[0])]
	labels		= [row for row in header if 'Sno' in row][0]
	candidates	= [row + [ add_sum_marks(row) ] for row in data if re.match(number, row[0])]
	num_cand	= len(candidates)
	print(num_cand)
# Ex 3/4
	selection	= list(filter(lambda row: int(row[-1]) == 1, candidates))
	print(len(selection))
# Ex 5
	labelindices =  {v:i for i, v in enumerate(labels)}
	for label, index in labelindices.items():
		print(label, index)
# Ex 6
	outfile		= open('mmarks.csv','w')
	csvwriter	= csv.writer(outfile,quoting=csv.QUOTE_ALL)
	candidates.sort(key = lambda row: row[-1], reverse = True)
	csvwriter.writerows(candidates)
# Ex 7
	OverallM	= candidates[:]
	top20pM		= OverallM[:num_cand//5]
	bot40pM		= OverallM[3*num_cand//5:]
	matrices	= {'OverallM': OverallM, 'top20pM': top20pM, 'bot40pM': bot40pM}
	print(top20pM)
	print(bot40pM)
# Ex 8
	test_counts = {group: ([0]*len(labels))[:] for group in matrices}
	for label in labelindices:
		if label not in ['Sno','Sel']:
			for group in matrices:
				for entry in matrices[group]:
					add_test_num(entry, labelindices[label], test_counts[group])
	print(test_counts)
# Ex 9
	total_marks = [entry[-1] for entry in candidates]
	is_selected = [int(entry[-2]) for entry in candidates]
	print(covariance(total_marks, is_selected))

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
# --
#	cand_extra	= list(map(add_sum_marks, candidates))
#	cand_extra.sort(key = lambda row: row[-1], reverse = True)

# 5.
#	header = filter(lambda row: 'Sno' in row, csvreader)
#	labelindices =  {v:i for i, v in enumerate(header[0])}
#	for headerentry, index in labelindices.items():
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
