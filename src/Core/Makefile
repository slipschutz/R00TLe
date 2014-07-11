TARGET   = ../../bin/Evt2Raw ../../bin/Raw2Cal ../../bin/Evt2Cal
SUBLIBS  = S800 Utilities DDASEvent LendaEvent R00TLeSettings LendaPacker LendaFilter
LIBS     = $(addprefix -l, $(SUBLIBS))
LIBDIR   = $(PWD)/../../lib/
#LIBDIR   = /user/e10003/anaroot/lib/

include ../common.mk

../../bin/Evt2Raw:	Evt2Raw.cc \
			../include/DAQdefs.h \
			../include/S800Event.hh ../include/S800.hh ../include/S800defs.h \
			../include/DDASEvent.hh ../include/ddaschannel.hh \
			../include/Utilities.hh

../../bin/Raw2Cal:	Raw2Cal.cc \
			../include/S800Event.hh ../include/S800.hh ../include/S800defs.h \
			../include/S800Calc.hh  ../include/S800Settings.hh ../include/S800Calibration.hh \
			../include/DDASEvent.hh ../include/ddaschannel.hh \
			../include/LendaPacker.hh ../include/LendaFilter.hh ../include/LendaEvent.hh \
			../include/R00TLeSettings.hh \
			../include/Utilities.hh

../../bin/Evt2Cal:	Evt2Cal.cc \
			../include/DAQdefs.h \
			../include/S800Event.hh ../include/S800.hh ../include/S800defs.h \
			../include/S800Calc.hh  ../include/S800Settings.hh ../include/S800Calibration.hh \
			../include/DDASEvent.hh ../include/ddaschannel.hh \
			../include/LendaPacker.hh ../include/LendaFilter.hh ../include/LendaEvent.hh \
			../include/R00TLeSettings.hh  \
			../include/Utilities.hh
