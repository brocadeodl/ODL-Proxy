TOPDIR := ../../../../..

PLATFORM                    := host

include ../source/Makefile.tools

CXXFLAGS                      += -I $(XERCESC_INCLUDE_DIR)
LIBFILES                      := $(if $(filter $(USE_STATIC_LIBS),1),$(XERCESC_LIB_DIR)/libxerces-c.a,-lxerces-c)
LIBFILES                      += -lpthread
COMMA := ,
LDFLAGS                       := $(if $(filter-out $(USE_STATIC_LIBS),1),-L$(XERCESC_LIB_DIR) -Wl$(COMMA)-R$(XERCESC_LIB_DIR))

.PHONY: all
all: PostBoot.$(OS).$(ARCH)

.SECONDEXPANSION:

MKFS := $(MAKEFILE_LIST)
PostBoot.$(OS).$(ARCH): PostBoot.cpp $(MKFS)
	$(ECHOCMD) -n "Building PostBoot... "
	$(CXX) $(CXXFLAGS) $(LDFLAGS) PostBoot.cpp -o PostBoot.$(OS).$(ARCH) $(LIBFILES)
	$(ECHOCMD) "Done"

clean:
	rm -rf PostBoot.$(OS).$(ARCH)

cleanall: clean
	rm -rf PostBoot.*.*;
