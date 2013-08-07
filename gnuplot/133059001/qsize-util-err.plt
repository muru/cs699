#! /usr/bin/gnuplot

set term postscript eps dashed size 4,2.8
set output 'qsize-util-err.eps'

set xrange [0:20]
set yrange [0:20]
set y2range [60:100]

set y2tics 60,5,100
set mxtics 2
set mytics 2
set grid xtics mxtics ytics mytics

set ylabel 'Queue Overflow (%)'
set y2label 'Utilization (%)'
set xlabel 'Queue Size'

set key at 19,9  
set style line 1 lw 1.5
set style line 3 lw 1.5

plot 'qsize-util-err/anal-loss01-node1-stats.txt' using 1:($3*100) with lp ls 3 lw 3 lc 3 pt 5 title 'QO: Err 01%',\
	 'qsize-util-err/anal-loss05-node1-stats.txt' using 1:($3*100) with lp ls 3 lw 3 lc 3 pt 9 title 'QO: Err 05%',\
	 'qsize-util-err/anal-loss10-node1-stats.txt' using 1:($3*100) with lp ls 3 lw 3 lc 3 pt 7 title 'QO: Err 10%',\
	 'qsize-util-err/anal-loss20-node1-stats.txt' using 1:($3*100) with lp ls 3 lw 3 lc 3 pt 3 title 'QO: Err 20%',\
	 'qsize-util-err/anal-loss01-node1-stats.txt' using 1:($2*100) with lp ls 1 lw 3 lc 1 pt 4 title 'Util: Err 01%' axes x1y2,\
	 'qsize-util-err/anal-loss05-node1-stats.txt' using 1:($2*100) with lp ls 1 lw 3 lc 1 pt 8 title 'Util: Err 05%' axes x1y2,\
	 'qsize-util-err/anal-loss10-node1-stats.txt' using 1:($2*100) with lp ls 1 lw 3 lc 1 pt 6 title 'Util: Err 10%' axes x1y2,\
	 'qsize-util-err/anal-loss20-node1-stats.txt' using 1:($2*100) with lp ls 1 lw 3 lc 1 pt 2 title 'Util: Err 20%' axes x1y2;
