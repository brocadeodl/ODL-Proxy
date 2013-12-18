MAKEFILE := $(lastword $(MAKEFILE_LIST))

include Makefile.tools
include Makefile.versioninfo

################################################################################
# all the -I and -iquote options are here 

# the performance of -I type flags is sensitive to order and 
# redundancy, so they should be custom fitted to a particular library

# we order the include directories putting the more used closer to top 
# (making sure the included files sequences are as previously)
# this is for speed
# so don't disturb the order unless you have to enforce an inclusion,
# or there comes a difference in usage pattern

# we use -iquote to search for #include "..." and system include directories to search # for #include <...>
# unfortunately some people put #include <...> for non-system include files (bad idea!)
# so we have to include some -I for those
################################################################################

# yes xercesc has to come before target/include (for some reason :)
CXXFLAGS += -I $(XERCESC_INCLUDE_DIR)
CXXFLAGS += -I.
CXXFLAGS += -iquote $(POSTGRESQL_INCLUDE_DIR) -I $(XALANC_INCLUDE_DIR)
CXXFLAGS += -iquote $(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP)

.PHONY: all phony

LIB.SO.VER := $(LIB.SO).$(WAVE_VERSION_MAJOR).$(WAVE_VERSION_MINOR).$(WAVE_VERSION_PATCH)

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
ALL_DEPS_GOALS := all $(LIB.SO.VER) $(LIB.A) sdk internalsdk

#add these because of a EC bug bug this has to depend on $(LIB.SO.VER)
ALL_DEPS_GOALS += $(LIB.SO)

# add these because they are obsolete goals so we don't care if they are as slow as before
ALL_DEPS_GOALS += buildversion buildresources


# LIB.SO is the libWave.so file - defined in Makefile.tools
all: | $(LIB.SO.VER) $(LIB.SO)

################################################################################
# here put goals that dynamically generate include files for use of our .cpp files
################################################################################
WHO_GENERATES_INCLUDES := $(WAVE_RESOURCES) $(DLFCN.H) $(EXTRACTION)


################################################################################
# compute pre-existing (static) sources
#
# objects OBJS are relative to $(BUILD_DIR_OBJ)
################################################################################
SRCS := $(shell find . -name '*.cpp')
OBJS := $(subst ./,,$(SRCS:.cpp=.o))

################################################################################
# objects generated from dynamic sources
# relative to $(BUILD_DIR_OBJ)
#
# objects built from all static .cpp files found in the tree below here
# are handled in the commonMakefileEnd
# ###############################################################################
# put WaveResourceIds.o first - this is the longest compile
# parallel makes happen to be fastest when long jobs are listed first
DYN_OBJS := WaveResourceIds.o
DYN_OBJS += PrismVersion.o


################################################################################
# this object has a special CXXFLAGS
################################################################################
$(BUILD_DIR_OBJ)/Framework/Profiling/WaveProfiler.o: CXXFLAGS := $(filter-out -pedantic,$(CXXFLAGS))

	
# second expansion pass for prerequsites is needed to resolve things like $$(@D)
# these directory targets are recognized by /. suffix, 
# and are existence-only (order-only)  prerequisites for their targets
.SECONDEXPANSION:

# the strange variable name $(@D)/.
# is to avoid parallelism bugs in the recursive makes
# see the manual for explanation
$(BUILD_DIR_DYNAMIC_SRC)/PrismVersion.cpp: phony | $$($$(@D)/.)
	$(MAKE) -C Version -f Makefile.version $(@D)/.=


################################################################################
# all the dynamically generated targets should at least have an empty recipe:
#
# make does not automatically reread the timestamp of a prerequisite prior to deciding whether 
# the targets needs to be updated
# to force this for dynamically-generated files, add empty recipes that will get executed
################################################################################

$(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP)/WaveResourceIds.cpp: | \
	 $(WAVE_RESOURCES) ;

include commonMakefileEnd


################################################################################
# from here to the end is additional stuff not needed for the OS build
################################################################################

LIB.SO.MAJOR := $(LIB.SO).$(WAVE_VERSION_MAJOR)
#backwards compatibility
buildversion: $(BUILD_DIR_OBJ)/Version/PrismVersion.o
buildresources: $(BUILD_DIR_OBJ)/Resources/WaveResourceIds.o

################################################################################
# the targets below are still in the old "script style", not coverted yet to proper makefile style
# could not test them because they are not currently building anyway
# Mark Galeck 3-6-2012
################################################################################

clean:
	rm -rf $(HOME)/build/$(PROJECT)
	rm -rf ../resourcegeneratorWaveResourceGenerator.$(OS).$(ARCH)
	rm -rf Resources/resources

SRC_ROOT               := $(CURDIR)
APPLICATION_ROOT       := $(HOME)/application

