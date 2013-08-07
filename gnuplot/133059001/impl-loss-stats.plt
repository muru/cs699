#!  /usr/bin/gnuplot
set term postscript enhanced eps size 3,2
set output 'impl-loss-stats.eps'

set grid
set ylabel 'Packet Drop (%)'
set xlabel 'Node Id'

set auto x
set yrange [0:3.5]

set style data histograms
set style histogram cluster gap 5
set xtics ("1" 0, "2" 2)


plot 'impl-loss-stats.txt' using (($2) + ($3))/10:xtic(1) with histograms lc 0 fill pattern 3 title 'Total', \
	 '' using ($2)/10 with histograms lc 0 fill pattern 1 title 'Overflow', \
	 '' using ($3)/10 with histograms lc 0 fill pattern 2 title 'Retry-Limit';


