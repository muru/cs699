#! /bin/bash

PROJDIR=~/cs699/project/133059001
TESTDIR=$PROJDIR/tests
ARCHDIR=$PROJDIR/archives
DOCDIR=$PROJDIR/doc
SCRIPTDIR=$PROJDIR/scripts

# PID of the script that generated the data to be processed
PID=$1
FILE=$1-flows.txt
TEMP=$FILE-temp
sed '1d' $ARCHDIR/$FILE | grep -v inf > $DOCDIR/$TEMP
cd $DOCDIR

#echo '"Error Rate" "Call Rate" "Flow ID" Hops Latency "Avg Packet Loss" "Avg Jitter" "Median Jitter"'
#echo '#Hops "Error Rate" Latency'				> latency-vs-error.txt
cat $TEMP | cut -d' ' -f4,1,5 | tr ' ' ','	> latency-vs-error.txt
$SCRIPTDIR/latency-error.py latency-vs-error.txt > lve2.txt

#echo '#"Error Rate" Hops "Avg Packet Loss"'		> loss-vs-hops.txt
cat $TEMP | cut -d' ' -f1,4,6 | tr ' ' ','	> loss-vs-hops.txt
$SCRIPTDIR/loss-hops.py loss-vs-hops.txt > lvh2.txt

#echo '#"Error Rate" Hops "Avg Jitter"'			> jitter-vs-error.txt 
cat $TEMP | cut -d' ' -f1,4,7 | tr ' ' ','	> jitter-vs-error.txt
$SCRIPTDIR/jitter-error.py jitter-vs-error.txt > jve2.txt

#echo '#"Error Rate" Hops "Avg Packet Loss" "Avg Jitter"' > jitter-vs-loss.txt
cat $TEMP | cut -d' ' -f1,4,6,7 | tr ' ' ','> jitter-vs-loss.txt
$SCRIPTDIR/jitter-loss.py jitter-vs-loss.txt > jvl2.txt

#echo '#"Error Rate" Hops Latency "Avg Jitter"'	> jitter-latency-vs-error.txt
cat $TEMP | cut -d' ' -f1,4,5,7 | tr ' ' ','> jitter-vs-loss.txt
$SCRIPTDIR/jitter-latency-error.py jitter-vs-loss.txt > jlve2.txt

gnuplot <<EOF
set term postscript eps color
set output '$DOCDIR/latency-vs-error.eps'
set title 'Latency vs Error Rate'
set ylabel 'Average Flow Setup Latency'
set xlabel 'Link Error Rate'
set xrange [0:0.1]

plot 'lve2.txt' i 0 u 1:2:3 w yerrorbars title columnheader(1), \
	'' i 1 using 1:2:3 w yerrorbars title columnheader(1), \
	'' i 2 using 1:2:3 w yerrorbars title columnheader(1
EOF

gnuplot <<EOF
set term postscript eps color
set output 'loss-vs-hops.eps'
set title 'Packet Loss vs Number of Hops'
set xlabel 'Number of Hops'
set ylabel 'Average Packet Loss'

plot './lvh2.txt' i 0 w lp title columnheader(1),\
	'' i 1 w lp title columnheader(1),\
	'' i 2 w lp title columnheader(1)
EOF

gnuplot <<EOF
set term postscript eps color
set output 'jitter-vs-error.eps'
set title 'Average Jitter vs Error Rate'
set ylabel 'Average Jitter of Flow'
set xlabel 'Link Error Rate'

plot './jve2.txt' i 0 w lp title columnheader(1),\
	'' i 1 w lp title columnheader(1),\
	'' i 2 w lp title columnheader(1)
EOF

gnuplot <<EOF
set term postscript eps color
set output 'jitter-vs-loss.eps'
set title 'Average Jitter vs Packet Loss'
set ylabel 'Average Jitter of Flow'
set xlabel 'Packet Loss'
set yrange [-0.5:0.5]

plot './jvl2.txt' i 0 w p pt 7 ps 0.7 t columnheader(1),\
	'' i 1 w p pt 7 ps 0.7 t columnheader(1), \
	'' i 2 w p pt 7 ps 0.7 t columnheader(1)
EOF

gnuplot <<EOF
set term postscript eps color
set output 'jitter-latency-vs-error.eps'
set title 'Average Jitter vs Error Rate'
set xlabel 'Error Rate'
set ylabel 'Average Jitter'
set y2label 'FLow Setup Latency'
set xrange[0:0.06]
set y2tics

plot 'jlve2.txt' i 0 using 1:2 w lp ls 1 lc 1 t columnheader(1), \
	'' i 0 using  1:3 axes x1y2 w lp ls 5 lc 1 notitle, \
	'' i 1 using 1:2 w lp ls 2 lc 2 t columnheader(1), \
	'' i 1 using  1:3 axes x1y2 w lp ls 6 lc 2  notitle, \
	'' i 2 using 1:2 w lp ls 3 lc 3 t columnheader(1), \
	'' i 2 using  1:3 axes x1y2 w lp ls 7 lc 3 notitle

EOF


