TARGET     = $(R00TLeInstall)/lib/libLendaPacker.so
OBJS       = LendaPacker.o LendaPackerDictionary.o
TEMP=LendaFilter R00TLeSettings LendaEvent DDASEvent
LIBDIR   = $(R00TLeInstall)/lib/
SPECIALLIBS=$(addprefix -l,$(TEMP)) -L$(LIBDIR)
include $(R00TLeInstall)/src/common.mk
