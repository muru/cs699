#! /bin/bash

PROJDIR=~/cs699/project/133059001	# Set to the absolute path of the working directory.

# FLOWS=`grep 'Unique flow requests received at root' results.txt | sed 's/.*[0-9]+//'`
# ADMITTED_FLOWS=`grep 'Unique calls admitted' results.txt | sed 's/.*[0-9]+//'`
# DROPPED_FLOWS=`grep 'Admitted - Successful' results.txt | sed 's/.*[0-9]+//'`

FLOW_DATA=`grep -f- outputFiles/results.txt <<EOF | 
Unique flow requests received at root
Unique calls admitted
Dropped after root
EOF
sed 's/[^0-9]*//'`
OUT=$1
shift

echo $@ $FLOW_DATA >>$OUT

for flow in flows/flow*
do
	# Flow setup latency 
	START_TIME=`grep Time_Flow_Request_Sent $flow | cut -d' ' -f3`
	ACCEPT_TIME=`grep Time_Flow_Request_Granted $flow | cut -d' ' -f3`
	N_HOPS=`grep No_Of_Hops $flow | cut -d' ' -f3`

	if [[ $ACCEPT_TIME > 0 ]]
	then
		LATENCY=`bc -ll <<EOF
		$ACCEPT_TIME - $START_TIME
EOF`
		PACKETS_SENT=`grep -c PACKET_SENT_BY_SOURCE $flow`
		PACKETS_LOST=`grep -c CHANNEL_PACKET_LOSS $flow`
		AVG_PACKET_LOSS=`bc -ll <<EOF
		($PACKETS_SENT - $PACKETS_LOST) / $PACKETS_SENT.0
EOF`
		#grep PACKET_SENT_BY_SOURCE $flow | cut -d' ' -f 3 > start.txt
		grep destination $flow | cut -d' ' -f10 --output-delimiter=, | sort -n > end.txt
		$PROJDIR/calc_jitter.py end.txt | read AVG_JITTER MED_JITTER
	else
		LATENCY="inf"
		AVG_PACKET_LOSS=0
		AVG_JITTER=0
		MED_JITTER=0
	fi
	echo $@ $flow $N_HOPS $LATENCY $AVG_PACKET_LOSS $AVG_JITTER $MED_JITTER
	#read
done
