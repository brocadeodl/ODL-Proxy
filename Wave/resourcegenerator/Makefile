TOPDIR := ../../../../..

PLATFORM := host

include ../source/Makefile.tools

.PHONY: all
all: WaveResourceGenerator.$(OS).$(ARCH)

CXXFLAGS                      += -I $(XERCESC_INCLUDE_DIR)
LIBFILES	:= $(if \
	$(filter $(USE_STATIC_LIBS),1),$(XERCESC_LIB_DIR)/libxerces-c.a,-lxerces-c)
LIBFILES                      += -lpthread
COMMA := ,
LDFLAGS	:= $(if $(filter-out $(USE_STATIC_LIBS),1),\
	-L$(XERCESC_LIB_DIR) -Wl$(COMMA)-R$(XERCESC_LIB_DIR))

.SECONDEXPANSION:

MKFS := $(MAKEFILE_LIST)
WaveResourceGenerator.$(OS).$(ARCH): WaveResourceGenerator.cpp $(MKFS)
	$(ECHOCMD) "Building Wave Resource Generator Application ... "
	$(CXX) $(CXXFLAGS) $(LDFLAGS) WaveResourceGenerator.cpp -o $@ $(LIBFILES)
	$(ECHOCMD) "Done"

clean:
	rm -rf WaveResourceGenerator.$(OS).$(ARCH)

cleanall: clean
	rm -rf WaveResourceGenerator.*.*;

include $(SRC_ROOT)/commonMakefileEnd
