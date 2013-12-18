TOPDIR := ../../../../..

include ../source/Makefile.versioninfo
include $(WAVE_HOME)/source/Makefile.tools
include  $(WAVE_HOME)/source/Makefile.versioninfo

.PHONY: all dcm cli phony
all: cli dcm

LIBFILES	+= -lDcm

LIBFILES += -lpam_misc

LDFLAGS		+= -L $(BUILD_DIR_LIB) -Wl,-R$(BUILD_DIR_LIB) 

# the -I and -iquote options are specified only here

CXXFLAGS += \
		-I $(WAVE_HOME)/source \
		-iquote $(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP) \
		-iquote $(SRC_ROOT) \
		-iquote $(BUILD_DIR_DYNAMIC_SRC) \
		-iquote $(POSTGRESQL_INCLUDE_DIR) \

    CXXFLAGS	+= -I $(SRC_ROOT)/Stub
	 CXXFLAGS	+= -I /usr/include/libxml2
    LIBFILES	+= -lxml2 -lcurl -lsqlite3

# the strange stuff with LIB WAVE_LIB WAVE_RESOURCES GENERATOR 
# is to avoid parallelism bugs with the recursive makes
# see the manual for explanation

WAVE_LIB.SO := $(WAVE_BUILD_DIR_LIB)/libWave.so
WAVE_LIB := $(WAVE_LIB.SO)
DCM_LIB := $(LIB.SO)
WAVE_LIB.SO.VER := \
$(WAVE_LIB).$(WAVE_VERSION_MAJOR).$(WAVE_VERSION_MINOR).$(WAVE_VERSION_PATCH)
LIB.SO.VER := $(LIB.SO).$(DCM_VERSION_MAJOR).$(DCM_VERSION_MINOR).$(DCM_VERSION_PATCH)
 
MKFS := $(MAKEFILE_LIST)
COMMON_DEPENDENCIES := $(DCM_LIB) $(WAVE_LIB) $(MKFS)


################################################################################
# list here dependencies for programs
# other than the common dependencies listed above
#
# the rule for programs in commonMakefileEnd
################################################################################

dcm: | Dcmd.$(OS_ARCH_XT) Fscd.$(OS_ARCH_XT) NosSmd.$(OS_ARCH_XT) DcmKernelInterface.$(OS_ARCH_XT)

Dcmd.$(OS_ARCH_XT): Dcmd.cpp
Fscd.$(OS_ARCH_XT): Fscd.cpp
NosSmd.$(OS_ARCH_XT): NosSmd.cpp
DcmKernelInterface.$(OS_ARCH_XT): DcmKernelInterface.cpp

cli:  | DcmClient.$(OS_ARCH_XT) NspClient.$(OS_ARCH_XT)

#use relative paths here only
DcmClient.$(OS_ARCH_XT): DcmClient.cpp ../../Wave/application/WaveCliReplyThread.cpp 
NspClient.$(OS_ARCH_XT) : NspClient.cpp ../../Wave/application/WaveCliReplyThread.cpp 

ifdef WAVE_LIB
$(WAVE_LIB.SO.VER): phony | $(WAVE_RESOURCES) $(RESOURCE_GENERATOR)
	$(MAKE) -C $(WAVE_SRC) $@ WAVE_RESOURCES= RESOURCE_GENERATOR=

.SECONDEXPANSION:
################################################################################
# make links
#
#the links do not depend on their targets, and in fact may be created to not-yet existing targets
# the order-only dependency on $(LIB.SO.VER) is however necessary to avoid emake conflicts, 
# that's because gnu make accesses a link target to cache it and emake has to do that to for 
# compatibility
################################################################################
$(WAVE_LIB.SO) $(WAVE_LIB.SO.MAJOR):  | $$(@D)/. $(WAVE_LIB.SO.VER)
	ln -sf $(notdir $(WAVE_LIB.SO.VER)) $@
endif


include $(WAVE_SRC)/commonMakefileEnd


################################################################################
# the targets below are still in the "script style", not coverted yet to makefile style
# Mark Galeck 3-12-2012
################################################################################
WAVE_SRC_ROOT     := $(WAVE_HOME)/source

DCM_TESTAPP_ROOT := $(HOME)/TestApp

clean: wave_clean dcm_clean
	rm -rf Dcmd.$(OS_ARCH_XT);
	rm -rf Fscd.$(OS_ARCH_XT);
	rm -rf DcmClient.$(OS_ARCH_XT);
	rm -rf NspClient.$(OS_ARCH_XT);
	rm -rf RedundancyOptimizer.$(OS_ARCH_XT);
	rm -rf dcmsobundle.$(OS_ARCH_XT).tar.gz;
	rm -rf dcmso.$(OS_ARCH_XT).tar.gz;
	rm -rf NosSmd.$(OS).$(ARCH);
	rm -rf DcmKernelInterface.$(OS).$(ARCH);
	rm -rf *.db *.log *.trc;
	rm -rf node*;

wave_clean:
	$(MAKE) -C $(WAVE_SRC) clean

dcm_clean:
	$(MAKE) -C $(SRC_ROOT) clean

cleanall:
	rm -f Dcmd.*.*;
	rm -f Fscd.*.*;
	rm -f NosSmd.*.*;
	rm -f DcmKernelInterface.*.*;
	rm -f DcmClient.*.*;
	rm -f NspClient.*.*;
	rm -f RedundancyOptimizer.*.*;
	rm -rf *.db *.log *.trc;

stoptestapps:
	cd node$(DCM_INSTANCE); \
	ps ax | grep -w "nsm_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - nsm_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - nsm_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep "ospf_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - ospf_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - ospf_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep "pim_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - pim_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - pim_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep "vrrp_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - vrrp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - vrrp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep -w "fcoe_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - fcoe_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - fcoe_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep -w "l2sys_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - l2sys_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - l2sys_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep -w "lldp_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - lldp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - lldp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep -w "ssm_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - ssm_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - ssm_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep -w "qos_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - qos_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - qos_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep -w "arp_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
	echo "Killing - arp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - arp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep -w "mstp_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - mstp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - mstp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep "igmp_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
		echo "Killing - igmp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
		echo "No Process - igmp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep "rtm_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
	echo "Killing - rtm_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
	echo "No Process - rtm_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
    ps ax | grep "eld_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
    if [ -s .tmp.pid ]; then \
    echo "Killing - eld_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
        kill -9 `cat .tmp.pid`; \
    else \
    echo "No Process - eld_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
    fi
	cd node$(DCM_INSTANCE); \
    ps ax | grep "udld_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
    if [ -s .tmp.pid ]; then \
    echo "Killing - udld_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
        kill -9 `cat .tmp.pid`; \
    else \
    echo "No Process - udld_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
    fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep "bgp_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
	echo "Killing - bgp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
    else \
	echo "No Process - bgp_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi
	cd node$(DCM_INSTANCE); \
	ps ax | grep "employee_management_app.Linux.$(ARCH) $(DCM_INSTANCE)" | grep -v grep | sed "s/^ \ *//g" | cut -f 1 -d " " > .tmp.pid; \
	if [ -s .tmp.pid ]; then \
	echo "Killing - employee_management_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
		kill -9 `cat .tmp.pid`; \
	else \
	echo "No Process - employee_management_app.Linux.$(ARCH) $(DCM_INSTANCE)"; \
	fi

starttestapps: stoptestapps
	echo "Starting nsm_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/nsm_app.Linux.$(ARCH) $(DCM_INSTANCE) > nsm_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting ospf_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/ospf_app.Linux.$(ARCH) $(DCM_INSTANCE) > ospf_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting pim_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/pim_app.Linux.$(ARCH) $(DCM_INSTANCE) > pim_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting vrrp_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/vrrp_app.Linux.$(ARCH) $(DCM_INSTANCE) > vrrp_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting fcoe_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/fcoe_app.Linux.$(ARCH) $(DCM_INSTANCE) > fcoe_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting l2sys_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/l2sys_app.Linux.$(ARCH) $(DCM_INSTANCE) > l2sys_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting lldp_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/lldp_app.Linux.$(ARCH) $(DCM_INSTANCE) > lldp_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting ssm_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/ssm_app.Linux.$(ARCH) $(DCM_INSTANCE) > ssm_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting qos_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/qos_app.Linux.$(ARCH) $(DCM_INSTANCE) > qos_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting arp_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/arp_app.Linux.$(ARCH) $(DCM_INSTANCE) > arp_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting mstp_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/mstp_app.Linux.$(ARCH) $(DCM_INSTANCE) > mstp_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting igmp_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/igmp_app.Linux.$(ARCH) $(DCM_INSTANCE) > igmp_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting rtm_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/rtm_app.Linux.$(ARCH) $(DCM_INSTANCE) > rtm_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting eld_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/eld_app.Linux.$(ARCH) $(DCM_INSTANCE) > eld_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting udld_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/udld_app.Linux.$(ARCH) $(DCM_INSTANCE) > udld_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting bgp_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/bgp_app.Linux.$(ARCH) $(DCM_INSTANCE) > bgp_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &
	echo "Starting employee_management_app.Linux.$(ARCH) $(DCM_INSTANCE)";
	cd node$(DCM_INSTANCE) && $(DCM_TESTAPP_ROOT)/employee_management_app.Linux.$(ARCH) $(DCM_INSTANCE) > employee_management_app.Linux.$(ARCH).$(DCM_INSTANCE).txt 2>&1 &

