#!  /usr/bin/gnuplot
set term postscript eps linewidth 2 size 4,2.8
set output 'errorbars.eps'

set grid
set xrange [1:10]
set yrange [-20:120]

set xlabel 'Supplier number'
set ylabel 'Average weight of potato sack (kg)'

set key top right

plot 'errorbars.txt' u 1:2 w lp lt 1 lc rgb "red" pt 9 title 'Average over 10 sacks',\
	 '' with errorbars lt 3 lw 1 lc rgb "blue" pt 2 notitle
