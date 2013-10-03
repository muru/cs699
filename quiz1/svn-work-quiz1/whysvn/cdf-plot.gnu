#! /usr/bin/gnuplot

set term eps size 4.5,3

set output 'cdf-plot.eps'
set xrange [-10:70]
set yrange [0:100]

set ytics 0,10,100

set xlabel "Number of retransmissions"
set ylabel "CDF (%)"

set key at 68,35

plot 'a1.csv' with lp ps 0.5 pt 5 title 'Algorithm-1',\
	 'a2.csv' with lp ls 0 lw 3 lc rgb "green" ps 0.3 pt 9 title 'Algorithm-2',\
	 40 with dots title 'Average expectation'


