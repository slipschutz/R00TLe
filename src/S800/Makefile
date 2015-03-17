TARGET     = $(R00TLeInstall)/lib/libS800.so
OBJS       = S800.o            S800Dictionary.o \
             S800Event.o       S800EventDictionary.o \
             S800Calc.o        S800CalcDictionary.o \
             S800Settings.o    S800SettingsDictionary.o \
             S800Calibration.o S800CalibrationDictionary.o lmmin.o lmcurve.o lmfit.o 


include $(R00TLeInstall)/src/common.mk

S800.o:	S800.cc S800.hh S800defs.h
S800Event.o: S800Event.cc S800Event.hh S800.hh S800defs.h
S800Calc.o: S800Calc.cc S800Calibration.hh S800Settings.hh S800Calc.hh
S800Calibration.o: S800Calibration.cc S800Calibration.hh S800Settings.hh S800Calc.hh S800.hh \
                   S800defs.h lmcurve.h lmmin.h lmfit.h

# for lmfit-3.3
lmfit.o: lmmin.h lmcurve.h
lmmin.o: lmmin.h
lmcurve.o: lmcurve.h
