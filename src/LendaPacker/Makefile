TARGET     = ../../lib/libLendaPacker.so
OBJS       = LendaPacker.o LendaPackerDictionary.o
TEMP=LendaFilter R00TLeSettings LendaEvent DDASEvent
LIBDIR   = $(PWD)/../../lib/
SPECIALLIBS=$(addprefix -l,$(TEMP)) -L$(LIBDIR)
include ../common.mk
