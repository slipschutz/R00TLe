TARGET   = $(R00TLeInstall)/bin/Calibrate
SUBLIBS  = S800 Utilities DDASEvent LendaEvent R00TLeSettings LendaPacker LendaFilter Scalers
LIBS     = $(addprefix -l, $(SUBLIBS))
LIBDIR   = $(R00TLeInstall)/lib/


include $(R00TLeInstall)/src/common.mk

$(TARET): Calibrate.cc \
			$(R00TLeInstall)/src/include/DAQdefs.h \
			$(R00TLeInstall)/src/include/S800Event.hh $(R00TLeInstall)/src/include/S800.hh \
			 $(R00TLeInstall)/src/include/S800defs.h \
			$(R00TLeInstall)/src/include/DDASEvent.hh $(R00TLeInstall)/src/include/ddaschannel.hh \
			$(R00TLeInstall)/src/include/Utilities.hh
