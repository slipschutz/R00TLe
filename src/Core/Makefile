TARGET   = $(R00TLeInstall)/bin/Evt2Raw $(R00TLeInstall)/bin/Raw2Cal $(R00TLeInstall)/bin/Evt2Cal $(R00TLeInstall)/bin/Cal2Cal  $(R00TLeInstall)/bin/Evt2Scalars
SUBLIBS  = S800 Utilities DDASEvent LendaEvent R00TLeSettings LendaPacker LendaFilter Scalers
LIBS     = $(addprefix -l, $(SUBLIBS))
LIBDIR   = $(R00TLeInstall)/lib/


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

../../bin/Cal2Cal:	Cal2Cal.cc \
			../include/S800Event.hh ../include/S800.hh ../include/S800defs.h \
			../include/S800Calc.hh  ../include/S800Settings.hh ../include/S800Calibration.hh \
			../include/DDASEvent.hh ../include/ddaschannel.hh \
			../include/LendaPacker.hh ../include/LendaFilter.hh ../include/LendaEvent.hh \
			../include/R00TLeSettings.hh \
			../include/Utilities.hh


# ../../bin/Evt2CalStatic:	Evt2Cal.cc \
# 			 ../S800/S800Event.o ../S800/S800.o ../S800/S800Calc.o ../S800/S800Settings.o \
# 			../S800/S800Calibration.o ../DDASEvent/DDASEvent.o ../DDASEvent/ddaschannel.o \
# 			../S800/lmfit.o ../S800/lmmin.o ../S800/lmcurve.o ../S800/S800CalcDictionary.o \
# 			../S800/S800SettingsDictionary.o ../S800/S800EventDictionary.o ../S800/S800Dictionary.o \
# 			../DDASEvent/DDASEventDictionary.o ../DDASEvent/ddaschannelDictionary.o \
# 			../LendaPacker/LendaPacker.o ../LendaEvent/LendaEvent.o ../LendaEvent/LendaBar.o \
# 			../LendaEvent/LendaChannel.o ../LendaEvent/LendaEventDictionary.o \
# 			../LendaEvent/LendaBarDictionary.o ../LendaEvent/LendaChannelDictionary.o \
# 			../LendaFilter/LendaFilter.o ../R00TLeSettings/R00TLeSettings.o \
# 			../R00TLeSettings/R00TLeSettingsDictionary.o ../Utilities/Utilities.o
# 	$(CXX) $(CFLAGS) $(CCFLAGS) $(ROOTLIBS) $^ -o $@ $(INCDIR)


../../bin/Evt2Scalars: Evt2Scalars.cc