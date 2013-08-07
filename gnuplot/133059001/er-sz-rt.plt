#!  /usr/bin/gnuplot

set term postscript eps size 3.75,2.62
set output 'er-sz-rt.eps'

set ylabel 'Error Rate'
set grid
set logscale y
set yrange [0.001:1]

set xtics ( "1" 0, "2" 1, "5.5" 2, "11" 3, \
			"1" 5, "2" 6, "5.5" 7, "11" 8, \
			"1" 10, "2" 11, "5.5" 12, "11" 13, \
			"1" 15, "2" 16, "5.5" 17, "11" 18) offset 0,0.4

set key top left noautotitles
set style data histogram
set style histogram cluster gap 1
set style fill solid 0.3

plot newhistogram "SNR = 13db",\
	'er-sz-rt.txt' every ::::3 using 2, \
	'' every ::::3 using 4 with histograms fill pattern 1, \
	'' every ::::3 using 6 with histograms fill solid 1.0, \
	newhistogram "SNR = 8db" lt 1,\
	'' every ::4::7 using 2,\
	'' every ::4::7 using 4 with histograms fill pattern 1,\
	'' every ::4::7 using 6 with histograms fill solid 1.0, \
	newhistogram "SNR = 3db" lt 1,\
	'' every ::8::11 using 2 title '100 bytes',\
	'' every ::8::11 using 4 with histograms fill pattern 1 title '500 bytes',\
	'' every ::8::11 using 6 with histograms fill solid 1.0 title '1400 bytes', \
	newhistogram "SNR = 2db" lt 1,\
	'' every ::12 using 2, \
	'' every ::12 using 4 with histograms fill pattern 1, \
	'' every ::12 using 6 with histograms fill solid 1.0
