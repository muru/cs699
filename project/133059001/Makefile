CC = gcc
LD = gcc
CFLAGS += -DDEBUG_FRAC
CFLAGS += -DDEBUG_ENABLE -g
LFLAGS += -lm -ll
NORMDEF = -D GENERATE_REQUESTS_FROM_FILE=1
GENDEF = -D GENERATE_REQUESTS_FROM_FILE=0

#PROJDIR=~/cs699/project/133059001

SRCDIR = src
BINDIR = bin
OBJDIR = obj
INCDIR = inc
DATADIRS = logs outputFiles flows mobility store_cap tests archives doc

CFLAGS += -I $(INCDIR)

HFILES = $(addprefix $(INCDIR)/, header.h config.h y.tab.h)

CFILES = coordinateToGraph.c handleDataSlot.c input.c routingPath.c fragment.c handlePackets.c scheduler.c handlePriorityQueue.c nodeStats.c simulation.c getEventStructures.c handleScheduleTxRx.c randomfunc.c handleContentionSlot.c rootfunc.c lex.yy.c y.tab.c
OFILES = $(addprefix $(OBJDIR)/, $(CFILES:%.c=%.o)) 
# coordinateToGraph.o handleDataSlot.o input.o routingPath.o fragment.o handlePackets.o scheduler.o  handlePriorityQueue.o nodeStats.o simulation.o getEventStructures.o handleScheduleTxRx.o randomfunc.o handleContentionSlot.o rootfunc.o)

SCRIPTS = run_tests.sh parse_output.sh calc_jitter.py

PROG = $(BINDIR)/lo3mac
GEN_PROG = $(BINDIR)/gen_lo3mac

# Normal Simulator - GENERATE_REQUESTS_FROM_FILE set to 1
norm: CFLAGS += $(NORMDEF)
norm: $(PROG) | $(DATADIRS)

# GENERATE_REQUESTS_FROM_FILE set to 0 
gen: CFLAGS += $(GENDEF)
gen: $(GEN_PROG) | $(DATADIRS)

.PHONY: test
test:
	./run_tests.sh

all: test

#gen_header: $(INCDIR)/header.h
#	grep -q 'GENERATE_REQUESTS_FROM_FILE *0' $^ || \
#	sed -ibak 's/\(GENERATE_REQUESTS_FROM_FILE\) .*/\1 0/' $^

#norm_header: $(INCDIR)/header.h
#	grep -q 'GENERATE_REQUESTS_FROM_FILE *1' $^ || \
#	sed -ibak 's/\(GENERATE_REQUESTS_FROM_FILE\) .*/\1 1/' $^

$(DATADIRS):
	mkdir -p $(DATADIRS)
	chmod u+wx $(DATADIRS)

$(BINDIR)/%: $(OFILES)
	$(LD) -o $@ $(OFILES) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HFILES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OFILES) $(PROG) $(GEN_PROG)
	rm -f $(SRCDIR)/*.c~ $(INCDIR)/*.h~ *~
	rm -f flows/flow*
	rm -f logs/node*
	rm -f outputFiles/*
	rm -f mobility/node*
	rm -f store_cap/*
	rm -rf tests/*
	rm -f networkGraph.txt 
	rm -f $(SRCDIR)/lex.yy.c $(SRCDIR)/y.tab.c $(INCDIR)/y.tab.h 

$(INCDIR)/y.tab.h $(SRCDIR)/y.tab.c $(SRCDIR)/lex.yy.c : config.l config.y $(INCDIR)/config.h
	yacc --defines=$(INCDIR)/y.tab.h -o $(SRCDIR)/y.tab.c config.y
	lex -o $(SRCDIR)/lex.yy.c config.l

.DEFAULT: ;
