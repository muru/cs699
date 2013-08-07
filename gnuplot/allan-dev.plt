#! /usr/bin/gnuplot
#set term eps dashed dashlength 0.5
set term postscript eps enhanced
set output 'allan-dev.eps'

set xlabel 'Averaging Interval (sec)'
set ylabel 'Allan Deviation'

set logscale

set yrange [0.001:1]
set xrange [0.01:60]

set xtics (0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1, 2, 5, 10, 60)
set grid

set style line 1 lc 0 pt 9 lw 4
set style line 2 lc 0 pt 5 lw 4

plot './allan-dev.txt' using 1:2 with linespoints ls 1 title 'Measured',\
	 './allan-dev.txt' using 1:4 with linespoints ls 2 title 'Uniform';

#!  /usr/bin/gnuplot
