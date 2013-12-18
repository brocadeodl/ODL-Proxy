################################################################################
# the main makefile for the dcm library
# builds all sources (pre-existing and generated)
#
# (replaces the old Makefile, Makefile.leaf, Makefile.library, and all Makefile.build 's)
################################################################################

TOPDIR := ../../../../..

.PHONY: enforcevariables all phony resources postboot yang dcm wavehost

all: dcm 

enforcevariables:
	echo xxxxxxxxxxx
	if [ -z $(WAVE_HOME) ]; then \
	    echo "WAVE_HOME variable must be set to the location where Wave source directory can be found."; \
	    exit 1; \
    fi

include $(WAVE_HOME)/source/Makefile.tools
include Makefile.versioninfo

################################################################################
# all the -I and -iquote options are here

# the performance of -I type flags is sensitive to order and 
# redundancy, so they should be custom fitted to a particular library
# that's why there are here only and not in the common makefiles

# we order the include directories putting the more used closer to top 
# (making sure the included files sequences are as previously)
# this is for speed
# so don't disturb the order unless you have to enforce an inclusion,
# or there comes a difference in usage pattern

# we use -iquote to search for #include "..." and system include directories to search for #include <...>
# unfortunately some people put #include <...> for non-system include files (bad idea!)
# so we have to include some -I for those
################################################################################

CXXFLAGS	+= \
	-I $(WAVE_SRC) \
	-I $(WAVE_BUILD_DIR_DYNAMIC_SRC) \
	-iquote $(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP) \
	-iquote $(BUILD_DIR_DYNAMIC_SRC) \
	-iquote $(POSTGRESQL_INCLUDE_DIR) \
	-I . \
	-iquote $(WAVE_HOME)/../Yang \

CXXFLAGS += \
	-iquote $(ABS_SRC_TOPDIR)/tps/ZebOS/igmpd \

CXXFLAGS += \
	-I $(SRC_ROOT)/Stub \
	-I /usr/include/libxml2

CXXFLAGS += -I $(XERCESC_INCLUDE_DIR)
CXXFLAGS += 	-I $(XALANC_INCLUDE_DIR)
CXXFLAGS                           += -Wno-variadic-macros

export CC     := $(TOOLCHAIN_PREFIX)gcc$(GCCVERSION)

# the strange stuff with RESOURCE_GENERATOR is to avoid parallelism bugs in the recursive makes
# see the manual for explanation

LIB.SO.VER := $(LIB.SO).$(DCM_VERSION_MAJOR).$(DCM_VERSION_MINOR).$(DCM_VERSION_PATCH) 

################################################################################
#  here list goals for which all the dependency makefiles are necessary to 
# include in this makefile
#
#
# for speed, we don't want to include all the dependency makefiles unless we have to
#
# for other goals, if they are objects or their prefixes (relative to build dir), 
# we compute the needed dependency makefiles
#
# for other goals, they don't need any dependencies here
################################################################################
ALL_DEPS_GOALS := all dcm $(LIB.SO.VER) $(LIB.A) sdk

# add these because of a EC bug they have to depend on $(LIB.SO.VER)
ALL_DEPS_GOALS += $(LIB.SO) $(LIB.SO.MAJOR)

# add these because they are obsolete goals so we don't care if they are as slow as before
ALL_DEPS_GOALS += buildversion buildresources buildpostboot

################################################################################
# here put goals that dynamically generate include files for use of our .cpp files
################################################################################
WHO_GENERATES_INCLUDES := yang postboot resources

include Makefile.libDcmClient

################################################################################
# these are all the results of our build in this directory that are needed for the 
# main OS build:
# library with version - in rpms
# library.so - needed to link with
# netconf-subsys - in rpms
# libDcmClient.so - needed to link with
# libDcmClient.so + version - in rpms
# DcmUserTags.wyser in DynamicSource dir - in rpms
# 
# processed yang files - included in customer distribution

# LIB.SO is the libDcm.so file path - it is defined in Makefile.tools
################################################################################
dcm:  $(BUILD_DIR_DYNAMIC_SRC)/DcmUserTags.wyser $(LIB.SO.VER) $(LIB.SO) $(DCM_CLIENT_LIB.SO.VER) $(DCM_CLIENT_LIB.SO)
	


################################################################################
# compute pre-existing (static) sources
#
# objects OBJS are relative to $(BUILD_DIR_OBJ)
################################################################################
SRCS := $(shell find . -name '*.cpp')
OBJS := $(subst ./,,$(SRCS:.cpp=.o))

################################################################################
# objects generated from dynamic sources under $(BUILD_DIR_DYNAMIC_SRC)
# ###############################################################################
# put DcmResourceIds.o first - this is the longest compile
# parallel makes happen to be fastest when long jobs are listed first
DYN_OBJS := DcmResourceIds.o
DYN_OBJS += DcmVersion.o
DYN_OBJS += DcmPostBootProgrammingTable.o

################################################################################
# the building and linking of OBJS and DYN_OBJS is in commonMakefileEnd
# ###############################################################################


yang:
	$(MAKE) -C $(WAVE_HOME)/../Yang all

resources: | $(RESOURCE_GENERATOR)
	$(MAKE) -C Resources -f Makefile.resources RESOURCE_GENERATOR=

postboot :
	$(MAKE) -C PostBoot -f Makefile.postboot

MKFS := $(MAKEFILE_LIST)

# we patch $(NETCONF_TPS_SRC) with $(NETCONF_ROOT_SRC).patch
# the patched file should match $(NETCONF_ROOT_SRC), if not, we bail !
#
# that is why, even though the patched file does not technically depend on $(NETCONF_ROOT_SRC)
# we redo the recipe should $(NETCONF_ROOT_SRC) change, to get a chance to bail out
#
# the user can't substitute a new $(NETCONF_TPS_SRC) by hand, so 
# we don't need to depend on that
#
# we also handle the situation where a build had already happened so the file $(NETCONF_TPS_SRC) not there anymore
# but got backed up - we undo the backup first

# we don't want to autodepend on the unzipped source that would not change anyway
#pragma noautodep */*.c
$(NETCONF_TPS_SRC).patched: $(NETCONF_ROOT_SRC) $(NETCONF_ROOT_SRC).patch $(MKFS)
	if [ -e $(NETCONF_TPS_SRC).backup ]; then $(UNDO_SUBST_PATCHED); fi;
	patch $(NETCONF_TPS_SRC) -o $@ < $(NETCONF_ROOT_SRC).patch
	diff $(NETCONF_ROOT_SRC) $@ || ($(ECHOCMD) "Netconf Patch is not updated." && exit 1)
# end of netconf-subsys stuff


$(BUILD_DIR_DYNAMIC_SRC)/DcmVersion.cpp: phony
	$(MAKE) -C DcmVersion -f Makefile.version


$(WAVE_UBREEZE_BIN): phony
	$(MAKE) -C $(WAVE_UBREEZE_APPLICATION)

#generate GetDataFromClientSource.cpp and GetDataFromClientSource.h files


# does not really make sense to declare this file as precious, as it's not in a chain of implicit rules
# but what can we do, emake for some reason deletes it, probably a bug - notified ElectricCloud
.PRECIOUS: $(BUILD_DIR_DYNAMIC_SRC)/DcmResourceIds.cpp


################################################################################
# here we have the empty recipes for all the dynamically generated targets
# that we don't have another recipe for
#
# make does not automatically reread the timestamp of a prerequisite prior to deciding whether 
# the targets needs to be updated
# to force this for dynamically-generated files, add empty recipes
#
# notice that some of them are not needed technically for dcm, because 
# these targets are not used in dcm, but later will be needed for the whole OS
#
# also notice that the order of implicit rules may be important here:
# we want DcmResource%.h to be matched to resources, not ...Model
# 
# first DcmResource%
# second %.h
# currently with make 3.81 compatibility, the first takes precendence over second
# by order
# after we switch to 3.82 compatibility, the rules are different: the longest match wins
# it will be still OK as "DcmResource" is longer than ".h"
################################################################################
$(BUILD_DIR_DYNAMIC_SRC)/DcmCStatus.h: resources ;
$(BUILD_DIR_DYNAMIC_SRC)/DcmResource%: resources ;
$(BUILD_DIR_DYNAMIC_SRC)/%.properties: resources ;
$(BUILD_DIR_DYNAMIC_SRC)/DcmResourceIds.cpp: resources ;
$(BUILD_DIR_DYNAMIC_SRC)/DcmPostBootProgrammingTable.cpp: postboot ;

$(BUILD_DIR_DYNAMIC_SRC)/DcmUserTags.wyser: $(BUILD_DIR_DYNAMIC_SRC)/wyser_finished $(MKFS) \
	| $($(BUILD_DIR_DYNAMIC_SRC)/.)
	$(ERLANG_BIN)/erlc $(WAVE_HASH_USERTAGS_PGM)
	$(ERLANG_BIN)/erl -noshell -s hashusertags readfile $(BUILD_DIR_DYNAMIC_SRC)/wyserUserTags -s init stop > $@.tmp
	#cd $(WAVE_HOME)/../Yang && $(WAVE_WYSER_BIN) -yp $(WAVE_HOME)/../Yang -generate-h-c -wysertags $@.tmp && mv $@.tmp $@

$(BUILD_DIR_DYNAMIC_SRC)/wyser_finished: $(MKFS) | yang $($(BUILD_DIR_DYNAMIC_SRC)/.) $(WAVE_WYSER_BIN)
	$(WAVE_WYSER_BIN) -yp $(WAVE_HOME)/../Yang -printallnames -of $(BUILD_DIR_DYNAMIC_SRC)/wyserUserTags -maxdepth -maxdepthfile $(BUILD_DIR_DYNAMIC_SRC)/wyserCliDepth
	touch $@

$(WAVE_WYSER_BIN): wavehost | \
	$(WAVE_RESOURCES) $(RESOURCE_GENERATOR) \
	$(WAVE_BUILD_DIR_DYNAMIC_SRC)/. $($(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP)/.)
	$(MAKE) ARCH=$(ARCH_HOST) -C $(WAVE_APPLICATION) \
		CROSS_COMPILE_PLATFORM= WAVE_LIB=  WAVE_RESOURCES=  RESOURCE_GENERATOR= \
		$(WAVE_BUILD_DIR_DYNAMIC_SRC)/.= $(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP)/.= \
		Wyser

wavehost: phony | \
	$(WAVE_RESOURCES) $(RESOURCE_GENERATOR) \
	$($(WAVE_BUILD_DIR_DYNAMIC_SRC)/.) $($(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP)/.)
	$(MAKE) ARCH=$(ARCH_HOST) -C $(WAVE_SRC) \
		CROSS_COMPILE_PLATFORM= WAVE_RESOURCES=  RESOURCE_GENERATOR= \
		$(WAVE_BUILD_DIR_DYNAMIC_SRC)/.= $(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP)/.=


include $(WAVE_SRC)/commonMakefileEnd

LIB.SO.MAJOR := $(LIB.SO).$(DCM_VERSION_MAJOR)
#for backwards compatibility
buildversion: $(BUILD_DIR_OBJ)/DcmVersion.o
buildresources: $(BUILD_DIR_OBJ)/DcmResourceIds.o
buildpostboot: $(BUILD_DIR_OBJ)/DcmPostBootProgrammingTable.o

doxygen: $(BUILD_DIR_DYNAMIC_SRC)
	$(MAKE) -C $(SRC_ROOT)/doxygen -f Makefile.doxygen;

SRC_ROOT    := $(shell pwd)
APPLICATION_ROOT            := $(abspath ../application)

clean: wavehostclean
	rm -rf $(HOME)/build/$(PROJECT)/$(OS)
	rm -rf $(WAVE_WYSER_BIN)
	rm -rf $(WAVE_SRC)/Resources/resources
	rm -rf $(WAVE_PATH)/resourcegeneratorWaveResourceGenerator.$(OS).$(ARCH)

cleanall:

wavehostclean:
	$(MAKE) ARCH=$(ARCH_HOST) -C ../../../../bccb/dsf/Wave/source clean

setcleanflags:
	 IS_DEPEND_NEEDED=false

cleanbuild: clean setcleanflags dcm

$(BUILD_DIR_OBJ):
	mkdir -p $(BUILD_DIR_OBJ);

$(BUILD_DIR_LIB):
	mkdir -p $(BUILD_DIR_LIB);

$(BUILD_DIR_SDK):
	mkdir -p $(BUILD_DIR_SDK);

$(BUILD_DIR_DYNAMIC_SRC):
	mkdir -p $(BUILD_DIR_DYNAMIC_SRC);

NM=$(TOOLCHAIN_PREFIX)nm

install:
	$(NM) ../build/Dcm/Linux/powerpc/optimized/lib/libDcm.so.1.0.0 > libDcm.so.nm

