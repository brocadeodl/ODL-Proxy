/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NSMDINTF_H
#define NSMDINTF_H

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/InterfaceCommon.h"
//#include "ClientInterface/APPM/APPMMessageDef.h"

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>

#define NSM_DAEMON "nsm"
#define NSM_GVLAN_PERSISTENCE_MO_NAME "GVlan"
#define GVLAN_STATE_DISABLE 0
#define GVLAN_STATE_ENABLE 1

#define NSMVLANINTF 6000
#define NSMPOINTF 6001
#define NSMPHYINTF 6002
#define NSMGLOBALLACPCONFIG 6003
#define NSMFCOEVLAN 6004
#define NSMFCOEPORT 6005
#define NSM_DEFAULT_CONFIG_CMD 6006
#define NSMECMPCONFIG 6007
#define NSMBEACONMSG 6008
#define NSMVLAGCONFIG 6009
#define VLAGEXMREBALANCE 6010
#define NSMCLIENTRESERVEDVLAN 6011
#define NSM_VCS_ISL_DISABLE 6012
#define SW_VLAN_CLASSIFICATION_MESSAGE 6013
#define NSM_VLAN_PROVISION 6015
#define MAC_GROUP_CONFIG_MESSAGE 6016
#define GVLAN_ENABLE_MESSAGE 6017

/* NSM Show cmd */
#define NSM_SHOW_INTERFACE 6100
#define NSM_PORTMEDIA_SHOW_INTERFACE 6200
#define NSM_DEBUG_HANDLER 6300
#define NSM_SHOW_ECMP 6400
#define NSM_SHOW_VLAG 6500
#define SHOW_SFP_MESSAGE 6600

#define LACP_SHOW 7000
#define LACP_SHOW_COUNTER 7001
#define LACP_SHOW_SYSID 7002
#define LACP_SHOW_PORTPORTCHANNEL 7003
#define LACP_SHOW_PORTCHANNELDETAILS 7004
#define LACP_DEBUG_DCMD 7005
#define LACP_CLEARCOUNTER 7006
#define LACP_MAX_LINKS 288
#define DCM_LACP_DEFAULT_PORT_PRIORITY	0x8000 
#define DCM_LACP_DEFAULT_SYSTEM_PRIORITY	0x8000 
#define LACP_DEFAULT_DEFAULT_UP            false
#define MAX_LEN_OF_PORT_PROFILE_NAME        128
#define MAX_LEN_OF_PORT_PROFILE_DOMAIN_NAME 128

#define DCM_AGG_CONFIG_NONE             0
#define DCM_AGG_CONFIG_LACP             1
#define DCM_AGG_CONFIG_STATIC           2
#define DCM_PORTCHANNEL_MAX_LAGS        6144
#define DCM_PORTCHANNEL_MAX_VLAGS       6144
#define DCM_PORTCHANNEL_MAX_NUM_VLAGS   512
#define DCM_PORTCHANNEL_MAX_POS_SA      64
#define DCM_PORTCHANNEL_MAX_RBRIDGES    8

/*Maximum ports per port-channel in SA mode(standard)*/
#define DCM_MAX_AGGREGATOR_LINKS_SA		16
/*Maximum ports per port-channel in VCS mode(standard)*/
#define DCM_MAX_AGGREGATOR_LINKS_VCS	32
#define DCM_MAX_AGG_LINKS_PER_NODE_VCS  16
/*Maximum ports per port-channel(brocade)*/
#define DCM_MAX_BRCD_AGGREGATOR_LINKS 	8
/*Maximum brocade trunks per chip*/
#define DCM_MAX_BRCD_AGGREGATOR_PER_CHIP 6
/* Size of LACP System Id, which is the switch base MAC address */
#define VLAG_LACP_GRP_ADDR_LEN     6         /* Copied from tps/ZebOS/lib/nsm/nsm_message.h */

/* Reply codes to NSM to determine the result of its soft database requests to set Primary Link Domain Id */
#define VLAG_PRIMARY_LINK_SELECTED       0	/* Successfully set vLAG Primary Link Domain Id */
#define VLAG_FULL                        1	/* Internal error - reached maximum number of vLAGS */
#define VLAG_PRIMARY_LINK_ALREADY_SELECTED    2	/* vLAG Primary Link Domain Id was already set by another switch */
#define VLAG_PARTNER_MISMATCH            3	/* vLAG Partner information does not match existing Partner information */
#define VLAG_PRIMARY_LINK_NOT_SELECTED   4	/* Cannot clear Primary Link since it was set by another switch */
#define VLAG_IS_WRONG_TYPE               5	/* Internal error - vLAG is different type */
#define VLAG_PRIMARY_LINK_CLEARED        6	/* Successfully cleared vLAG Primary Link Domain Id */

#define MAX_STR 64
#define MAX_ON_OFF_STR 4
#define IF_LONG_NAMESIZE			64
#define MAX_DEBUG_TRACE 1024
#define MAX_INTERFACE_NAMSIZ 	20
#define MAX_SET_DEBUG 16
#define NSM_VLANCLASSIFIER_ALL_GROUP	0
#define NSM_NATIVE_VLAN_DEFAULT		1
/* Maximum number of secondary adresses */
#define NSM_INTERFACE_ADDRESS_MAX  (255)

#define DCMD_NSM_IF_TYPE_ETHERNET       2
#define DCMD_NSM_IF_TYPE_AGGREGATE      9

#define IS_CMDCODESET(cmdcode, flag) (cmdcode & flag)

#define NSM_USER_ABORTED 1000
#define HAL_IF_CAP_1G           0x80  /* interface is capable of 1Gig only*/
#define HAL_IF_CAP_ISL_CAPABLE  0x100 /* interface is capable of ISL */

#define INTF_PER_PAGE 10
#define NETCONF_INTF_PER_PAGE 70

#define MAX_ALLOWED_VLAN_ID    8191
#define MAX_VLANS_IN_NSM        8191
#define MAX_DOT1Q_VLAN_ID		4096
#define VLAN_ID_4091			4091
#define MAX_LENGTH_MAC          16 /* The mac address should be of the format "000a.000b.000c" */
#define MAC_ADDR_LENGTH_IN_BYTES 6 /* The mac address is of 48 bits or 6 bytes */
#define INVALID_VNI             0 
#define INVALID_MAC             ""
#define INVALID_CTAG            0
#define INVALID_VLAN            0
#define INVALID_SLOT_ID         -1

#define MAX_PVLAN_DOMAIN_ON_PROMISCUOS 24
#define NSM_MAX_NUM_SEC_PVLANS_PER_PVLAN_DOMAIN	24
#define NSM_MAX_NUM_PVLAN_DOMAIN_SUPPORTED_CAL	31
#define NSM_MAX_NUM_PVLAN_DOMAIN_SUPPORTED_MER	1000

typedef enum
    {
        DEFAULT_INTERFACE = 0
    }NsmDefaultConfigureCmdsId;

/* Tag Names */
typedef enum
    {
        NSMSHOW_INTERFACE = 0,
        NSM_DEFAULT_CONFIG_RESPONSE,
        NSM_UPDATE_INTf_CONFIG,
		NSMSHOW_IP_INTERFACE,
		NSM_SHOW_PORTCHANNEL,
		NSM_SHOW_PORTCHANNEL_DETAIL,
		NSM_SHOW_PORTCHANNEL_ID,
		NSMSHOW_VLAN_BR,
		NSMSHOW_ECMP,
		NSMSHOW_VLAG
    }NsmMessageBufferId;

typedef enum
    {
        CHASSIS_DISABLE,
        CHASSIS_ENABLE,
        SLOT_OFFLINE,
        SLOT_ONLINE
    }NsmModifyPhyCmd;

/* Tag Names */
typedef enum
    {
        NSMPORTMEDIASHOW_INTERFACE = 0, 
        NSMPORTMEDIA_DEFAULT_CONFIG_RESPONSE 
    }NsmPortMediaMessageBufferId;


/* Tag Names */
typedef enum
    {
        LACPSHOW = 0,
		LACPSHOWCOUNTER,
		LACPSHOWSYSID,
		LACPSHOWPORTPORTCHANNEL,
		LACPSHOWPORTCHANNELDETAILS,
        LACPSHOWDEBUG
	}LacpMessageBufferId;

/* show nsm cmd code */
typedef enum
    {
        NSMSHOW_UNKNOWN = 0,
        NSMSHOW_ALL,
        NSMSHOW_INTFALL ,
        NSMSHOW_VLANALL ,
        NSMSHOW_POALL ,
		NSMSHOW_IP_INTF,
		NSMSHOW_PORTCHANNEL,
		NSMSHOW_PORTCHANNEL_DETAIL,
		NSMSHOW_PORTCHANNEL_ID,
		NSMSHOW_PORTCHANNEL_LOADBALANCE,
		NSMSHOW_VLAN_BRIEF,
        NSMSHOW_VLAN_BRIEF_PROVISIONED,
        NSMSHOW_VLAN_BRIEF_UNPROVISIONED,
		NSMSHOW_VLAN_PVLAN,
		NSMSHOW_VLAN_VLANID,
		NSMSHOW_ECMP_LOADBALANCE,
		NSMSHOW_VLAG_LOADBALANCE,
        NSMSHOW_ALL_PORTCHANNEL_IDS,
		NSMSHOW_REMOTE_SPAN_VLAN,
        NSMSHOW_IF_STATUS,
        NSMSHOW_IF_DESC,
        NSMSHOW_IF_ERR_COUNTERS
    }NsmShowIntfCmdCode;


/* show nsm cmd code */
typedef enum
    {
        NSMPORTMEDIASHOW_ALL = 0, 
        NSMPORTMEDIASHOW_INTF = 1,
        NSMPORTMEDIASHOW_LINECARD 
    }NsmShowPortMediaCmdCode;



/* [no] debug nsm cmd code */
typedef enum
    {
		NSMDEBUGSET_ALL = 0,
		NSMDEBUGSET_LEVEL,
		NSMDEBUGSET_EVENTS,
		NSMDEBUGSET_PACKET,
		NSMDEBUGSET_PACKET_RECV,
		NSMDEBUGSET_PACKET_SEND,
		NSMDEBUGSET_PACKET_RS_DETAIL,
		NSMDEBUGSET_PACKET_DETAIL,
		NSMDEBUGSET_KERNEL,
		NSMDEBUGSET_HA,
		NSMDEBUGSET_HA_ALL,
		NSMDEBUGSET_FCOE,
		NSMDEBUGSET_ALL_NO,
		NSMDEBUGSET_LEVEL_NO,
		NSMDEBUGSET_EVENTS_NO,
		NSMDEBUGSET_PACKET_NO,
		NSMDEBUGSET_PACKET_RECV_NO,
		NSMDEBUGSET_PACKET_SEND_NO,
		NSMDEBUGSET_PACKET_RS_DETAIL_NO,
		NSMDEBUGSET_PACKET_DETAIL_NO,
		NSMDEBUGSET_KERNEL_NO,
		NSMDEBUGSET_HA_NO,
		NSMDEBUGSET_HA_ALL_NO,
		NSMDEBUGSET_FCOE_NO,
		NSMDEBUGSHOW_TRACE,
        ALLDEBUGSET_ALL_NO
    }NsmDebugCmdCode;

/* 'clear counters' cmd code */
typedef enum
    {
        NSM_CLEAR_COUNTERS_ALL = 10 ,
        NSM_CLEAR_COUNTERS_IF,
        NSM_CLEAR_COUNTERS_SLOT,
		NSM_CLEAR_COUNTERS_MAX
    }NsmClearCountersCmdCode;

/* show lacp cmd code */
typedef enum
    {
        LACPSHOW_UNKNOWN = 0 ,
        LACPSHOW_SYSID,
		LACPSHOW_COUNTER,
        LACPSHOW_PORT_PORTCHANNEL,
        LACPSHOW_PORTCHANNEL_DETAILS,
        LACP_SHOW_DEBUG_CMD
	}LacpShowCmdCode;
/* Clear lacp counter cmd code */
typedef enum
	{
		LACP_CLEAR_COUNTERS_UNKNOWN	= 0,
		LACP_CLEAR_COUNTERS,
		LACP_CLEAR_PORTCHANNEL_COUNTERS
	}LacpClear_cmd_t;
typedef enum
    {
        NSM_LACP_SHOWBUF_UNKNOWN = 0,
        NSM_LACP_SHOWBUF_PORTCHANNEL,
        NSM_LACP_SHOWBUF_PORTCHANNEL_SUMMARY,
        NSM_LACP_SHOWBUF_PORTCHANNEL_DETAILS,
        NSM_LACP_SHOWBUF_PORTCHANNEL_LOADBALANCE,
        NSM_LACP_SHOWBUF_PORTCHANNEL_ID
    }LacpShowBufCode;
	
	typedef enum
    {
        NSM_PHY_PO_ACCESS = 0 ,
        NSM_PHY_PO_COVERGED ,
        NSM_PHY_PO_TRUNK ,
        NSM_PHY_PO_PVLAN_TRUNK ,
        NSM_PHY_PO_PVLAN_TRUNK_PROMISCUOUS ,
        NSM_PHY_PO_PVLAN_HOST ,
        NSM_PHY_PO_PVLAN_TRUNK_HOST 
    }NsmPhyPoIntfMode;

#define NSMUPDATE_VLAN_CREATE_CMD  (1 << 0)
#define NSMUPDATE_VLAN_DESC_CMD    (1 << 1)
#define NSMUPDATE_VLAN_STATUS_CMD  (1 << 2)
#define NSMUPDATE_PVLAN_TYPE_CMD   (1 << 3)
#define NSMUPDATE_PVLAN_SEC_ASSOC_RANGE_CMD   (1 << 4)

typedef enum
    {
        NSMUPDATE_VLAN_DESC = 0 ,
        NSMUPDATE_VLAN_DELDESC ,
        NSMUPDATE_VLAN_MTU ,
        NSMUPDATE_VLAN_DELMTU ,
        NSMUPDATE_VLAN_STATUS ,
        NSMUPDATE_VLAN_STP_STATUS ,
        NSMUPDATE_VLAN_DELSTATUS,
        NSMUPDATE_VLAN_PVLAN_TYPE ,
        NSMUPDATE_VLAN_DELPVLAN_TYPE,
        NSMUPDATE_VLAN_PVLAN_SEC_VLAN ,
        NSMUPDATE_VLAN_DELPVLAN_SEC_VLAN,         /* private-vlan remove v1 */
        NSMUPDATE_VLAN_DELPVLAN_SEC_VLAN_ALL,     /* no private-vlan */

        NSMCREATE_VLAN_INTF ,
        NSMUPDATE_VLAN_INTF,
        NSMDELETE_VLAN_INTF,

        NSMUPDATE_VLAN_BULK_CONF,
		NSMUPDATE_VLAN_RXLIM,

		NSMUPDATE_VLAN_PVLAN_ADD_SEC_ASSOC_TYPE,
		NSMUPDATE_VLAN_PVLAN_DEL_SEC_ASSOC_TYPE,
		NSMUPDATE_VLAN_DELPVLAN_SEC_ASSOC_TYPE,

		NSMUPDATE_VLAN_REMOTE_SPAN,
		NSMUPDATE_VLAN_MAX
    }NsmGlobalUpdateVlanIntfCmdCode;

typedef enum
{
	VLANCLASSIFIER_SHOW_UNKNOWN = 0,
	VLANCLASSIFIER_SHOW,//This is wild card
	VLANCLASSIFIER_SHOW_RULE,
	VLANCLASSIFIER_SHOW_RULE_ALL,
	VLANCLASSIFIER_SHOW_GROUP,
	VLANCLASSIFIER_SHOW_GROUP_ALL,
	VLANCLASSIFIER_SHOW_INTF_TE,
	VLANCLASSIFIER_SHOW_INTF_PO,
	VLANCLASSIFIER_SHOW_INTF_GROUP,
	VLANCLASSIFIER_SHOW_INTF_GROUP_ALL
}VlanClassifierShowCmdCode;


/*Vlan Classifier input Msg*/
typedef struct VlanClassifierShow_InputMsg_t_ {
	char back_end_data[MAX_STR];
	unsigned int cmdCode;
	unsigned short int last_rule_id;
	unsigned short int last_group_id;
}VlanClassifierShow_InputMsg_t;

/*Vlan Classifier Show Output message*/

typedef struct VlanClassifierShow_OutputMsg_t_ {
	unsigned short int vlan;
	unsigned short int rule;
	unsigned short int group;
	char app_data[MAX_STR];
}VlanClassifierShow_OutputMsg_t;

typedef struct VlanClassifierShow_Cmd_t_ {
	unsigned int no_of_records;
	unsigned char more_rec_flag;
	VlanClassifierShow_OutputMsg_t vlan_classifier[1];
}VlanClassifierShow_Cmd_t;	


#define NSMUPDATE_PO_CREATE_CMD    (0x1 <<0)
#define NSMUPDATE_PO_DESC_CMD    (0x1 <<1)
#define NSMUPDATE_PO_MTU_CMD     (0x1 <<2)
#define NSMUPDATE_PO_SW_CMD      (0x1 <<3)
#define NSMUPDATE_PO_MODE_CMD    (0x1 <<4)
#define NSMUPDATE_PO_STATUS_CMD  (0x1 <<5)
#define NSMUPDATE_PO_MODETYPE_CMD  (0x1 <<6)
#define NSMUPDATE_PO_SW_ADD_VLAN_CMD (0x1 <<7)
#define NSMUPDATE_PO_SW_TRUNK_VLAN_ALL_CMD   (0x1 <<8)
#define NSMUPDATE_PO_SW_ADD_ACCESS_VLAN_CMD (0x1 <<10) 
#define NSMUPDATE_PO_SW_ADD_TRUNK_VLAN_CMD  (0x1 <<11) 
#define NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN_CMD (0x1 <<12)
#define NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN_CMD (0x1 <<13)
#define NSMUPDATE_PO_NATIVEVLANDISABLE_CMD      (0x1 <<14)
#define NSMUPDATE_PO_NATIVEVLANID_CMD			(0x1 <<15)
#define NSMUPDATE_PO_DELSTATUS_CMD				(0x1 <<16)
#define NSMUPDATE_PO_MINLINKS_CMD				(0x1 <<17)
#define NSMUPDATE_PO_LOADBALANCE_CMD			(0x1 <<18)
#define NSMUPDATE_PO_IGNORE_SPLIT_CMD     (0x1 <<19)
#define NSMUPDATE_PO_NO_IGNORE_SPLIT_CMD  (0x1 <<20)
#define NSMUPDATE_PO_PORT_PROFILED_CMD	(0x1 <<21)
#define NSMUPDATE_PO_ALLOWEDSPEED_CMD    (0x1 <<22)
#define NSMUPDATE_PO_RA_GUARD_CMD        (0x1 <<23)

    typedef enum
    {
        NSM_PO_MODE_ACTIVE = 1,
        NSM_PO_MODE_PASSIVE,
        NSM_PO_MODE_ON
    }NsmPoMode;

    typedef enum
    {
        NSM_PO_TYPE_STANDARD = 1,
        NSM_PO_TYPE_BROCADE
    }NsmPoType;


    typedef enum
    {
        NSMUPDATE_PO_DESC = 0 ,
        NSMUPDATE_PO_DELDESC ,
        NSMUPDATE_PO_MTU ,
        NSMUPDATE_PO_DELMTU ,
        NSMUPDATE_PO_SW ,
        NSMUPDATE_PO_MINLINKS ,
        NSMUPDATE_PO_DELMINLINKS ,
		NSMUPDATE_PO_ALLOWEDSPEED,
        NSMUPDATE_PO_LOADBALANCE ,
        NSMUPDATE_PO_DELLOADBALANCE ,
        NSMUPDATE_PO_DELSW ,
        NSMUPDATE_PO_MODE ,
        NSMUPDATE_PO_POMODE ,
        NSMUPDATE_PO_POTYPE ,
        NSMUPDATE_PO_DELMODE ,
        NSMUPDATE_PO_STATUS ,
        NSMUPDATE_PO_DELSTATUS ,
        NSMUPDATE_PO_IGNORE_SPLIT ,
        NSMUPDATE_PO_NO_IGNORE_SPLIT ,
        NSMUPDATE_PO_PVLAN_MODE ,
        NSMUPDATE_PO_PVLAN_MODE_TRUNK,
        NSMUPDATE_PO_PVLAN_MODE_TRUNK_HOST ,
        NSMUPDATE_PO_PVLAN_MODE_TRUNK_PROMISCUOUS ,
        NSMUPDATE_PO_PVLAN_MODE_PROMISCUOUS ,
        NSMUPDATE_PO_PVLAN_MODE_HOST ,
        NSMUPDATE_PO_DELPVLAN_MODE,
		NSMUPDATE_PO_PVLAN_NATIVE_VLANID,
		NSMUPDATE_PO_PVLAN_NATIVE_VLANID_DISABLE,
		NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ALL,
		NSMUPDATE_PO_PVLAN_TRUNK_VLAN_NONE,
		NSMUPDATE_PO_PVLAN_TRUNK_VLAN_ADD,
		NSMUPDATE_PO_PVLAN_TRUNK_VLAN_REMOVE,
		NSMUPDATE_PO_PVLAN_TRUNK_VLAN_EXCEPT,
        NSMUPDATE_PO_PVLAN_MAPPING ,
        NSMUPDATE_PO_DELPVLAN_MAPPING ,               /*switchport private-vlan mapping v1 remove v2 */
        NSMUPDATE_PO_DELPVLAN_MAPPING_ALL ,           /* no switchport private-vlan mapping */
        NSMUPDATE_PO_PVLAN_HOST_ASSOCIATION ,
        NSMUPDATE_PO_DELPVLAN_HOST_ASSOCIATION ,      /* no switchport private-vlan host-association v1 v2 */
        NSMUPDATE_PO_DELPVLAN_HOST_ASSOCIATION_ALL ,      /* no switchport private-vlan host-association */
        NSMUPDATE_PO_PVLAN_TRUNK_ASSOCIATION ,        
        NSMUPDATE_PO_DELPVLAN_TRUNK_ASSOCIATION ,     /* no switchportprivate-vlan association trunk v1 v2 */
        NSMUPDATE_PO_DELPVLAN_TRUNK_ASSOCIATION_PRIMARY , /* no switchport private-vlan association trunk v1 */
        NSMUPDATE_PO_DELPVLAN_TRUNK_ASSOCIATION_ALL , /* no switchport private-vlan association trunk */

        NSMUPDATE_PO_SW_ADD_ACCESS_VLAN ,
		NSMUPDATE_PO_SW_DELETE_ACCESS_VLAN,
        NSMUPDATE_PO_SW_ADD_TRUNK_VLAN,
        NSMUPDATE_PO_SW_TRUNK_VLAN_ALL,
        NSMUPDATE_PO_SW_TRUNK_VLAN_NONE,
        NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN,
        NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN,
        NSMUPDATE_PO_SW_DELETE_TRUNK,
		NSMUPDATE_PO_PORT_PROFILED,
        NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PO,
        NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO,
        NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PO,
        NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PO,
        NSMUPDATE_ASSOCIATE_BULK_PORT_PROFILE_WITH_PO,
        NSMUPDATE_ASSOCIATE_BULK_PORT_PROFILE_DOMAIN_WITH_PO,

        /* The following 3 command codes are sent by NSM to the Global plug-in. */
        NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID ,         /* NSM request to set vLAG primary link domain id */
        NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_OVERRIDE ,/* NSM request to override vLAG primary link domain id */
        NSMUPDATE_PO_RT_PRIMARY_LINK_DOMAIN_ID_CLEAR ,   /* NSM request to clear vLAG primary link domain id */

        /* The following command code is sent by the Global plug-in to NSM */
        NSMUPDATE_PO_RT_LACP_SYSTEM_ID,                  /* Global plug-in sets new vLAG LACP System id */
		
        NSMUPDATE_PO_CHANNELGROUP_FIRSTMEMBER,

        NSMCREATE_PO_INTF,
        NSMUPDATE_PO_INTF,
        NSMDELETE_PO_INTF,
        NSMADD_PO_TOVLANINTF,
        NSMDELETE_PO_TOVLANINTF,

        NSMUPDATE_PO_NATIVEVLANDISABLE,
        NSMUPDATE_PO_NATIVEVLANID,
		NSMUPDATE_PO_SET_IPV6_RA_GUARD,
		NSMUPDATE_PO_RESET_IPV6_RA_GUARD,

        NSMUPDATE_PO_BULK_CONF,
        NSMUPDATE_PO_EXCEPTION_STATUS,
        NSMUPDATE_PO_SET_MAC_ADDRESS,
        NSMUPDATE_PO_DEL_MAC_ADDRESS,
        NSMUPDATE_PO_SET_MAC_GROUP,
        NSMUPDATE_PO_DEL_MAC_GROUP,
        NSMUPDATE_PO_SET_CTAG,
        NSMUPDATE_PO_DEL_CTAG,
        NSMUPDATE_PO_PVLAN_SET_CTAG,
        NSMUPDATE_PO_PVLAN_DEL_CTAG

    }NsmGlobalUpdatePoIntfCmdCode;

#define NSMUPDATE_PHY_DESC_CMD    (0x1 <<1)
#define NSMUPDATE_PHY_MTU_CMD     (0x1 <<2)
#define NSMUPDATE_PHY_SW_CMD      (0x1 <<3)
#define NSMUPDATE_PHY_MODE_CMD    (0x1 <<4)
#define NSMUPDATE_PHY_STATUS_CMD  (0x1 <<5)
#define NSMUPDATE_PHY_ADD_CHANNELGROUP_CMD  (0x1 <<6)
#define NSMUPDATE_PHY_SW_ADD_VLAN_CMD (0x1 <<7)
#define NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL_CMD   (0x1 <<9)
#define NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN_CMD (0x1 <<10) 
#define NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN_CMD  (0x1 <<11) 
#define NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN_CMD (0x1 <<12)
#define NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN_CMD (0x1 <<13)
#define NSMUPDATE_PHY_LINESPEED_CMD     (0x1 <<14)
#define NSMUPDATE_PHY_NATIVEVLANDISABLE_CMD     (0x1 <<15)
#define NSMUPDATE_PHY_NATIVEVLANID_CMD     (0x1 <<16)
#define NSMUPDATE_PHY_DELSTATUS_CMD  (0x1 <<17)
#define NSMUPDATE_PHY_VEPA_CMD     (0x1 <<18)
#define NSMUPDATE_PHY_ADD_CHANNELGROUP_DBCOMMIT_CMD  (0x1 <<19)
#define NSMUPDATE_PHY_ADD_PORTPROFILEPORT_CMD  (0x1 <<20)
#define NSMUPDATE_PHY_RA_GUARD_CMD (0x1 << 21)
#define NSMUPDATE_PHY_NBR_DISCOVERY_DISABLE_CMD (0x1 << 22)
#define NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN_CMD  (0x1 <<23)
#define NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN_CMD  (0x1 <<24)

    typedef enum
    {
        NSMUPDATE_PHY_DESC = 0 ,
        NSMUPDATE_PHY_DELDESC ,
        NSMUPDATE_PHY_MTU ,
        NSMUPDATE_PHY_DELMTU ,
        NSMUPDATE_PHY_SW,
        NSMUPDATE_PHY_DELSW ,
        NSMUPDATE_PHY_MODE ,
        NSMUPDATE_PHY_DELMODE ,
        NSMUPDATE_PHY_STATUS ,
        NSMUPDATE_PHY_EXCEPTION_STATUS ,
        NSMUPDATE_PHY_DELSTATUS,
        NSMUPDATE_PHY_VEPA ,
        NSMUPDATE_PHY_TIMEOUT,
	 	NSMUPDATE_PHY_PORT_PRIORITY,
	 	NSMUPDATE_PHY_LACP_DEFAULT_UP,
        NSMUPDATE_PHY_LINESPEED,						/* speed */
        NSMUPDATE_PHY_NATIVEVLANDISABLE,
        NSMUPDATE_PHY_NATIVEVLANID,
        NSMUPDATE_PHY_PVLAN_MODE ,
        NSMUPDATE_PHY_PVLAN_MODE_TRUNK,
        NSMUPDATE_PHY_PVLAN_MODE_TRUNK_HOST,
        NSMUPDATE_PHY_PVLAN_MODE_TRUNK_PROMISCUOUS,
        NSMUPDATE_PHY_PVLAN_MODE_PROMISCUOUS,
        NSMUPDATE_PHY_PVLAN_MODE_HOST,
        NSMUPDATE_PHY_DELPVLAN_MODE,
        NSMUPDATE_PHY_PVLAN_NATIVE_VLANID,
        NSMUPDATE_PHY_PVLAN_NATIVE_VLANID_DISABLE,
        NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ALL,
        NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_NONE,
        NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_ADD,
        NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_REMOVE,
        NSMUPDATE_PHY_SW_PVLAN_TRUNK_VLAN_EXCEPT,
        NSMUPDATE_PHY_PVLAN_MAPPING ,
        NSMUPDATE_PHY_DELPVLAN_MAPPING ,               /* switchport private-vlan mapping v1 remove v2 */
        NSMUPDATE_PHY_DELPVLAN_MAPPING_ALL ,           /* no switchport private-vlan mapping */
        NSMUPDATE_PHY_PVLAN_HOST_ASSOCIATION ,
        NSMUPDATE_PHY_DELPVLAN_HOST_ASSOCIATION ,      /* no switchport private-vlan host-association v1 v2 */
        NSMUPDATE_PHY_DELPVLAN_HOST_ASSOCIATION_ALL ,      /* no switchport private-vlan host-association */
        NSMUPDATE_PHY_PVLAN_TRUNK_ASSOCIATION ,        
        NSMUPDATE_PHY_DELPVLAN_TRUNK_ASSOCIATION ,     /* no switchportprivate-vlan association trunk v1 v2 */
        NSMUPDATE_PHY_DELPVLAN_TRUNK_ASSOCIATION_PRIMARY , /* no switchport private-vlan association trunk v1 */
        NSMUPDATE_PHY_DELPVLAN_TRUNK_ASSOCIATION_ALL , /* no switchport private-vlan association trunk */
        NSMCREATE_PHY_INTF,
        NSMUPDATE_PHY_INTF,
        NSMDELETE_PHY_INTF,
        NSM_ADD_CHANNELGROUP,
        NSM_DELETE_CHANNELGROUP,

        NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN ,
		NSMUPDATE_PHY_SW_DELETE_ACCESS_VLAN,
        NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN,
        NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL,
        NSMUPDATE_PHY_SW_TRUNK_VLAN_NONE,
        NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN,
        NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN,
        NSMUPDATE_PHY_SW_DELETE_TRUNK,
		NSMUPDATE_PHY_SET_IPV6_RA_GUARD,					//ipv6 ra-gaurd
		NSMUPDATE_PHY_RESET_IPV6_RA_GUARD,				//no ipv6 ra-gaurd
        
	NSMUPDATE_PHY_SFLOW_ENABLE,
	NSMUPDATE_PHY_SFLOW_POLLING_INTERVAL,
	NSMUPDATE_PHY_SFLOW_SAMPLE_RATE,
	NSMUPDATE_PHY_SFLOW_SET_ALL,
	NSMUPDATE_PHY_DELSFLOW_ENABLE,
	NSMUPDATE_PHY_DELSFLOW_POLLING_INTERVAL,
	NSMUPDATE_PHY_DELSFLOW_SAMPLE_RATE,
    NSMUPDATE_PHY_PORT_PROFILED,/*profiling a port with default port-profile domain*/
    NSMUPDATE_ASSOCIATE_PORT_PROFILE_WITH_PHY_PORT,/*profiling a port with port-profile*/
    NSMUPDATE_ASSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT,/* profiling a port with an user defined port-profile-domain */
    NSMUPDATE_DISSOCIATE_PORT_PROFILE_WITH_PHY_PORT,
    NSMUPDATE_DISSOCIATE_PORT_PROFILE_DOMAIN_WITH_PHY_PORT,
    NSMUPDATE_ASSOCIATE_BULK_PORT_PROFILE_WITH_PHY_PORT,
    NSMUPDATE_ASSOCIATE_BULK_PORT_PROFILE_DOMAIN_WITH_PHY_PORT,
    NSMUPDATE_PHY_BULK_CONF,
	NSMDELETE_EDGE_CONFIG,	
    NSM_PLUGIN_EDGE_TO_ISL,
    NSM_PLUGIN_EDGE_TO_ISL_RESP,
    NSM_PLUGIN_ISL_TO_EDGE,
    NSM_PLUGIN_ISL_TO_EDGE_RESP,

    NSMUPDATE_PHY_ACTVIVE_STATUS,
    NSMUPDATE_PHY_INACTVIVE_STATUS,
 	NSMUPDATE_PHY_ELD,                      /* ELD */
    NSMUPDATE_ELD_PROTO_VAL,                /* ELD */
    NSMUPDATE_ELD_VLAN,						/* ELD */
    NSMUPDATE_ELD_RXLIM,						/* ELD */

    NSMUPDATE_PHY_NBR_DISCOVERY_DISABLE,
    NSMUPDATE_PHY_DELNBR_DISCOVERY_DISABLE,

    NSMUPDATE_PHY_SW_TRUNK_ADD_RSPANVLAN,
    NSMUPDATE_PHY_SW_ACCESS_ADD_RSPANVLAN,
    NSMUPDATE_PHY_SW_TRUNK_DEL_RSPANVLAN,
    NSMUPDATE_PHY_SW_ACCESS_DEL_RSPANVLAN,
	GVLAN_ACCESS_PORT_SET_MAC_ADDRESS,
	GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS,
	GVLAN_ACCESS_PORT_SET_MAC_GROUP,
	GVLAN_ACCESS_PORT_DEL_MAC_GROUP,
	GVLAN_TRUNK_PORT_SET_CTAG,
	GVLAN_TRUNK_PORT_DEL_CTAG,
	GVLAN_PVLAN_TRUNK_PORT_SET_CTAG,
	GVLAN_PVLAN_TRUNK_PORT_DEL_CTAG,

	RSPAN_GVLAN_ACCESS_PORT_SET_MAC_ADDRESS,
	RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS,
	RSPAN_GVLAN_ACCESS_PORT_SET_MAC_GROUP,
	RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_GROUP,
	RSPAN_GVLAN_TRUNK_PORT_SET_CTAG,
	RSPAN_GVLAN_TRUNK_PORT_DEL_CTAG,

    NSMUPDATE_PHY_OPENFLOW,
    NSMUPDATE_PHY_DELOPENFLOW

    }NsmLocalUpdatePhyIntfCmdCode;

typedef enum {
    PVLAN_TYPE_UNKNOWN = 0,
    PVLAN_TYPE_PRIMARY,
    PVLAN_TYPE_ISOLATED,
    PVLAN_TYPE_COMMUNITY
} pvlanTypeEnum;

typedef enum {
    VLAN_TYPE_UNKNOWN = 0,
    VLAN_TYPE_TVLAN
} vlanTypeEnum;


typedef enum {
    PORT_SECURITY_CREATE = 0,
	PORT_SECURITY_REMOVE,
	PORT_SECURITY_ACTION,
	PORT_SECURITY_MAC,
	PORT_SECURITY_MAC_REMOVE,
	PORT_SECURITY_STICKY_MAC,
	PORT_SECURITY_STICKY_MAC_REMOVE,
	PORT_SECURITY_MAC_MAX,
	PORT_SECURITY_STICKY,
	PORT_SECURITY_STICKY_REMOVE,
	PORT_SECURITY_OUI,
	PORT_SECURITY_OUI_REMOVE,
	PORT_SECURITY_SHUTDOWN_TIME,
	STICKY_MAC_CREATE_BULK,
	PORT_SECURITY_CONFIG_REPLAY
	
} portSecEnum;

typedef enum {
    PORT_SECURITY_NO_ACTION = 0,
	PORT_SECURITY_RESTRICT,
	PORT_SECURITY_SHUTDOWN,
	PORT_SECURITY_RASLOG
	
} portSecActionEnum;


/* Line Speed Types */
/*
typedef enum
    {
        LINE_SPEED_AUTO,
        LINE_SPEED_1000,
        LINE_SPEED_10000
    }LineSpeed;
*/

typedef enum
{
    NSMUPDATE_NATIVE_VLAN = 0,
	NSM_PLUGIN_INTF_STATEREPLAY_RESP,
    NSM_PLUGIN_SLOT_CONFIG_REPLAY_COMPLETE_RESP
}NsmGlobalUpdateCmdCode;

typedef enum
{
	NSMUPDATE_VLAG_LOADBALANCE
}NsmVlagUpdateCmdCode;

typedef enum
{
    LACPCREATE_SYS_PRIO = 0 ,
    LACPUPDATE_SYS_PRIO ,
    LACPRESET_SYS_PRIO,
	DOT1XUPDATE_STATUS,
	DOT1XUPDATE_TESTTIMEOUT
}LacpGlobalUpdateCmdCode;

typedef struct LacpGlbUpdConfMsg_ {
    int SystemPriority;
    int dot1xenable;
    unsigned int dot1xtesttimeout;
    unsigned int cmdCode;
}LacpGlbUpdConfMsg_t;

typedef struct NsmGlbUpdConfMsg_ {
    int nativevlanDisable;
	int slotId;
    unsigned int cmdCode;
}NsmGlbUpdConfMsg_t;

typedef enum {
	NSMUPDATE_ECMP_LOADBALANCE=0,
	NSMUPDATE_ECMP_HASHSWAP,
	NSMUPDATE_ECMP_BULK_CONF,
    NSMUPDATE_ECMP_DEL_LOADBALANCE
}EcmpGlobalUpdateCmdCode;

typedef enum {
    SW_MODE_UNKNOWN = 0,
    SW_MODE_ACCESS,
    SW_MODE_TRUNK,
    SW_MODE_CONVERGED,
    SW_MODE_PVLAN_PROMISCUOUS,
    SW_MODE_PVLAN_TRUNK_PROMISCUOUS,
    SW_MODE_PVLAN_HOST,
    SW_MODE_PVLAN_TRUNK_HOST,
    SW_MODE_PVLAN_TRUNK
} swModeEnum;

typedef enum {
   LOADBAL_MODE_DST_MAC_VID = 1,
   LOADBAL_MODE_SRC_MAC_VID,
   LOADBAL_MODE_SRC_DST_MAC_VID,
   LOADBAL_MODE_SRC_DST_IP,
   LOADBAL_MODE_SRC_DST_IP_MAC_VID,
   LOADBAL_MODE_SRC_DST_IP_PORT,
   LOADBAL_MODE_SRC_DST_IP_MAC_VID_PORT
} poLoadBalanceModeEnum;

typedef enum {
    PVLAN_TAG_EMPTY = 0,
    PVLAN_TAG_PROMISCUOUS,
    PVLAN_TAG_HOST,
    PVLAN_TAG_TRUNK
} pvlanTag;

typedef enum {
	PHY_INTERFACE_TYPE = 0,
	PO_INTERFACE_TYPE,
	PO_PHY_INTERFACE_TYPE
} StickyMacRecordsType;


typedef enum {
    PROVISION_SPECIFIED = 1, /* indicates to provision specified vlans */
    PROVISION_ALL,     /* indicates to provision ALL vlans */
    PROVISION_EXCEPT,  /* indicates to provision ALL vlans except specified ones */
    PROVISION_NOT_REQUIRED, /* indicates not to provision any vlans */
    UNPROVISION_ELIGIBLE_VLANS, /* indicates to un-provision eligible vlans */
    UNPROVISION_SPCIFIED,  /* indicates to un-provision specified vlans */
    READ_PROVISIONED_VLANS, /* Request to read provisioned VLANs, in the form 
                            of dcm_nsm_vlan_bmp bitmap */
    PROVISION_NOT_USED /* Used for not equality checks/count, keep it at end */
} vlan_provision_code;

typedef enum {
    NSM_VLAN_PROVISIONED_NOW = 1,   /* provisioned as part of this operation. */
    NSM_VLAN_ALREADY_PROVISIONED, /* was already provisioned */
    NSM_VLAN_UNPROVISIONED_NOW, /* un-provisioned as part of this operation */
    NSM_VLAN_ALREADY_UNPROVISIONED, /* was already un-provisioned */
    NSM_VLAN_PROVISION_FAILED_NO_IVIDS /* insufficient IVIDs */
} vlan_provision_status;

typedef struct NsmVlanIntfMsg_t_ {
    unsigned int vlanId;
    char vlanName[MAX_STR];
    unsigned int vlanIfIndex;
    char desc[MAX_STR];
    int  flag;
    unsigned int cmdCode;
    unsigned int bulkCmdCode;
    int  pvlanType;
    char *pVlanSecAssocRange;
    unsigned int secondaryVlanId;
    unsigned int configuredCmds;
    int rspanEnabled;
    int replayInProgress;
    int  vlanType;
    int  tlsid;
} NsmVlanIntfMsg_t;

typedef struct nsm_vlan_provision_request_ {
    vlan_provision_code provision_code; /* provision type */
    unsigned char need_explicit_unprovision;
    char *vlan_range; /* range of VLANs to be dealt with */
} nsm_vlan_provision_request_t;

/* Structure used to indicate number of ports in a RBridge */
typedef struct rbridge_ports_
{
  u_int16_t domain_id;        /* RBridge id */
  u_int16_t active_ports_cnt; /* Number of active ports in port-channel */
} rbridge_ports_t;

typedef struct NsmEcmpMsg_t_ { 
	unsigned int loadBalanceMode;
	unsigned int cmdCode;
	unsigned int hashSwap;
}NsmEcmpMsg_t;

typedef struct NsmVlagMsg_t_ { 
	unsigned int loadBalanceMode;
	unsigned int cmdCode;
	unsigned int poId;
}NsmVlagMsg_t;

typedef struct NsmPoIntfMsg_t_ {

    char poId[MAX_STR];
    char poName[MAX_STR];
    unsigned int poIfIndex;
    char desc[MAX_STR];
    int flag;
    int swMode;
    unsigned int mode;
    unsigned int mtu;   
    int isAllowedVlanAll;
    char swVlanName[MAX_STR];
    unsigned int poMode;

    /* The following 4 fields are not configured by the user */
    /* The following field is set by NSM for handling BUM traffic */
    unsigned int poPrimaryLinkDomainId;
    /* The following 3 fields are set by NSM to manage dynamic vLAGs */
    unsigned int poPartnerSystemPriority;
    unsigned int poPartnerKey;
    char poPartnerSystem[VLAG_LACP_GRP_ADDR_LEN];/* VLAG_LACP_GRP_ADDR_LEN must be 6 */
    /* The following field is set by the Global plug-in and passed back to NSM */
    char poLacpSystemId[VLAG_LACP_GRP_ADDR_LEN];/* VLAG_LACP_GRP_ADDR_LEN must be 6 */
    unsigned int replyCode;

    unsigned int poType;
    unsigned int poMinLinks;
	unsigned int poAllowedSpeed;
    unsigned int poLoadBalanceMode;
    unsigned int cmdCode;
    unsigned int bulkCmdCode;
    unsigned int primaryVlanId;
    unsigned int secondaryVlanId;

    unsigned int pVlanHostPrimVlanId;
    unsigned int pVlanHostSecVlanId;
    unsigned int pVlanNativeVlanId;

    int swModeFlag;
    unsigned int native_vlan_disable;
    unsigned int native_vlan_id;
    unsigned int configuredCmds;
    unsigned char isPortProfiled;
    char *vlanList;
    char *pVlanList;
    int ignore_split;
    unsigned char raGuard;
    unsigned char mac[MAC_ADDR_LENGTH_IN_BYTES];
    unsigned int ctagId;
	unsigned int macGroupId;

    char portProfileName[MAX_LEN_OF_PORT_PROFILE_NAME + 1];
    char portProfileDomainName[MAX_LEN_OF_PORT_PROFILE_DOMAIN_NAME + 1];
    
} NsmPoIntfMsg_t;

/* Beacon Msg */
typedef struct NsmBeaconMsg_t_ {
	int cliCode;
	char ifName[MAX_STR];
	int cmdId;
} NsmBeaconMsg_t;

typedef struct NsmBeaconMsgOut_t_ {
    char ifName[MAX_STR];
    int ret;
} NsmBeaconMsgOut_t;

typedef struct NsmPhyIntfMsg_t_ {

    char ifName[MAX_STR];
    char poName[MAX_STR];
    unsigned int swIfIndex;
    unsigned int ifType;
    char desc[MAX_STR];
    int flag;
    int swMode;
    unsigned int mode;
    unsigned int mtu;
    unsigned int openflow;
    unsigned int line_speed;
    unsigned int native_vlan_disable;
    unsigned int native_vlan_id;
    int isAllowedVlanAll;
    char swVlanName[MAX_STR];
    unsigned int cmdCode;
    unsigned int bulkCmdCode;
    unsigned int primaryVlanId;
    unsigned int secondaryVlanId;

    unsigned int pVlanHostPrimVlanId;
    unsigned int pVlanHostSecVlanId;

    int swModeFlag;
    int lacpTimeout;
	int lacpPortPriority;
	int lacpDefaultUp;
    int enable;
    int vepa;
    int pollingInterval;
    int sampleRate;
    unsigned int configuredCmds;
    unsigned char isPortProfiled;
	unsigned int msgId;
	unsigned int exception;
    int poMode;
    int poType;
    unsigned int poLacpSystemIdDomainId;
    unsigned int pVlanNativeVlanId;

    char *vlanList;
    char *pVlanList;
    char polacpSystemId[MAX_STR];
	unsigned char raGuard;
	unsigned char mac[MAC_ADDR_LENGTH_IN_BYTES];
	unsigned int ctagId;
	unsigned int macGroupId;

    //Set the port-profile name/ port-profile domain name when a port is put in
    //port-profiled mode.
    char portProfileName[MAX_LEN_OF_PORT_PROFILE_NAME + 1];
    char portProfileDomainName[MAX_LEN_OF_PORT_PROFILE_DOMAIN_NAME + 1];
} NsmPhyIntfMsg_t;

typedef struct PortSecurityMsg_t_ {
	char ifName[20];
	int numAllowedMacs;
	unsigned int action;
	char mac[20];
	unsigned int cmdCode;
	unsigned int shutdownTime;
	int sticky;
	int vlanId;
} PortSecurityMsg_t;

typedef struct PortSecurityStickyMacRecord_t_ {
	char mac[20];
    char if_name[20];
    int vlan;
	unsigned char if_type;
} PortSecurityStickyMacRecord_t;

typedef struct PortSecurityStickyOutputMsg_t_ {
	unsigned int num_records;
	StickyMacRecordsType records_type;
	PortSecurityStickyMacRecord_t mac_records[1];
} PortSecurityStickyOutputMsg_t;

/* Show media interface */
typedef struct NsmPortMediaShowIntfSfp_OutputMsg_t_{
  char sfp_valid;
  char identifier; /* Type of serial transceiver */
  char connector;  /* Connector type */
  char transceiver[8]; /* Transceiver compatibility */
  char encoding;   /* Serial encoding algorithm */
  char baud_rate;  /* Nominal baud rate */
  char len9m_km;       /* Link length supported for 9m fibre (km) */
  char len9um;     /* Link length supported for 9um fibre */
  char len50um;    /* Link length supported for 50um fibre */
  char len625um;   /* Link length supported for 62.5um fibre */
  char lencu;      /* Link length supported for copper */
  char vendor[16]; /* Vendor name (ASCII) */
  unsigned char oui[3];     /* Vendor IEEE company ID */
  char partno[16]; /* Part number (ASCII) */
  char rev[4];     /* Revision level (ASCII) */
  char wavelength[2];  /* Laser wavelength */
  char options[2]; /* Optional signal implementation */
  char br_max;     /* Upper baud rate margin */
  char br_min;     /* Lower baud rate margin */
  char serialno[16];   /* Serial number (ASCII) */
  char date_code[8];   /* Vendor's manufactoring date code */
  char reason;       /* reason code when sfp_valid is 0 */
  char cc_ext;     /* Check code for extended ID fields */
  char cc_base;    /* Check code for base ID fields */
  int  smart;      /* Smart GBIC */
  int  temperature;    /* Module temperature (degrees C) */
  int  voltage;        /* Supply voltage (Volts) */
  int  opt_current;    /* Laser diode drive current (milliAmps) */
  int  tx_power;   /* Transmitted optical power (microWatts) */
  int  rx_power;   /* Received optical power (microWatts) */
} NsmPortMediaShowIntfSfp_OutputMsg_t;


typedef struct NsmPortMediaShowIntf_OutputMsg_t_{
    char ifName[IF_LONG_NAMESIZE + 1];
 	NsmPortMediaShowIntfSfp_OutputMsg_t sfp;
} NsmPortMediaShowIntf_OutputMsg_t;

/* Nsm show input Msg */
typedef struct NsmPortMediaShowIntf_InputMsg_t_ {

    char ifTypeName[MAX_STR];
    char ifName[MAX_STR];
	int slotId;
    unsigned int cmdCode;
} NsmPortMediaShowIntf_InputMsg_t;

/* Nsm debug input Msg */
typedef struct NsmDebug_InputMsg_t_ {
    unsigned char DebugType;
    unsigned int cmdCode;
} NsmDebug_InputMsg_t;

/* Nsm debug output Msg */
typedef struct NsmDebug_OutputMsg_t_ {
    char DebugOptions[MAX_DEBUG_TRACE];
} NsmDebug_OutputMsg_t;

typedef struct NsmPaginationResp_ {
    char  lastIndex[MAX_STR]; //index of the last interface returned from the daemon.
    char  isMore;//indicates whether more data exists in the daemon for a given interfaceType e.g: vlan.
    u_int32_t totalRecordsInBackend; //Total number of records of a given type(e.g: vlan) and not total records in a given page.
}NsmPaginationResp_t;

typedef struct NsmPaginationReq_ {
	int  no_of_records;
    char startIndex[MAX_STR];
    char endIndex[MAX_STR];
	InterfaceType ifType;
	int requestType;
}NsmPaginationReq_t;
/* Nsm show input Msg */
typedef struct NsmShowIntf_InputMsg_t_ {
    char ifName[MAX_STR];
    unsigned int cmdCode;
	NsmPaginationReq_t pageReq;
    unsigned int startId;
    unsigned int endId;
} NsmShowIntf_InputMsg_t;

/*Nsm Ecmp load balance input msg */
typedef struct NsmShowEcmp_InputMsg_t_ {

    unsigned int rbridgeId;
    unsigned int cmdCode;

}NsmShowEcmp_InputMsg_t;

/*Nsm Vlag load balance input msg */
typedef struct NsmShowVlag_InputMsg_t_ {

    unsigned int poId;
    unsigned int cmdCode;

}NsmShowVlag_InputMsg_t;

typedef struct LacpShow_InputMsg_t_ {

    char if_name[MAX_STR];
    unsigned int po_num;
    unsigned int cmdCode;
    unsigned int ifType;
    unsigned int startId;
    unsigned int endId;
    
} LacpShow_InputMsg_t;


typedef enum {
	LACP_DEBUG_CMDCODE_EVENT,
	LACP_DEBUG_CMDCODE_CLI,
	LACP_DEBUG_CMDCODE_TIMER,
	LACP_DEBUG_CMDCODE_SYNC,
	LACP_DEBUG_CMDCODE_HA,
	LACP_DEBUG_CMDCODE_ALL,
	LACP_DEBUG_CMDCODE_TRACE,
	LACP_DEBUG_CMDCODE_PDU
} LacpDebug_cmd_t;

typedef struct LacpDebug_InputMsg_t_ {
    char if_name[MAX_STR];
    unsigned int trace_level;
    unsigned int tx_or_rx;
    LacpDebug_cmd_t cmdCode;
    int negate;
} LacpDebug_InputMsg_t;

typedef struct NsmSowIntf_Stats_t_ {
    unsigned long long total_pkts_rcv;
    unsigned long long total_octets_rcv;
    unsigned long long uc_pkts_rcv;
    unsigned long long mc_pkts_rcv;
    unsigned long long brdc_pkts_rcv;
    unsigned long long pkts_64_octets;
    unsigned long long pkts_65_127_octets;
    unsigned long long pkts_128_255_octets;
    unsigned long long pkts_256_511_octets;
    unsigned long long pkts_512_1023_octets;
    unsigned long long pkts_1024_1518_octets;
    unsigned long long pkts_gt_1518_octets;
    unsigned long long undersize_pkts;
    
    unsigned long long jabber_pkts;
    unsigned long long fragments_pkts;
    unsigned long long port_in_overflow_discards;
   
    unsigned long long oversize_pkts;

    unsigned long long discard_pkts_rcv;

    unsigned long long total_pkts_sent;
    unsigned long long total_octets_sent;
    unsigned long long uc_pkts_sent;
    
    unsigned long long mc_pkts_sent;
    unsigned long long brdc_pkts_sent;
    unsigned long long port_out_overflow_frames;
    unsigned long long mac_transmit_err;
    unsigned long long discard_pkts_sent;
	
    unsigned long long pkt_rate_rcv;
    unsigned long long pkt_rate_sent;
    unsigned long long bit_rate_rcv;
    unsigned long long bit_rate_sent;
    unsigned long long pause_pkts_sent;
    unsigned long long pause_pkts_rcv;
    unsigned long long bad_crc;
    unsigned long long fcs_err;
    unsigned long long error_pkts_rcv;
    /*trill-port counters: priority 0 + priority 7 = control frames rx
	                   priority 7 = control frames tx
    */
    unsigned long long trill_port_tx_control_frames;
    unsigned long long trill_port_rx_control_frames;

	double line_rate_in;
	double line_rate_out;
	double line_inrate_percent;
	double line_outrate_percent;

} NsmStats_t;

typedef struct NsmIpAddr_t_ {
	unsigned int ip;
	unsigned int mask;
	unsigned int dest; // u.prefix
	unsigned int dest_family; // p.family
	unsigned char has_dest;
	unsigned char has_bcast;
	unsigned char has_p2p;
	unsigned char is_primary;
} NsmIpAddr_t;

/* Nsm show output Msg */
#define NSM_FEATURE_PRIORITY_TAG 0x1
#define NSM_FEATURE_RA_GUARD (1 << 1)

/*  NsmShowIntf_OutputMsg_t - m_proto_membership bit definitions */
#define UDLD_PROTOCOL_ENABLED      (1 << 0)

typedef struct NsmShowIntf_OutputMsg_t_ {
	
    unsigned int  id;
    unsigned int  idPos;
    char ifName[MAX_STR]; // also used by show ip int
    char vrfName[MAX_STR]; // also used by show ip int
	unsigned int swModeFlag;
	unsigned int swMode;
	unsigned int fcoePort;
	unsigned int defaultVlan;
	unsigned int inactiveVlan;
	unsigned int feature;
    char desc[MAX_STR];
    unsigned int inpkts;
    unsigned char m_if_is_up; // also used by show ip int
    unsigned int hw_type;
    u_int64_t m_proto_membership;
    unsigned char  m_if_is_logical_online; // also used by show ip int
	unsigned char  m_if_exception_set; // also used by show ip int
    unsigned char  m_blade_dis;
	char  vlan_type_str[MAX_STR];
    char  m_if_is_logical_online_info[MAX_STR];
	char  m_if_exception_str[MAX_STR];

    char m_nsm_if_type_str[MAX_STR];
    char m_hwAddr_str[MAX_STR];
    char m_curAddr_str[MAX_STR];

	char m_tracking_status[MAX_STR];
	char m_tracked_intf[MAX_STR];
    unsigned char m_Pluggable_media_present_flag;
    char m_pluggable_media_name[MAX_STR];
    unsigned int m_wavelength;

    unsigned int m_linespeed;
    char m_linespeed_str[MAX_STR];
    char m_confspeed_str[MAX_STR];
	char m_duplex_str[MAX_STR];
    char m_flowcontrol_rx_str[MAX_ON_OFF_STR];
    char m_flowcontrol_tx_str[MAX_ON_OFF_STR];
    unsigned int m_duplex;

	char m_ifLastChange[MAX_STR];
    char m_ifCtrsLastClr[MAX_STR];

    unsigned long long  m_ifIndex;
    unsigned int mtu; // also used by show ip int
    unsigned int ipmtu;//added to display ipmtu
	unsigned int arp_ageing_timeout; // also used by show ip int

	//Added for port-channel
    unsigned int poMinLinks;
    unsigned int poLoadBalanceMode;
	char m_allowspeed_str[MAX_STR];
	
    NsmStats_t stats;

	// Only for "show ip int"
	unsigned char has_ip;
	unsigned char is_ve;
	unsigned char has_proxy_arp;
	unsigned char has_anycast;
	unsigned char has_virtual;
    unsigned char has_isl;
	unsigned char has_label;
    unsigned char label[MAX_STR];
	unsigned char has_ld;

	unsigned int ipCount;
	NsmIpAddr_t IpAddr[NSM_INTERFACE_ADDRESS_MAX];
    unsigned int m_mediaRJ45;
    unsigned char port_mode;//port_mode(L2/L3/Unknown)
} NsmShowIntf_OutputMsg_t;

#define DCM_NSM_VLAN_MAX                 8191              /* Max VLANs.   */

#define DCM_NSM_VLAN_ALL                 (DCM_NSM_VLAN_MAX + 1)/* All VLANs.   */
#define DCM_NSM_VLAN_NONE                0                 /* No VLANs.    */
#define DCM_NSM_VLAN_DEFAULT_VID         1                 /* Default VID. */
 
/* VLAN bitmap manipulation macros. */
#define DCM_NSM_VLAN_BMP_WORD_WIDTH                32
#define DCM_NSM_VLAN_BMP_WORD_MAX                  \
        ((DCM_NSM_VLAN_MAX + DCM_NSM_VLAN_BMP_WORD_WIDTH) / DCM_NSM_VLAN_BMP_WORD_WIDTH)


enum dcm_nsm_vlan_state
  {
    DCM_NSM_VLAN_INVALID, // this state should never be sent by the dcm client.
    DCM_NSM_VLAN_DISABLED_NO_MEMBERS, //provisioned and has no member ports.
    DCM_NSM_VLAN_ACTIVE, //provisioned and is active.
    DCM_NSM_VLAN_DISABLED_MEMBERS_DOWN,//provisioned and all member ports are down.
    DCM_NSM_VLAN_UNPROVISIONED //configured, but unprovisioned.

  };

enum rpc_nsm_vlan_state
  {
    RPC_NSM_VLAN_INVALID = 1,
    RPC_NSM_VLAN_SUSPEND,
    RPC_NSM_VLAN_ACTIVE,
    RPC_NSM_VLAN_DISABLED_MEMBERS_DOWN
  };

enum rpc_nsm_vlan_port_mode
  {
    RPC_NSM_VLAN_PORT_MODE_ACCESS = 1,
    RPC_NSM_VLAN_PORT_MODE_TRUNK,
    RPC_NSM_VLAN_PORT_MODE_HYBRID
  };

enum rpc_nsm_vlan_type
  {
    RPC_NSM_VLAN_FCOE = 1,
    RPC_NSM_VLAN_STATIC,
    RPC_NSM_VLAN_DYNAMIC,
    RPC_NSM_VLAN_CVLAN,
    RPC_NSM_VLAN_SVLAN,
    RPC_NSM_SVLAN_P2P,
    RPC_NSM_VLAN_APPM,
    RPC_NSM_VLAN_DEFAULT
  };

/* VLAN port mode. */
enum dcm_nsm_vlan_port_mode
  {
    DCM_NSM_VLAN_PORT_MODE_INVALID,
    DCM_NSM_VLAN_PORT_MODE_ACCESS,
    DCM_NSM_VLAN_PORT_MODE_HYBRID,
    DCM_NSM_VLAN_PORT_MODE_TRUNK,
    DCM_NSM_VLAN_PORT_MODE_PROFILE,
    DCM_NSM_VLAN_PORT_MODE_CE,
    DCM_NSM_VLAN_PORT_MODE_CN,
    DCM_NSM_VLAN_PORT_MODE_PN
  };
struct dcm_nsm_vlan_bmp
{
  u_int32_t bitmap[DCM_NSM_VLAN_BMP_WORD_MAX];
};
#define DCM_NSM_VLAN_BMP_INIT(bmp)                                             \
   do {                                                                    \
       pal_mem_set ((bmp).bitmap, 0, sizeof ((bmp).bitmap));               \
   } while (0)
 
#define DCM_NSM_VLAN_BMP_SET(bmp, vid)                                         \
   do {                                                                    \
        int _word = (vid) / DCM_NSM_VLAN_BMP_WORD_WIDTH;                       \
        (bmp).bitmap[_word] |= (1U << ((vid) % DCM_NSM_VLAN_BMP_WORD_WIDTH));  \
   } while (0)

#define DCM_NSM_VLAN_BMP_UNSET(bmp, vid)                                       \
   do {                                                                    \
        int _word = (vid) / DCM_NSM_VLAN_BMP_WORD_WIDTH;                       \
        (bmp).bitmap[_word] &= ~(1U <<((vid) % DCM_NSM_VLAN_BMP_WORD_WIDTH));  \
   } while (0)																																	

#define DCM_NSM_VLAN_BMP_IS_MEMBER(bmp, vid)                                   \
  ((bmp).bitmap[(vid) / DCM_NSM_VLAN_BMP_WORD_WIDTH] & (1U << ((vid) % DCM_NSM_VLAN_BMP_WORD_WIDTH)))


#define DCM_NSM_VLAN_BMP_CLEAR(bmp)         DCM_NSM_VLAN_BMP_INIT(bmp)

#define DCM_NSM_VLAN_BMP_OR(bmp1,bmp2,bmp3)                                    \
    do {                                                                   \
         int _w;                                                           \
         for (_w = 0; _w < DCM_NSM_VLAN_BMP_WORD_MAX; _w++)                    \
            (bmp1).bitmap[_w] =  ((bmp2).bitmap[_w] | (bmp3).bitmap[_w]);  \
    } while (0)

#define DCM_NSM_VLAN_BMP_CMP(bmp1,bmp2)                                        \
   pal_mem_cmp ((bmp1)->bitmap, (bmp2)->bitmap, DCM_NSM_VLAN_BMP_WORD_MAX)     \

#define NSM_VLAN_WYSEREA_MIN             2
#define NSM_VLAN_WYSEREA_MAX             DCM_NSM_VLAN_MAX


/*Below enum corresponds to different types of vlan classifications. */
typedef enum {
    NOCLASSIFICATION,
    CTAG,
    MAC,
    VNI
} gvlan_classification_type_t;

typedef struct mac_address_t_ {
    char macAddress[MAX_LENGTH_MAC];
} mac_address_t;

typedef struct ctag_t_ {
    u_int32_t ctag;
} ctag_t;

typedef struct vni_t_ {
    u_int32_t vni;
} vni_t;

/* The below union corresponds to a vlan classification value */
typedef union gvlan_classification_value_ {
    mac_address_t mac;
    ctag_t ctag;
    vni_t  vni;
} gvlan_classification_value_t;

/* The below struct corresponds to a port record and its vlan classifications. */
typedef struct showVlanBrPortRecode {
	char ifName[64 + 2];
	 struct dcm_nsm_vlan_bmp vlanMemBM;
	char mode;
    gvlan_classification_type_t classificationType;
    u_int32_t numberOfVlanClassifications;
    gvlan_classification_value_t classificationRecord[1];
} showVlanBrPortRecord_t;


#define DCM_NSM_SHOWVLANBREIF_REC_TYPE_PORT		1
#define DCM_NSM_SHOWVLANBREIF_REC_TYPE_VLAN		2

/*The below struct corresponds to a vlan record and its member ports */
typedef struct showVlanBrVlanRecord {

#define DCM_NSM_VLAN_NAME_MAX   16
	/* VLAN name. */
	char vlanName[DCM_NSM_VLAN_NAME_MAX + 2];
    u_int32_t vid;

#define DCM_NSM_VLAN_STATIC     (1 << 0)
#define DCM_NSM_VLAN_DYNAMIC    (1 << 1)
#define DCM_NSM_VLAN_CVLAN      (1 << 2)
#define DCM_NSM_VLAN_SVLAN      (1 << 3)
#define DCM_NSM_SVLAN_P2P       (1 << 4)
#define DCM_NSM_VLAN_APPM           (1 << 5)
//#ifdef HAVE_FABOS
#define DCM_NSM_VLAN_FCOE       (1 << 6)
#define DCM_NSM_VLAN_DEFAULT    (1 << 7)
//#endif
	char type;
	char state;
	int rspanEnabled; 
    u_int32_t numberOfVlanMemberPorts;
    showVlanBrPortRecord_t portRecords[1];

} showVlanBrVlanRecord_t;

/*The below struct corresponds to the output buffer returned from the back end daemon.*/
typedef struct NsmShowVlan_OutputMsg {
    u_int32_t bufferSize;
    u_int32_t num_records;
	unsigned int record_type;
	unsigned int rc;
	NsmPaginationResp_t pageResp;
    showVlanBrVlanRecord_t vlan_rec[1];
} NsmShowVlanBr_OutputMsg_t;

/* End of Nsm Show Vlan Brief */

typedef struct showVlanPVlanPortRec_ {
	char if_name[MAX_STR];
	unsigned int mode;
    gvlan_classification_type_t classificationType;
    u_int32_t numberOfClassifications;
    gvlan_classification_value_t classificationRecord[1];//This struct will be repeated "numberOfClassifications" times.
} showVlanPVlanPortRec_t;

typedef struct showVlanPVlanVlanRec_ {
    unsigned int priVlanId;
    unsigned int secVlanId;
    unsigned int numPorts;
    unsigned int pVlanType;
    showVlanPVlanPortRec_t portrecord[1];//This struct will be repeated "numPorts" times.
} showVlanPVlanVlanRec_t;

typedef struct NsmShowVlanPVlanOutputMsg_ {
    u_int32_t bufferSize; //size of the entire buffer returned from the back end to the plugin.
	unsigned int numPVlans;
	unsigned char recordType;
    showVlanPVlanVlanRec_t vlanrecord[1]; //This struct will be repeated "numPVlans" times.
} NsmShowVlanPVlanOutputMsg_t;
/* "show vlan private-vlan" commands end*/

/* Lacp show output Msg */
typedef struct LacpShow_OutputMsg_t_ {

    char if_name[MAX_STR];

} LacpShow_OutputMsg_t;


/* Lacp show counter output Msg */
typedef struct lacp_show_counter_output_msg_ {

    unsigned int lag_id;
    unsigned int agg_ix;
    char if_name[MAX_STR];
    unsigned int lacpdu_sent_count;
    unsigned int lacpdu_recv_count;
    unsigned int mpdu_sent_count;
    unsigned int mpdu_recv_count;
    unsigned int pckt_sent_err_count;
    unsigned int pckt_recv_err_count;

} lacp_show_counter_output_msg_t;

/* Lacp show sysid output Msg */
typedef struct lacp_show_sysid_output_msg_ {

	unsigned int po_num;
    unsigned int sys_prio;
    unsigned int sid_master_domain_id;
    unsigned char sys_id[6];

} lacp_show_sysid_output_msg_t;

/* Lacp show port port-channel output Msg */
typedef struct lacp_show_portpo_output_msg_ {

    unsigned char name[16];
	unsigned long long actor_port_number;
    unsigned int sys_priority;
    uint16_t     sid_mapped_id;
    unsigned char sys_id[6];
	unsigned int partner_oper_prio;
    unsigned char partner_oper_sys[6];
    unsigned int actor_port_priority;
    unsigned int actor_admin_port_key;
    unsigned int actor_oper_port_key;
    unsigned char rcv_state_str[16];
    unsigned char periodic_state_str[16];
    unsigned char mux_state_str[32];
    unsigned char admin_state[8];
    unsigned char oper_state[8];
    unsigned char partner_oper_state[8];
    unsigned int partner_oper_port_number;
    unsigned int brcd_trunk_type;
    unsigned int actor_chip_number;
    unsigned int actor_max_deskew;
    unsigned int partner_chip_number;
    unsigned int partner_max_deskew;
    unsigned char actor_brcd_state[5];
    unsigned char partner_brcd_state[5];
	unsigned char selected;
	unsigned char defaulted_state_action;

} lacp_show_portpo_output_msg_t;

/* Lacp show port-channel details output Msg */
typedef struct lacp_po_link_details_output_msg_ {
    unsigned char name[16];
	unsigned long long actor_port_number;
	unsigned int sync;
	unsigned int is_primary_ifindex;
	unsigned int mux_machine_state;
	unsigned int selected;
	
} lacp_po_link_details_output_msg_t;

/* Lacp show port-channel details output Msg */
typedef struct lacp_show_podetails_output_msg_ {

    unsigned int po_num;
    /* A flag to denote the type of Aggregator Association STATIC or LACP*/
    u_char agg_config_type;
    /* Type variable indicating whether the trunk is standard, 
     * brocade or hybrid 
     */
    u_char config_trunk_type;
	u_char ignore_split;
	u_char usingPartnerAdminInfo;
    unsigned int sys_priority;
    unsigned int sid_mapped_id;
    unsigned char sys_id[6];
    unsigned short rbridge_count;
    rbridge_ports_t rbridge_member[16];
    unsigned int admin_key;
    unsigned int oper_key;
    unsigned int recv_link_count;
    unsigned int trans_link_count;
    unsigned int individual_agg;
    unsigned int ready_agg;
	unsigned int partner_sys_prio;
    unsigned char partner_sys[6];
    unsigned int partner_oper_key;
	unsigned int num_links;
	lacp_po_link_details_output_msg_t links[16];

} lacp_show_podetails_output_msg_t;

/* Lacp show debug output Msg */
typedef struct lacp_show_debug_output_msg_ {	
	u_int32_t debug_enabled;
    char rx_enabledports[LACP_MAX_LINKS][IF_LONG_NAMESIZE];
	u_int32_t rx_numports;
	char tx_enabledports[LACP_MAX_LINKS][IF_LONG_NAMESIZE];
	u_int32_t tx_numports;
} lacp_show_debug_output_msg_t;

typedef struct lacp_show_msg_ {
	unsigned int num_records;
    unsigned int record_type;
    unsigned char more_rec_flag;
    union {
    	lacp_show_counter_output_msg_t counters[1];
    	lacp_show_sysid_output_msg_t sysids[1];
    	lacp_show_portpo_output_msg_t portpos[1];
    	lacp_show_podetails_output_msg_t podetails[1];
        lacp_show_debug_output_msg_t  denabled;
	} records;
} lacp_show_msg_t;

/* Collect Nsm interface Msg */

typedef struct nsm_req_msg_t_ {
    unsigned int cmdCode;
} nsm_req_msg_t;

typedef enum
{
    EDGE,
    ISL
} dcm_islState;

typedef enum
{
    SHUTDOWN,
    NO_SHUTDOWN
} dcm_status;

typedef struct nsm_intf_msg {
    char ifName[MAX_STR];
    char desc[MAX_STR];
    unsigned long long  m_ifIndex;
    unsigned int mtu;
    unsigned int isl_capable;
    unsigned short int islState;
    unsigned short int status;
} nsm_intf_msg_t;

typedef struct nsm_intf_all_msg_ {
	unsigned int num_records;
    unsigned int record_type;
  	nsm_intf_msg_t intfdetails[1];
} nsm_intf_all_msg_t;

typedef enum
{
	DCM_HAL_TRUNK_TYPE_STANDARD = 1,
	DCM_HAL_TRUNK_TYPE_BRCD		= 2,
	DCM_HAL_TRUNK_TYPE_HYBRID	= 3,
	DCM_HAL_TRUNK_TYPE_INTERNAL = 4,
	DCM_HAL_TRUNK_TYPE_ISL      = 5
} dcm_hsl_trunk_type_t;

/* NSM get all port-channel ids output Msg */
typedef struct nsm_show_poid_output_msg_ {
    unsigned int po_num;
} nsm_show_poid_output_msg_t;

/* Nsm show port-channel details output Msg */
typedef struct nsm_po_link_details_output_msg_ {
    unsigned char member_name[MAX_INTERFACE_NAMSIZ + 1];
	unsigned int is_primary_ifindex;
} nsm_po_link_details_output_msg_t;

typedef struct nsm_portchannel_show_info_ {
    unsigned int po_num;
	/* A flag to denote the type of Aggregator Association STATIC or LACP*/
	u_char agg_config_type;
  	/* Type variable indicating whether the trunk is standard, 
	 * brocade or hybrid 
	 */
	u_char config_trunk_type; 
	int num_links; 
    nsm_po_link_details_output_msg_t links[16];
    int rbridge_count;
    rbridge_ports_t rbridge_member[16];
} nsm_portchannel_show_info_t;

typedef struct nsm_portchannel_show_msg_ {
	unsigned int num_records;
    unsigned int record_type;
    unsigned char more_rec_flag;
  	nsm_portchannel_show_info_t podetails[1];
} nsm_portchannel_show_msg_t;

/* show port-channel load-balance */
typedef struct nsm_po_loadbalance_info_ {
	unsigned int po_num;
	u_char agg_config_type;
	int psc;/* Variable that holds loadbalancing mode. default 7 */ 
} nsm_po_loadbalance_info_t; 

typedef struct nsm_po_loadbalance_all_msg_ {
	unsigned int num_records;
	unsigned int record_type;
	nsm_po_loadbalance_info_t podetails[1];
} nsm_po_loadbalance_all_msg_t;

/* show fabric ecmp load-balance */
  typedef struct nsm_ecmp_loadbalance_info_ {
      int rbridgeId;
      int psc;
      int hashSwap;/* Variable that holds loadbalancing mode. default 7 */
  } nsm_ecmp_loadbalance_info_t;

  typedef struct nsm_ecmp_loadbalance_all_msg_ {
      unsigned int num_records;
      unsigned int record_type;
      nsm_ecmp_loadbalance_info_t ecmpdetails[1];
  } nsm_ecmp_loadbalance_all_msg_t;

/* show fabric vlag load-balance */
  typedef struct nsm_vlag_loadbalance_info_ {
      int poId;
      int psc;/* Variable that holds loadbalancing mode. default 7 */
  } nsm_vlag_loadbalance_info_t;

  typedef struct nsm_vlag_loadbalance_all_msg_ {
      unsigned int num_records;
      unsigned int record_type;
      nsm_vlag_loadbalance_info_t vlagdetails[1];
  } nsm_vlag_loadbalance_all_msg_t;

  typedef struct nsm_show_all_portchannel_ids_msg_ {
	  unsigned int num_records;
	  unsigned int record_type;
	  nsm_show_poid_output_msg_t poids[1];
  } nsm_show_all_portchannel_ids_msg_t;

typedef enum
{
    NSM_FCOE_CMD_INVALID,
    NSM_FCOE_CMD_CREATE_VLAN,
    NSM_FCOE_CMD_DELETE_VLAN,
    NSM_FCOE_CMD_UPDATE_VLAN
} NsmFcoeVlanCmdCode;
                                                

typedef struct NsmFcoeVlanMsg_ {
    unsigned int cmd_code;
    unsigned int old_vlan;
    unsigned int new_vlan;
    unsigned char vlanName[MAX_STR];
    unsigned int vlanIfIndex;
    unsigned char desc[MAX_STR];
} NsmFcoeVlanMsg_t;

typedef struct NsmReservedVlanMsg_ {
    unsigned int cmdcode;
    unsigned int start;
    unsigned int end;
} NsmReservedVlanMsg_t;

typedef struct NsmFcoeportMsg_ {
    uint32_t    cmd_code;
	unsigned int ifindex;
    uint8_t     if_name[MAX_STR];
    uint8_t     map_name[MAX_STR];
    uint32_t    num_vlans;
    uint32_t    fcoe_vlan_list[8];
    uint8_t     cee_map_name[MAX_STR];
	uint32_t    ifType;
}NsmFcoeportMsg_t;

typedef struct nsm_clear_input_msg {
    uint32_t    cmd_code;
    char     if_name[MAX_STR];
    uint32_t    vlan_id;
    uint32_t    slot_id;
    uint32_t    po_id;
}nsm_clear_input_msg_t;

#define NSM_DCM_CLEAR_RESULT_OK 0x0
#define NSM_DCM_CLEAR_RESULT_FAIL 0x1

typedef struct nsm_clear_output_msg {
    uint32_t    cmd_code;
    uint32_t    result;
    uint32_t    result1;
    uint32_t    result2;
}nsm_clear_output_msg_t;

typedef enum
{
	SERVICE_READY = 0,
	LINECARD_READY,
	CHASSIS_READY,
    LINECARD_ONLINE,
	POSTBOOT_TRIGGER,
	INTERFACE_STATE_REPLAY
} NsmDcmEvent_t;

typedef union lacp_mgmt_buffer_num_ {
    struct {
        uint16_t   mapped_id;
        uint16_t   tag;
    } buff;
    uint32_t   num;
} lacp_mgmt_buffer_num_t;

static inline uint16_t
lacp_get_buffer_mapped_id (uint32_t  buff_num)
{
    return (((lacp_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

typedef union dot1x_mgmt_buffer_num_ {
    struct {
        uint16_t   mapped_id;
        uint16_t   tag;
    } buff;
    uint32_t   num;
} dot1x_mgmt_buffer_num_t;

static inline uint16_t
dot1x_get_buffer_mapped_id (uint32_t  buff_num)
{
    return (((dot1x_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

typedef union nsm_mgmt_buffer_num_ {
    struct {
        uint16_t   mapped_id;
        uint16_t   tag;
    } buff;
    uint32_t   num;
} nsm_mgmt_buffer_num_t;

static inline uint16_t
nsm_get_buffer_mapped_id (uint32_t  buff_num)
{
    return (((nsm_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

static inline uint16_t
nsm_get_buffer_tag (uint32_t  buff_num)
{
    return (((nsm_mgmt_buffer_num_t*) &buff_num)->buff.tag);
}

typedef struct NsmVlagExmRebalanceMsg_t_ {
    unsigned int port_channel;
} NsmVlagExmRebalanceMsg_t;

typedef enum {
	SHOW_SFP = 0,
	SHOW_SFP_LINECARD_ID,
	SHOW_SFP_LINECARD_ID_PORT_ID
} ShowSfpCmdCode;

typedef struct SfpShowInputMsg_ {
	unsigned int cmdCode;
	unsigned int linecard;
	unsigned int port;
} SfpShowInputMsg_t;

typedef struct SfpShowOutputRecord_ {
	unsigned int linecard;
	unsigned int port;
	char sfpType[32];
	char breakoutType[32];
	char speed[32];
} SfpShowOutputRecord_t;

typedef struct SfpShowOutputMsg_ {
	unsigned int num_records;
	unsigned int record_type;
	SfpShowOutputRecord_t sfp[1];
} SfpShowOutputMsg_t;

typedef struct vlan_provision_status_record_ {
    uint32_t vlan_id;
	vlan_provision_status provision_status;
} vlan_provision_status_record_t;

typedef struct nsm_vlan_provision_response_ {
	unsigned int number_of_records;
	vlan_provision_status_record_t status[1];
} nsm_vlan_provision_response_t;

typedef struct MacAddressVlanClassificationMsg_ {
	unsigned int vlanId;
	unsigned char mac[MAX_LENGTH_MAC];
} MacAddressVlanClassificationMsg_t;

typedef struct MacGroupVlanClassificationMsg_ {
	unsigned int vlanId;
	unsigned int macGroupId;
} MacGroupVlanClassificationMsg_t;

typedef struct CtagVlanClassificationMsg_ {
	unsigned int vlanId;
	unsigned int ctagId;
} CtagVlanClassificationMsg_t;

typedef union SwitchportVlanClassificationUnion_ {
	MacAddressVlanClassificationMsg_t macAddressRec;
	MacGroupVlanClassificationMsg_t macGroupRec;
	CtagVlanClassificationMsg_t ctagRec;
} SwitchportVlanClassificationUnion_t;

typedef struct SwitchportVlanClassificationRec_ {
    unsigned int cmdCode;
    unsigned int ifType;
    char ifName[IF_LONG_NAMESIZE];
    SwitchportVlanClassificationUnion_t payload;
} SwitchportVlanClassificationRec_t;

typedef struct SwitchportVlanClassificationMsg_ {
    unsigned int numRecords;
    SwitchportVlanClassificationRec_t rec[1];
} SwitchportVlanClassificationMsg_t;

typedef struct MacGroupRecord_ {
    unsigned char address[MAC_ADDR_LENGTH_IN_BYTES];
    unsigned char mask[MAC_ADDR_LENGTH_IN_BYTES];
} MacGroupRecord_t;

typedef struct MacGroupInputMsg_ {
    unsigned int cmdCode;
    unsigned int macGroupId;
    unsigned int numMacRecords;
    MacGroupRecord_t macGroupRecord[1];
} MacGroupInputMsg_t;

typedef enum {
    MAC_GROUP_CREATE,
    MAC_GROUP_DELETE,
    MAC_GROUP_SET_ENTRY,
    MAC_GROUP_DEL_ENTRY
} MacGroupCmdCode;

typedef enum {
	SW_VLAN_CLASSIFICATION_SET_MAC_ADDR,
	SW_VLAN_CLASSIFICATION_DEL_MAC_ADDR,
	SW_VLAN_CLASSIFICATION_SET_MAC_GROUP,
	SW_VLAN_CLASSIFICATION_DEL_MAC_GROUP,
	SW_VLAN_CLASSIFICATION_SET_CTAG,
	SW_VLAN_CLASSIFICATION_DEL_CTAG,
	SW_VLAN_CLASSIFICATION_SET_PVLAN_CTAG,
	SW_VLAN_CLASSIFICATION_DEL_PVLAN_CTAG,
	SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_ADDR,
	SW_VLAN_CLASSIFICATION_DEL_RSPAN_MAC_ADDR,
	SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_GROUP,
	SW_VLAN_CLASSIFICATION_DEL_RSPAN_MAC_GROUP,
	SW_VLAN_CLASSIFICATION_SET_RSPAN_CTAG,
	SW_VLAN_CLASSIFICATION_DEL_RSPAN_CTAG,
	SW_VLAN_CLASSIFICATION_SET_NATIVE_CTAG
} SwVlanClassificationCmdCode;

/* GVLAN ENABLE STAGE */
typedef enum {
	GVLAN_ENABLE_VLD_0,				/* This cmdCode is used for validations and is used by NSM plugin only. */
	GVLAN_ENABLE_STG_1,				/* First Stage for the GVLAN MODE Transition */
	GVLAN_ENABLE_STG_2,
	GVLAN_ENABLE_STG_3,
	GVLAN_ENABLE_STG_4,
	GVLAN_ENABLE_STG_5				/* This is the last cmdCode and should be for updating firmware upgrade/downgrade purposes by NSM plugin only */
} gvlan_enable_stage_t;

/* Message to modify the FABRIC GVLAN Mode */
typedef struct gvlan_enable_input_msg_
{
	unsigned char enable;			/* GVLAN Enable/Disable. This member should be used as a flag. */
	unsigned char undo;				/* Undo the previous Stage. This member should be used as a flag.  */
//	fgl_mode mode;					/* FGL Encapsulation Type when GVLAN is Enabled, Stagging applied to this as well */
	gvlan_enable_stage_t stage;		/* Stage ID from vcs_fabric_mode_stage_t */
} gvlan_enable_input_msg_t;

#endif

