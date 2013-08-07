#!  /usr/bin/gnuplot

set term postscript eps enhanced size 4,2.8

set output 'pip-latency-comparison.eps'

set yrange [0:450]
set xrange [1:6]

set grid

set key at 6, 300

set xlabel 'Number of Hops'
set ylabel 'Latency (seconds)'

plot 'pip-latency-comparison.txt' using 1:2 with lp ls 1 lw 3 lc rgb "red" pt 2 title 'PIP',\
	 '' using 1:3 with lp ls 3 lw 3 lc rgb "blue" pt 1 title 'Flush', \
	 '' using 1:4 with lp ls 7 lw 3 lc rgb "black" pt 7 title 'PSFQ', \
	 '' using 1:5 with lp ls 4 lw 3 lc rgb "#FF00FA" pt 9 title 'Fetch'; 

