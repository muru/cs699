LC=latex
XC=xelatex
BC=bibtex
GP=gnuplot
GL=makeglossaries

AUX := literature.aux los.aux stable_filtes.aux Why.aux 
TEX := literature.tex los.tex stable_filtes.tex Why.tex 
IMG := 3d2.jpg 3d.jpg allbound.jpg bound.jpg compare2d.jpg ellipse.jpg seed.jpg s_in_uns.jpg theta_s_in_uns.jpg 
REF := references.bib
GLS := glossary.tex

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
