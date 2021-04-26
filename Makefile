CC=g++
#wildcard, this finds all *.c file and make *.o files
objects = $(patsubst %.c,%.o,$(wildcard *.c))

DELPHES_DIR := /home/c_neeraj/HEP/delphes/Delphes-3.4.2

PLATFORM_TYPE := $(shell uname -s)
ROOTFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs)
ROOTINC := $(shell root-config --incdir)

#include delphes exroot
INC := -I$(DELPHES_DIR) -I$(DELPHES_DIR)/classes -I$(DELPHES_DIR)/external/
LIB := -L$(DELPHES_DIR) -lDelphes -Wl,-rpath $(DELPHES_DIR)

all: trkCount_mu trkmu_ptEta

trkCount_mu: $(objects) trackcount_mu.C
	$(CXX) -O3 $(objects)  trackcount_mu.C $(ROOTFLAGS) $(INC) $(ROOTLIBS) $(LIB) -Wl,-rpath,$(DELPHES_DIR) -o trkCount_mu $(ROOTLIBS)

trkmu_ptEta: $(objects) trkmu_ptEta.C
	$(CXX) -O3 $(objects) trkmu_ptEta.C $(ROOTFLAGS) $(INC) $(ROOTLIBS) $(LIB) -Wl,-rpath,$(DELPHES_DIR) -o trkmu_ptEta $(ROOTLIBS)

clean:
	rm $(FILES) trkCount_mu trkmu_ptEta
