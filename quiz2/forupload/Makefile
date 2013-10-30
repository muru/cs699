GP=gnuplot
SH=bash

GS=plot.gnu
SS=gen-data.sh
PYS=trace-analysis.py

DATS=anonP1.dat  anonP2.dat  anonP3.dat
CSVS=anonP1.csv  anonP2.csv  anonP3.csv

EPS=plot.eps

.PHONY: all

all: $(EPS)

$(EPS): $(DATS) $(GS)
	$(GP) $(GS)

$(DATS): $(CSVS) $(SS) $(PYS)
	$(SH) $(SS)

.PHONY: clean

clean:
	rm $(DATS) $(EPS) -f

