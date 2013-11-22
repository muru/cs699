LC=latex
XC=xelatex
BC=bibtex
GP=gnuplot

TEX := doc.tex
AUX := $(TEX:%.tex=%.aux)
EPS := latency-vs-error.eps loss-vs-hop.eps jitter-vs-error.eps scatter-jitter-vs-loss.eps jitter-latency-vs-error.eps

MAIN := doc
PDF := $(MAIN).pdf

all: $(PDF)

$(PDF): $(AUX)
	$(XC) $(MAIN) 

$(EPS): $(PLT) $(DATA)
	$(GP) $(PLT) 

%.aux: %.tex
	$(XC) $(MAIN) 

.PHONY: clean

clean:
	rm -f $(PDF) $(EPS) *.acn *.acr *.alg *.aux *.bbl *.blg *.brf *.dvi *.glg *.glo *.gls *.ist *.lof *.log *.out *.toc 
