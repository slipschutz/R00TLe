CXX        = g++
CC         = gcc
ROOTCINT   = rootcint

ROOTCFLAGS =   $(shell root-config --cflags)
ROOTLIBS   =   $(shell root-config --libs)  # does not include -lGui
ROOTGLIBS  =   $(shell root-config --glibs) # does     include -lGui
ROOTINC    = -I$(shell root-config --incdir)

WARN_OPT   = -Wall -Wno-long-long #-pedantic 
OPTIM_OPT  = -O3
DEBUG_OPT  = -g
POS_INDEP  = -fPIC
LFLAGS     = $(DEBUG_OPT) $(POS_INDEP)
CFLAGS     = $(LFLAGS) $(WARN_OPT) $(OPTIM_OPT)
CCFLAGS    = -std=c++0x $(ROOTCFLAGS)
SOFLAGS    = -shared

DICTFILES  = $(wildcard *Dictionary.*)
OBJFILES   = $(wildcard *.o)
GCHFILES   = $(wildcard *.gch)

INCDIR     = -I../include -I/usr/opt/nscldaq/10.2-105/include/
LDFLAGS    = -Wl,-rpath=$(LIBDIR) -Wl,-rpath=/usr/opt.nscldaq/10.2-105/lib/

.SECONDARY: $(OBJS)

.PHONY:    all clean
all:       $(TARGET)
clean:
	-@rm -fv $(TARGET) $(SHLIBS) $(OBJFILES) $(DICTFILES) $(GCHFILES) 2>/dev/null || true

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cc %.hh
	@echo "Compiling $<..."
	@$(CXX) $(CFLAGS) $(CCFLAGS) $(INCDIR) -c $< -o $@

../../lib/lib%.so: $(OBJS)
	@echo "Building Library $<..."
	@$(CXX) $(LFLAGS) $(ROOTLIBS) $(SOFLAGS) -o $@ $^

../../bin/%: %.cc
	@echo "Building target $<..."
	@$(CXX) $(CFLAGS) $(CCFLAGS) $(ROOTLIBS) -L$(LIBDIR) $(LIBS) $(INCDIR) $(LDFLAGS) $< -o $@

%Dictionary.o: %.hh %LinkDef.h
	@echo "Compiling ROOT dictionary $(patsubst %.o,%.cc,$@)..."
	@$(ROOTCINT) -f $(patsubst %.o,%.cc,$@) -c -p $(INCDIR) $^;
	@$(CXX) $(CFLAGS) $(CCFLAGS) $(INCDIR) -c $(patsubst %.o,%.cc,$@) -o $@

%Dictionary3.o: %.hh $(SPECIALHEADERS) %LinkDef.h
	@echo "Compiling ROOT dictionary $(patsubst %.o,%.cc,$@)..."
	@$(ROOTCINT) -f $(patsubst %.o,%.cc,$@) -c -p $(INCDIR) $^;
	@$(CXX) $(CFLAGS) $(CCFLAGS) $(INCDIR) -c $(patsubst %.o,%.cc,$@)

