#!/usr/bin/python

# WiFi when used in the lab creates congestion.  To diagnose the
# problem, we have been collecting traces.  In this quiz, you will
# write a python script to analyse such a trace.  For the purpose of
# the quiz, the collected trace has been anonymized and greatly
# simplified.

# The python script is given the trace as input, in the form of a CSV
# file (similar to that in the python exercise)

# The first line in the csv file is the header, and each subsequent
# line corresponds to a packet transmission.  There are at least six
# columns:

# rx: this is the receiver's MAC address (anonymized)
# tx: this is the transmitter's MAC address (anonymized)
# len: this is the packet length or size in bytes
# type: this is the packet type (WiFi has 3 types of packets)
# rateMbps: this is the data rate or link speed at which packet was transmitted (WiFi supports multiple data rates; you can see this using iwconfig in linux)
# rssi: this is the received signal strength indicator

# Warning: Do not spend too much time trying to understand what each
# of the above fields mean; such understanding is necessary for actual
# analysis but not for this CS699 quiz

# Answer each of the parts below, ONLY between the BEGIN and END lines
# indicated.  If you do not adhere to this, you will likely get
# NEGATIVE MARKS.

#################### BEGIN_Q01 ####################

# Marks: 1

# import necessary modules
import sys
import csv

# Exit if the script does NOT have exactly one command-line argument
if (len(sys.argv) != 2):
	print('Usage: ' + sys.argv[0] + ' <FILE>')
	quit(-1)

filename = sys.argv[1] # from command line argument
filehandle = open(filename) # open the file
filecsv = csv.reader(filehandle) # use csv reader on filehandle

# This is a list of lists (i.e. matrix)
# Each inner list corresponds to one packet from the CSV file
Pkts = []

# This is a dictionary mapping from the column name (first row of CSV
# file) to the column number
colName2Num = {}

# Code to initialize Pkts and colName2Num is below
colName2Num = {v:i for i, v in enumerate(filecsv.next())}
Pkts = list(filecsv)

#################### END_Q01 ####################
#sys.exit(0) # Uncomment to test until Q01

type_of_interest = 0

#################### BEGIN_Q02 ####################

# Marks: 2
# 0.5 for correctness
# 1 for single line solution + 1 HP
# 0.5 for solution independent of order of columns in CSV file

# num_pkts_of_desired_type counts the number of packets in the trace,
# of type 'type_of_interest'

num_pkts_of_desired_type = len(filter(lambda pkt: int(pkt[colName2Num['type']]) == type_of_interest, Pkts))

#################### END_Q02 ####################
#print num_pkts_of_desired_type # Uncomment to test until Q02
#sys.exit(0) # Uncomment to test until Q02

rssi_thresh = -50

#################### BEGIN_Q03 ####################

# Marks: 3
# 1 for correctness
# 1.5 for single line solution + 2 HP
# 0.5 for solution independent of order of columns in CSV file

# num_bytes_below_rssi_thresh counts the number of bytes in all
# packets whose rateMbps is strictly below the 'rssi_thresh' threshold

num_bytes_below_rssi_thresh = sum([int(pkt[colName2Num['len']]) for pkt in Pkts if int(pkt[colName2Num['rssi']]) < rssi_thresh]) # Write code to get this

#################### END_Q03 ####################
#print num_bytes_below_rssi_thresh # Uncomment to test until Q03
#sys.exit(0) # Uncomment to test until Q03

D = {}

#################### BEGIN_Q04 ####################

# D is a dictionary which maps from packet rateMbps to the number of
# bytes of that specific rateMbps in the trace

# You have to define the 'mapping_func' below, so that the map call
# using this mapping_func, given below initializes D appropriately

# Marks: 2
# 1.5 marks for correctness
# 0.5 for solution independent of order of columns in CSV file

# Hint: we are NOT interested in the return value of the map call, but
# its side effect on D

def mapping_func(row):
	key = int(row[colName2Num['rateMbps']])
	val = int(row[colName2Num['len']])
	D[key] = val + D.get(key, 0)
    # Define function appropriately

#################### END_Q04 ####################

# Make sure to comment out any print statements you may have above,
# so that the print output below can be fed to gnuplot via plot.gnu

map(mapping_func, Pkts)
theKeys = D.keys()
theKeys.sort()
for k in theKeys:
    print str(k) + '\t' + str(D[k])

