ROOTLIB = $(shell root-config --libs --glibs)
INCDIR = $(shell root-config --cflags)
INCDIR += -I$(shell echo $(FAIRROOTPATH)/include)
INCDIR += -I$(shell echo $(VMCWORKDIR)/detectors/emc/EmcData)
INCDIR += -I$(shell echo $(VMCWORKDIR)/detectors/emc/EmcTools)
INCDIR += -I$(shell echo $(SIMPATH)/include)
INCDIR += -I./src
LIBDIR = -L$(shell echo ${VMCWORKDIR}/panda/lib) -L$(shell echo $(SIMPATH)/lib) -L./lib
#vpath %.hh ./src
CC = g++ $(ROOTLIB) $(INCDIR) $(LIBDIR)

OBJS = tmp/ECalStandalone.o tmp/EmcCalibHelper.o tmp/EmcCalibMinuit2.o tmp/EmcCalibMinuit.o  tmp/EmcCalibrationLh.o tmp/EmcCalibFactory.o tmp/EmcCalibIO.o tmp/EmcCalibMinuit2Fcn.o tmp/EmcCalibration.o tmp/EmcCalibWrapper.o

all: prepareSample ECalStandalone

%:%.C lib/libEmcCalibDataModel.so
	$(CC) $^ -lEmc -lboost_serialization -o $@

lib/libEmcCalibDataModel.so: src/EmcCalibDataModelDict.cxx
	mkdir -p lib
	g++ ${INCDIR} -o $@ -c $^
	cp src/EmcCalibDataModelDict_rdict.pcm lib/

ECalStandalone:${OBJS}
	$(CC) $^ -lEmcCalibDataModel -lMinuit -lMinuit2 -lboost_serialization -o $@

tmp/%.o:src/%.cc
	g++ ${INCDIR} -o $@ -c $^
