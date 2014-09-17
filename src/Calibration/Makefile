TARGET   = ../../bin/Calibrate
SUBLIBS  = S800 Utilities DDASEvent LendaEvent R00TLeSettings LendaPacker LendaFilter Scalers
LIBS     = $(addprefix -l, $(SUBLIBS))
LIBDIR   = $(PWD)/../../lib/


include ../common.mk

../../bin/Calibrate: Calibrate.cc \
			../include/DAQdefs.h \
			../include/S800Event.hh ../include/S800.hh ../include/S800defs.h \
			../include/DDASEvent.hh ../include/ddaschannel.hh \
			../include/Utilities.hh
