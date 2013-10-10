#!/usr/bin/python

import sys
import csv
import re
import itertools

def add_sum_marks(row):
	num_row = [int(x) if x.isdigit() else 0 for x in row]
	row.append(sum(num_row[1:-1]))

def add_test_num(row, index, test_count):
	if row[index].isdigit():
		test_count[index] = test_count[index] + 1

if len(sys.argv) != 2:
	print('Usage: ' + sys.argv[0] + ' <CSVFILE>')
	quit(-1)

with open(sys.argv[1]) as csvfile:
	csvreader = csv.reader(csvfile)
	data = [row for row in csvreader]
# Ex 1/2
	number = re.compile('^\d+$')
	header = [row for row in data if not re.match(number, row[0])]
	titles = filter(lambda row: 'Sno' in row, header)[0]
	candidates = [row for row in data if re.match(number, row[0])]
	print(len(candidates))
# Ex 3/4
	selection = filter(lambda row: int(row.pop()) == 1, candidates)
	print(len(selection))
# Ex 5
	headerindex =  {v:i for i, v in enumerate(titles)}
	for label, index in headerindex.items():
		print(label, index)
# Ex 6
	map(add_sum_marks, candidates)
	candidates.sort(key = lambda row: row[-1], reverse=True)
	outfile = open('mmarks.csv','w')
	csvwriter = csv.writer(outfile,quoting=csv.QUOTE_ALL)
	csvwriter.writerows(candidates)
# Ex 7
	OverallM = candidates[:]
	top20pM = OverallM[:len(OverallM)/5]
	bot40pM = OverallM[3*len(OverallM)/5:]
	print(top20pM)
	print(bot40pM)
# Ex 8
	test_count = {'Overall' : [0]*len(titles), 'top20p' : [0]*len(titles), 'bot40p' : [0]*len(titles)}
	for label, index in headerindex.items():
		if label not in ['Sno','Sel']:
			map(lambda row: add_test_num(row, index, test_count['Overall']), OverallM)
			map(lambda row: add_test_num(row, index, test_count['top20p']), top20pM)
			map(lambda row: add_test_num(row, index, test_count['bot40p']), bot40pM)
	print(test_count)


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
