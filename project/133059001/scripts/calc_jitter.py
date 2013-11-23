#! /usr/bin/python3

import sys
import fileinput
import math

def avg(X):
	try:
		return math.fsum(X) / len(X)
	except e:
		print(e, file=sys.stderr)
		return 0

def median(X):
	s = X.sort()
	l = len(X)
	if l % 2 == 0:
		return X[l//2]
	else:
		return (X[l//2] + X[l//2 - 1]) / 2

rec_time = [float(time) for time in fileinput.input()]

n = len(rec_time)

if len(rec_time) < 3:
	print(0, 0)
else:
	prev_latency = rec_time[1] - rec_time[0]
	prev_time = rec_time[0]
	jitter = [0]*(n - 2)

	i = 0
	for time in rec_time[2:]:
		latency = time - prev_time
		prev_time = time
		jitter[i] = latency - prev_latency
		prev_latency = latency
		i = i + 1
	print(avg(jitter), median(jitter))

#with open(sys.argv[1]) as start_file, open(sys.argv[2]) as received_file:
#	received_csv = csv.reader(received_file)
#	start_time = [float(row) for row in start_file]
#	received_time = [[int(row[0]), float(row[1])] for row in received_csv]
#	if (len(start_time) == 0) or (len(received_time) == 0):
#		quit(-1)
#	num_packets = len(start_time)
#	latency = [float("nan")]*num_packets
#
#	for time in received_time:
#		index = time[0] - 1
#		latency[index] = time[1] - start_time[index]
#	rec_latency = [x for x in latency if not math.isnan(x)]
#
#	avg_latency = avg(rec_latency)
#	jitter = [x - avg_latency for x in rec_latency]
#	avg_jitter = avg(jitter)
#	median_jitter = median(jitter)
#	
#	print(avg_jitter, median_jitter)
