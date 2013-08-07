#!  /usr/bin/gnuplot
set term postscript eps enhanced color size 4,1.4
set output 'roofnet-noise11.eps'

set xlabel 'Transmitter-Receiver pair num.'
set ylabel 'Silence level (dBm)'

set xrange [0:25]
set yrange [-100:-70]

set key top center

plot 'roofnet-noise11.txt' using 1 w lp lw 2.5 title '5%-ile',\
	 '' u 4 w lp lt 3 pt 9 lw 2.5 lc rgb "blue" t 'Median (50%-ile)',\
	 '' u 2 w lp lt 7 pt 2 lw 3 lc rgb "black" t '95%-ile'
