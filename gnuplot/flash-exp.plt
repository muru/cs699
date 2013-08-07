#! /usr/bin/gnuplot

set term postscript eps enhanced size 4,2.8 lw 3
set output 'flash-exp.eps'

set xlabel 'Number of Bytes'
set ylabel 'Latency (ticks)'

set grid

set xtics 0,100,500
set ytics 0,200,1200

set key  at 480,780

set yrange [0:1200]

plot 'flash-exp.txt' using 1:2 with lp lc rgb 'red' pt 13 title 'read',\
	 'flash-exp.txt' using 1:3 with lp ls 3 lc rgb 'blue' pt 7 title 'write',\
	 'flash-exp.txt' using 1:4 with lp ls 2 lc rgb 'green' pt 5 title 'sync';
#!  /usr/bin/gnuplot
