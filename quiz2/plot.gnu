set term postscript eps color
set output 'plot.eps'
set title 'Packet rateMbps vs NumBytes'
set xlabel 'Packet rateMbps'
set ylabel 'NumBytes'
plot \
'anonP1.dat' using 1:2 with linespoints lt 1 lw 3 pt 2,\
'anonP2.dat' using 1:2 with linespoints lt 3 lw 3 pt 1,\
'anonP3.dat' using 1:2 with linespoints lt 7 lw 3 pt 7
