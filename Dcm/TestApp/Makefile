TOPDIR := ../../../../..

all:

include $(WAVE_HOME)/source/Makefile.tools

export APPLICATION_ROOT := $(shell pwd)
export FABOS_ROOT       := $(APPLICATION_ROOT)/../../../..
export SPRINGBOARD      := $(APPLICATION_ROOT)/../../../../..

DCMHOME           := $(shell pwd)/..
WAVE_SRC_ROOT     := $(WAVE_HOME)/source

all: dcm

DCM_SRC_ROOT    := $(DCMHOME)/source
DCM_BUILD_ROOT  := $(DCMHOME)/build/Dcm

include $(WAVE_SRC_ROOT)/Makefile.versioninfo
include $(DCM_SRC_ROOT)/Makefile.versioninfo

export DCM_DYNAMIC_SRC_DIR  := $(DCM_BUILD_ROOT)/$(OS)/$(ARCH)/$(BUILD_TYPE)/DynamicSource
export DCM_LIB_DIR          := $(DCM_BUILD_ROOT)/$(OS)/$(ARCH)/$(BUILD_TYPE)/lib
export DCM_CLIENT_LIB_DIR   := $(DCM_BUILD_ROOT)/$(OS)/$(ARCH)/$(BUILD_TYPE)/lib

CXXFLAGS += -I $(WAVE_HOME)/source
CXXFLAGS += -I $(POSTGRESQL_INCLUDE_DIR) 
CXXFLAGS += -I $(WAVE_BUILD_DIR_DYNAMIC_SRC) -I $(WAVE_BUILD_DIR_DYNAMIC_SRC_ARCH_KIND_INDEP)
CXXFLAGS += -I $(DCM_SRC_ROOT) -I $(DCM_DYNAMIC_SRC_DIR)
CXXFLAGS += -I $(DCM_SRC_ROOT)/Stub
CXXFLAGS += -I /usr/include/libxml2
LIBFILES += -lxml2 -lcurl -lsqlite3

CLIENT_LDFLAGS  := $(LDFLAGS) -L $(DCM_CLIENT_LIB_DIR) -Wl,-R$(DCM_CLIENT_LIB_DIR)
LDFLAGS         += -L $(DCM_LIB_DIR) -Wl,-R$(DCM_LIB_DIR)

CLIENT_LIBFILES := -lDcmClient $(LIBFILES) -luuid
LIBFILES        := -lDcm $(LIBFILES) -luuid


CXXFLAGS += -Wno-variadic-macros

dcm: nsm trilloam ospf vrrp arp ssm mstp l2sys fcoe lldp rmon qos eld udld pim igmp rtm employee_management bgp
	#$(CXX) $(CXXFLAGS) RAS_test.c -c -o RAS_test.$(OS).o 
	#$(CXX) $(CXXFLAGS) $(LDFLAGS) RAS_test.$(OS).o -o RAS_test.$(OS).$(ARCH) $(LIBFILES)
	$(CXX) $(CXXFLAGS) vcenter_app.c -c -o vcenter_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) vcenter_app.$(OS).o -o vcenter_app.$(OS).$(ARCH) $(LIBFILES)
	$(CXX) $(CXXFLAGS) vcenter_app2.c -c -o vcenter_app2.$(OS).o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) vcenter_app2.$(OS).o -o vcenter_app2.$(OS).$(ARCH) $(LIBFILES)

	$(CXX) $(CXXFLAGS) VIC.cpp -c -o VIC.$(OS).o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) VIC.$(OS).o -o VIC.$(OS).$(ARCH) $(LIBFILES)

nsm:
	$(CXX) $(CXXFLAGS) nsm_app.c -c -o nsm_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) nsm_app.$(OS).o -o nsm_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

trilloam:
	$(CXX) $(CXXFLAGS) trilloam_app.c -c -o trilloam_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) trilloam_app.$(OS).o -o trilloam_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

ospf:
	$(CXX) $(CXXFLAGS) ospf_app.c -c -o ospf_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) ospf_app.$(OS).o -o ospf_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

vrrp:
	$(CXX) $(CXXFLAGS) vrrp_app.c -c -o vrrp_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) vrrp_app.$(OS).o -o vrrp_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

arp:
	$(CXX) $(CXXFLAGS) arp_app.c -c -o arp_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) arp_app.$(OS).o -o arp_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

ssm:
	$(CXX) $(CXXFLAGS) ssm_app.c -c -o ssm_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) ssm_app.$(OS).o -o ssm_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

mstp:
	$(CXX) $(CXXFLAGS) mstp_app.c -c -o mstp_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) mstp_app.$(OS).o -o mstp_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

l2sys:
	$(CXX) $(CXXFLAGS) l2sys_app.c -c -o l2sys_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) l2sys_app.$(OS).o -o l2sys_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

fcoe:
	$(CXX) $(CXXFLAGS) fcoe_app.c -c -o fcoe_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) fcoe_app.$(OS).o -o fcoe_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

lldp:
	$(CXX) $(CXXFLAGS) lldp_app.c -c -o lldp_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) lldp_app.$(OS).o -o lldp_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

rmon:
	$(CXX) $(CXXFLAGS) rmon_app.c -c -o rmon_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) rmon_app.$(OS).o -o rmon_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

qos:
	$(CXX) $(CXXFLAGS) qos_app.c -c -o qos_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) qos_app.$(OS).o -o qos_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

bgp: uapp.$(OS).o
	$(CXX) $(CXXFLAGS) bgp_app.c -c -o bgp_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) bgp_app.$(OS).o -o bgp_app.$(OS).$(ARCH) uapp.$(OS).o $(CLIENT_LIBFILES)

eld:
	$(CXX) $(CXXFLAGS) eld_app.c -c -o eld_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) eld_app.$(OS).o -o eld_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

udld: uapp.$(OS).o
	$(CXX) $(CXXFLAGS) udld_app.c -c -o udld_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) udld_app.$(OS).o -o udld_app.$(OS).$(ARCH) uapp.$(OS).o $(CLIENT_LIBFILES)

pim:
	$(CXX) $(CXXFLAGS) pim_app.c -c -o pim_app.$(OS).o 
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) pim_app.$(OS).o -o pim_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

igmp:
	$(CXX) $(CXXFLAGS) igmp_app.c -c -o igmp_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) igmp_app.$(OS).o -o igmp_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

rtm:
	$(CXX) $(CXXFLAGS) rtm_app.c -c -o rtm_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(CLIENT_LDFLAGS) rtm_app.$(OS).o -o rtm_app.$(OS).$(ARCH) $(CLIENT_LIBFILES)

employee_management:
	$(CXX) $(CXXFLAGS) employee_management_app.c -c -o employee_management_app.$(OS).o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) employee_management_app.$(OS).o -o employee_management_app.$(OS).$(ARCH) $(LIBFILES)

uapp.$(OS).o:
	$(CXX) $(CXXFLAGS) uapp.c -c -o uapp.$(OS).o

clean:
	rm -f mstp_app.$(OS).$(ARCH);
	rm -f mstp_app.$(OS).o;
	rm -f l2sys_app.$(OS).$(ARCH);
	rm -f l2sys_app.$(OS).o;
	rm -f nsm_app.$(OS).$(ARCH);
	rm -f nsm_app.$(OS).o;
	rm -f arp_app.$(OS).$(ARCH);
	rm -f arp_app.$(OS).o;
	rm -f ssm_app.$(OS).$(ARCH);
	rm -f ssm_app.$(OS).o;
	rm -f fcoe_app.$(OS).$(ARCH);
	rm -f fcoe_app.$(OS).o;
	rm -f rmon_app.$(OS).$(ARCH);
	rm -f rmon_app.$(OS).o;
	rm -f qos_app.$(OS).$(ARCH);
	rm -f qos_app.$(OS).o;
	rm -f bgp_app.$(OS).$(ARCH);
	rm -f bgp_app.$(OS).o;
	rm -f trilloam_app.$(OS).$(ARCH);
	rm -f trilloam_app.$(OS).o;
	rm -f ospf_app.$(OS).$(ARCH);
	rm -f ospf_app.$(OS).o;
	rm -f vrrp_app.$(OS).$(ARCH);
	rm -f vrrp_app.$(OS).o;
	rm -f vcenter_app.$(OS).o;
	rm -f vcenter_app.$(OS).$(ARCH);
	rm -f vcenter_app2.$(OS).o;
	rm -f vcenter_app2.$(OS).$(ARCH);
	rm -f lldp_app.$(OS).o;
	rm -f lldp_app.$(OS).$(ARCH);
	rm -f pim_app.$(OS).$(ARCH);
	rm -f pim_app.$(OS).o;
	rm -f RAS_test.$(OS).$(ARCH);
	rm -f RAS_test.$(OS).o;
	rm -f eld_app.$(OS).$(ARCH);
	rm -f eld_app.$(OS).o;
	rm -f udld_app.$(OS).$(ARCH);
	rm -f udld_app.$(OS).o;
	rm -f igmp_app.$(OS).$(ARCH);
	rm -f igmp_app.$(OS).o;
	rm -f rtm_app.$(OS).$(ARCH);
	rm -f rtm_app.$(OS).o;
	rm -f VIC.$(OS).$(ARCH);
	rm -f VIC.$(OS).o;
	rm -f employee_management_app.$(OS).$(ARCH);
	rm -f employee_management_app.$(OS).o;
	rm -f uapp.$(OS).o;
	rm -rf *.db *.log *.trc;
	rm -rf node*;

