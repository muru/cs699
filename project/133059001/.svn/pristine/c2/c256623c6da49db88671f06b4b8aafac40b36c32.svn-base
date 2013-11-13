CC = gcc
LD = gcc
CFLAGS += -DDEBUG_FRAC
CFLAGS += -DDEBUG_ENABLE -g
LFLAGS += -lm -ll

SRCDIR = src
BINDIR = bin
OBJDIR = obj
INCDIR = inc

CFLAGS += -I $(INCDIR)

HFILES = $(addprefix $(INCDIR)/, header.h config.h y.tab.h)

CFILES = coordinateToGraph.c handleDataSlot.c input.c routingPath.c fragment.c handlePackets.c scheduler.c handlePriorityQueue.c nodeStats.c simulation.c getEventStructures.c handleScheduleTxRx.c randomfunc.c handleContentionSlot.c rootfunc.c lex.yy.c y.tab.c
OFILES = $(addprefix $(OBJDIR)/, $(CFILES:%.c=%.o)) 
# coordinateToGraph.o handleDataSlot.o input.o routingPath.o fragment.o handlePackets.o scheduler.o  handlePriorityQueue.o nodeStats.o simulation.o getEventStructures.o handleScheduleTxRx.o randomfunc.o handleContentionSlot.o rootfunc.o)

PROG = $(BINDIR)/lo3mac

all: $(PROG)

$(PROG): $(OFILES)
	$(LD) -o $(PROG) $(OFILES) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HFILES)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OFILES) $(PROG)
	rm -f *.c~ *.h~ *~
	rm -f flows/flow*
	rm -f logs/node*
	rm -f outputFiles/*
	rm -f lo3mac
	rm -f mobility/node*
	rm -f store_cap/*
	rm -f lex.yy.c y.tab.c y.tab.h 

$(INCDIR)/y.tab.h $(SRCDIR)/y.tab.c $(SRCDIR)/lex.yy.c : config.l config.y $(INCDIR)/config.h
	yacc --defines=$(INCDIR)/y.tab.h -o $(SRCDIR)/y.tab.c config.y
	lex -o $(SRCDIR)/lex.yy.c config.l
