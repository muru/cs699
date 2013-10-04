IDIR=include
SDIR=src
BDIR=bin
ODIR=obj

CXX=g++
CXXFLAGS= -std=c++11 -O3 -g
INCLUDES= -I $(IDIR)

OBJS:=$(addprefix $(ODIR)/, main.o hmm.o helper.o)
DEPS:=$(IDIR)/hmm.h
BIN:=$(BDIR)/hmm

all : $(BIN)

$(BIN) : $(OBJS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(ODIR)/%.o : $(SDIR)/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $< 

$(OBJS) : | $(ODIR)

$(ODIR) :
	mkdir $(ODIR)

.PHONY: clean
	
clean: 
	rm -f $(OBJS) $(BIN)
