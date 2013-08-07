#!  /usr/bin/gnuplot

set term postscript enhanced eps color size 3.75,2.62

set output 'pip-channel-hop.eps'

set mytics
set key at 4,60
set yrange [0:65]
set auto x
set xlabel 'WiFi inter-packet gap (milli-seconds)'
set ylabel 'Throughput (Kbps)'

set grid xtics ytics mytics
set style data histograms 
set style histogram cluster gap 5
set style fill solid 1.0 border -1

plot 'pip-channel-hop.txt' using 2:xticlabel(1) with histograms fill lt 1 lw 2 lc rgb "red" title columnhead, \
	 '' using 3 with histograms fill lt 1 lw 2 lc rgb "green" title columnhead, \
	 '' using 4 with lp lt 4 pt 3 ps 1 lw 1.5 lc rgb "blue" title columnhead
