TOPDIR := ../../../../..

include ../source/Makefile.versioninfo
include ../source/Makefile.tools

# the -I and -iquote options are specified only here
CXXFLAGS += \
	-I $(SRC_ROOT) \
	-iquote $(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP) \
	-iquote $(POSTGRESQL_INCLUDE_DIR) \
	-I $(OPENVSWITCH_INCLUDE_DIR) \
	-I $(WAVEOSS_INCLUDE_DIR) \
	-I $(XERCESC_INCLUDE_DIR) \
	-I $(XALANC_INCLUDE_DIR)

ifeq "fabos" "$(PLATFORM)"
CXXFLAGS += \
        -I $(FABOS_ROOT)/../target/include
endif


LIB.SO.VER := $(LIB.SO).$(VERSION_FULL)
WAVE_LIB := $(LIB.SO)

################################################################################
# common dependencies for all programs
#
# extractions are libraries not built by us but extracted - they are listed in commonMakefileEnd
#
# notice we need both the .so library file, the links, and the file linked to
# if both are there, then in fact make checks the same timestamp for both, so .so is redundant
# but if only .so is present, then we don't care about it's timestamp, only existence
# thus  .so are order-only prerequisites
################################################################################
COMMON_DEPENDENCIES := $(WAVE_LIB) $(MKFS)

PROGRAMS := \
	Spectrum.$(OS_ARCH_XT) \
	WaveCli.$(OS_ARCH_XT) \
	RedundancyOptimizer.$(OS_ARCH_XT) \
	WaveConfigurationWriter.$(OS_ARCH_XT) \
	Wyser.$(OS_ARCH_XT) \
	WavesyStemManagement.$(OS_ARCH_XT) \
	WyserCli.$(OS_ARCH_XT) \
	WaveMessageExchange.$(OS_ARCH_XT) \
	WaveMessageExchangeClient.$(OS_ARCH_XT)


.PHONY: all wavesobundle waveso clean cleanall phony Wyser
all: $(PROGRAMS)

Wyser: Wyser.$(OS_ARCH_XT)

################################################################################
# list here dependencies for programs
# other than the common dependencies listed above
#
# the rule for programs in commonMakefileEnd
#
# use relative paths here only
################################################################################
Spectrum.$(OS_ARCH_XT): Spectrum.cpp
WaveCli.$(OS_ARCH_XT): WaveCli.cpp WaveCliReplyThread.cpp
RedundancyOptimizer.$(OS_ARCH_XT): RedundancyOptimizer.cpp
WaveConfigurationWriter.$(OS_ARCH_XT): WaveConfigurationWriter.cpp
Wyser.$(OS_ARCH_XT): Wyser.cpp
WavesyStemManagement.$(OS_ARCH_XT): Wsmd.cpp
WyserCli.$(OS_ARCH_XT): WyserCli.cpp
WaveMessageExchange.$(OS_ARCH_XT) : WaveMessageExchange.cpp
WaveMessageExchangeClient.$(OS_ARCH_XT) : WaveMessageExchangeClient.cpp


include ../source/commonMakefileEnd

################################################################################
# the targets below are still in the "script style", not converted yet to makefile style
# Mark Galeck 3-12-2012
################################################################################

wavesobundle: Spectrum.$(OS_ARCH_XT) $(LIB.SO.MAJOR)
	rm -f wavesobundle.$(OS).$(ARCH).tar.gz;
	cp  $(LIB.SO.MAJOR) .;
	#cp $(XERCESC_LIB_DIR)/libxerces-c.so.$(XERCESC_VERSION_TAG) .;
	#cp $(XERCESC_LIB_DIR)/libxerces-depdom.so.$(XERCESC_VERSION_TAG) .;
	#tar -cvf wavesobundle.$(OS).$(ARCH).tar lib$(WAVE).so.$(WAVE_VERSION_MAJOR) libxerces-c.so.$(XERCESC_VERSION_TAG) libxerces-depdom.so.$(XERCESC_VERSION_TAG) Spectrum.$(OS).$(ARCH); gzip wavesobundle.$(OS).$(ARCH).tar;
	tar -cvf wavesobundle.$(OS).$(ARCH).tar lib$(WAVE).so.$(WAVE_VERSION_MAJOR) Spectrum.$(OS).$(ARCH); gzip wavesobundle.$(OS).$(ARCH).tar;
	#chmod 755 lib$(WAVE).so.$(WAVE_VERSION_MAJOR) libxerces-c.so.$(XERCESC_VERSION_TAG) libxerces-depdom.so.$(XERCESC_VERSION_TAG); rm -f lib$(WAVE).so.$(VERSION_MAJOR) libxerces-c.so.$(XERCESC_VERSION_TAG) libxerces-depdom.so.$(XERCESC_VERSION_TAG);
	chmod 755 lib$(WAVE).so.$(WAVE_VERSION_MAJOR); rm -f lib$(WAVE).so.$(WAVE_VERSION_MAJOR);

waveso: $(LIB.SO.MAJOR)
	rm -f waveso.$(OS).$(ARCH).tar.gz
	cp  $(LIB.SO.MAJOR) .
	tar -cvf waveso.$(OS).$(ARCH).tar lib$(WAVE).so.$(WAVE_VERSION_MAJOR); gzip waveso.$(OS).$(ARCH).tar;
	chmod 755 lib$(WAVE).so.$(WAVE_VERSION_MAJOR); rm -f lib$(WAVE).so.$(WAVE_VERSION_MAJOR);

clean: wave_clean
	rm -rf Spectrum.$(OS).$(ARCH)
	rm -rf WaveCli.$(OS).$(ARCH)
	rm -rf RedundancyOptimizer.$(OS).$(ARCH)
	rm -rf waveso.$(OS).$(ARCH).tar.gz;
	rm -rf wavesobundle.$(OS).$(ARCH).tar.gz;
	rm -rf WaveConfigurationWriter.$(OS).$(ARCH);
	rm -rf Wyser.$(OS).$(ARCH);

wave_clean:
	$(MAKE) -C $(WAVE_SRC) clean

cleanall:
	rm -f Spectrum.*.*;
	rm -f WaveCli.*.*;
	rm -f RedundancyOptimizer.*.*;
	rm -f waveso.*.*.tar.gz;
	rm -f wavesobundle.*.*.tar.gz;
	rm -f WaveConfigurationWriter.*.*;
	rm -f Wyser.*.*;
