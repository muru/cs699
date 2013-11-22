LC=latex
XC=xelatex
BC=bibtex
GP=gnuplot

TEX := doc.tex los.tex stable_filtes.tex Why.tex 
AUX := $(TEX:%.tex=%.aux)
IMG := latency-vs-error.eps loss-vs-hop.eps jitter-vs-error.eps scatter-jitter-vs-loss.eps jitter-latency-vs-error.eps

GRAPH := qsize-util-err
EPS := $(GRAPH).eps
PLT := $(GRAPH).plt
DATA := $(addprefix $(GRAPH)/, anal-loss01-node1-stats.txt anal-loss05-node1-stats.txt anal-loss10-node1-stats.txt anal-loss20-node1-stats.txt)

MAIN := Why
GLO := $(MAIN).glo
BBL := $(MAIN).bbl
PDF := $(MAIN).pdf

all: $(PDF)

$(PDF): $(EPS) $(AUX) $(BBL) $(GLO) $(IMG)
	$(XC) $(MAIN) 

$(EPS): $(PLT) $(DATA)
	$(GP) $(PLT) 

$(BBL): $(TEX) $(REF)
	$(BC) $(MAIN) 

$(GLO): $(TEX) $(GLS)
	$(GL) $(MAIN) 

%.aux: %.tex
	$(XC) $(MAIN) 

.PHONY: clean

clean:
	rm -f $(PDF) $(EPS) *.acn *.acr *.alg *.aux *.bbl *.blg *.brf *.dvi *.glg *.glo *.gls *.ist *.lof *.log *.out *.toc 
