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

#ifndef __OSPF_MESSAGE_DEF_H__
#define __OSPF_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/nsmVrfIntf.h"
#include <string.h>
//#include "/vobs/projects/springboard/fabos/src/dce/l3/include/ospfenum.h"
#include "ospfenum.h"

// Some default values
#define OSPF_DEFAULT_HELLO_INTERVAL 10
#define OSPF_DEFAULT_ROUTER_DEAD_INTERVAL 40
#define OSPF_DEFAULT_RETRANSMIT_INTERVAL 5
#define OSPF_DEFAULT_TRANSMIT_DELAY 1
#define OSPF_DEFAULT_COST 1
#define OSPF_DEFAULT_PRIORITY 1
#define OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME 300
#define OSPF_DEFAULT_MD5_NEW_KEY_ACTIVATION_WAIT_TIME 300
#define OSPF_MAX_AREA_COUNT     64
#define OSPF_DEFAULT_MAX_METRIC_EXT_METRIC  16711680 
#define OSPF_DEFAULT_MAX_METRIC_SUMM_METRIC 16711680 

#define OSPF_TIMER_SPF_MAX      60000
#define OSPF_TIMER_SPF_INVALID  OSPF_TIMER_SPF_MAX+1 

#define OSPF_DCM_IF_NAME_LEN	32
#define OSPF_STR_64_LEN	64

// Total # keywords any OSPF CLI can have
#define OSPF_CLI_MAX_ARGS          20

// Max characters in output of any show CLI
#define OSPF_SHOW_MAX_OUTPUT_SIZE  20000

// Max characters in the error msg for any OSPF CLI
#define OSPF_ERROR_MAX_OUTPUT_SIZE 200

// Size of a string of an ipv4 address
#define OSPF_IPV4_ADDR_STR_SIZE 17

#define OSPF_AUTH_KEY_SIZE		26
#define OSPF_AUTH_KEY_VALID_SIZE_8 8
#define OSPF_AUTH_KEY_VALID_SIZE 13
#define OSPF_MD5_AUTH_KEY_VALID_SIZE_16 16
#define OSPF_MD5_AUTH_KEY_VALID_SIZE 25

#define OSPF_CONFIG_DEF_INFO_ORIG_ALWAYS		(1<<0)
#define OSPF_CONFIG_DEF_INFO_ORIG_METRIC		(1<<1)
#define OSPF_CONFIG_DEF_INFO_ORIG_METRIC_TYPE	(1<<2)
#define OSPF_CONFIG_DEF_INFO_ORIG_ROUTE_MAP		(1<<3)

#define OSPF_DCMD_DEFAULT_LSA_GROUP_PACING_INTERVAL 240
#define SHOW_OSPF_HANDLE_REQ_TAG 4
#define SHOW_OSPF_HANDLE_RESP_TAG 5

#define OSPF_SET_MAX_METRIC_STARTUP_TIME         1
#define OSPF_SET_MAX_METRIC_STARTUP_WAIT_BGP     2
#define OSPF_SET_MAX_METRIC_LINK_TYPE            4
#define OSPF_SET_MAX_METRIC_EXTERNAL_LSA         8
#define OSPF_SET_MAX_METRIC_SUMMARY_LSA          16

#define OSPF_MAX_METRIC_LINK_TYPE_PTP           1
#define OSPF_MAX_METRIC_LINK_TYPE_STUB          2
#define OSPF_MAX_METRIC_LINK_TYPE_TRANSIT       4
#define OSPF_MAX_METRIC_LINK_TYPE_ALL           8


typedef enum {
    OSPF_MEMORY_ALLOCATION_FAILED = 2010,
    OSPF_DCM_API_MSG_ERROR,
    OSPF_DCM_LINUX_FILEDOESNOT_EXIST,
    OSPF_L3_COMMANDS_NOT_ALLOWED_ON_L2_INTERFACE,
    OSPF_L3_COMMANDS_NOT_ALLOWED_ON_PO_INTERFACE,
    OSPF_CLI_ERR_DISTRIBUTE_LIST_RMAP_DEFINED,
    OSPF_CLI_ERR_MAX_OSPF_IF,
    OSPF_VRF_DOES_NOT_EXIST,
    OSPF_ERR_REDIS_BGP_NON_DEF_VRF,
	OSPF_ERR_VRF_LITE_DEF_VRF,
    OSPF_UNKNOWN_INTERNAL_ERROR
} ospf_error_code_t;

typedef enum
{   
	OSPF_CREATE = 0,
    OSPF_REMOVE,
    OSPF_SET_ELEM
}OSPFOpCode;


typedef enum
{   
	OSPF_INTF_AREA = 0,
	OSPF_INTF_HELLO,
	OSPF_INTF_DEAD,
	OSPF_INTF_RETRANSMIT,
	OSPF_INTF_TRANSDELAY,
	OSPF_INTF_COST,
	OSPF_INTF_PRIORITY,
	OSPF_INTF_AUTH_CHANGE_WAIT_TIME,
	OSPF_INTF_AUTH_KEY,
	OSPF_INTF_MD5_KEY_ACTV_WAIT_TIME,
	OSPF_INTF_MD5_AUTH_KEY,
	OSPF_INTF_FILTER_ALL,
	OSPF_INTF_FILTER_EXT,
	OSPF_INTF_FILTER_SUMM_EXT,
	OSPF_INTF_MTU_IGNORE,
	OSPF_INTF_NETWORK,
	OSPF_INTF_ACTIVE,
	OSPF_INTF_PASSIVE,
	OSPF_INTF_DEL,
	OSPF_INTF_CONFIG_REPLAY
}OSPFIntfCmdCode;

typedef enum
{
	OSPF_NONE = 0,
	OSPF_SIMPLE,
	OSPF_MD5
}OSPFAuthType;

typedef enum
{
	OSPF_INTF_FILTER_NONE = 0,
	OSPF_INTF_ALL_EXTERNAL,      // ip ospf database-filter all-external *
	OSPF_INTF_ALL_SUMM_EXTERNAL, // ip ospf database-filter all-summary-external *
	OSPF_INTF_ALL                // ip ospf database-filter all-out
}OSPFIntfDatabaseFilterCat;

typedef enum
{
	OSPF_INTF_ALLOW_INVALID = 0,
	OSPF_INTF_ALLOW_DEFAULT,        // ip ospf database-filter * allow-default-out
	OSPF_INTF_ALLOW_DEFAULT_TYPE_4, // ip ospf database-filter * allow-default-and-type4-out
	OSPF_INTF_ALLOW_OUT             // ip ospf database-filter * out
}OSPFIntfDatabaseFilterAllow;

typedef enum
{
	OSPF_INTF_NETWORK_NONE = 0,
	OSPF_INTF_BROADCAST,
	OSPF_INTF_NON_BROADCAST,
	OSPF_INTF_POINT_TO_POINT
}OSPFIntfNetworkType;

typedef enum
{
    OSPF_PERMIT_DENY_OPTION_NONE = 0,
    OSPF_PERMIT_DENY_OPTION_PERMIT,
    OSPF_PERMIT_DENY_OPTION_DENY
}OSPFPermitDenyOption;

typedef enum
{
    OSPF_MATCH_METRIC_TYPE_NONE = 0,
    OSPF_MATCH_METRIC_TYPE_MATCH,
    OSPF_MATCH_METRIC_TYPE_SET
}OSPFMatchMetricType;

typedef enum
{
    OSPF_METRIC_NONE = 0,
    OSPF_TYPE1,
    OSPF_TYPE2
}OSPFDefInfoOrigmetricType;

typedef enum
{
    OSPF_ROUTE_OPTION_NONE = 0,
    OSPF_ROUTE_OPTION_ALL,
    OSPF_ROUTE_OPTION_CONNECTED,
    OSPF_ROUTE_OPTION_BGP,
    OSPF_ROUTE_OPTION_STATIC
}OSPFRouteOption;

/* Message Structs */
typedef struct routerOSPFConfigMsg_t_ {
    unsigned char opCode;
    unsigned char cmdCode;
	unsigned int  m_referenceBandwidth;
	unsigned int  m_refBwUseActivePorts;
	unsigned int  m_databaseOverflowInterval;
	unsigned char m_defInfoOrigAlways;
	unsigned char m_defaultPassiveInterface;
	unsigned int  m_defInfoOrigMetric;
	OSPFDefInfoOrigmetricType  m_defInfoOrigMetricType;
	char  m_defInfoOrigRouteMap[OSPF_STR_64_LEN+1];
	unsigned int  m_defaultMetric;
	unsigned int  m_distanceExternal;
	unsigned int  m_distanceInterArea;
	unsigned int  m_distanceIntraArea;
	char  m_distListRouteMap[OSPF_STR_64_LEN+1];
	char   m_domainId[OSPF_IPV4_ADDR_STR_SIZE];
	unsigned int  m_domainTag;
	unsigned int  m_extLSDBLimit;
	unsigned int  m_gracefulRestartNotifyTime;
	unsigned int  m_gracefulRestartRestartTime;
	unsigned int  m_maxMetricAllLSA;
	char   m_neighborAddress[OSPF_IPV4_ADDR_STR_SIZE];
	OSPFDefInfoOrigmetricType m_extMetricType;
	unsigned int m_isNSSATranslator;
	char   m_redistributeConnectedRouteMap[OSPF_STR_64_LEN+1];
	char   m_redistributeBgpRouteMap[OSPF_STR_64_LEN+1];
	char   m_redistributeStaticRouteMap[OSPF_STR_64_LEN+1];
	unsigned int m_rfc1583Compatibility;
	unsigned int m_rfc1587Compatibility;
	unsigned int m_timerLsaGroupPacingInterval;
	unsigned int m_timerInitDelay;
    unsigned int m_timerHoldTime;
	unsigned int m_timerMaxHoldTime;
	char   m_summaryAddress[OSPF_IPV4_ADDR_STR_SIZE];
	char   m_summaryMask[OSPF_IPV4_ADDR_STR_SIZE];
	char   m_vrf[NSM_INTF_NAME_SIZE];
	unsigned char m_cmdType;
	unsigned char m_maxMetricLinkType;
	unsigned  int m_maxMetricOnStartupTime;
	unsigned int  m_extMaxMetric;
	unsigned int  m_summMaxMetric;
	unsigned int  m_extMaxMetricLsa;
	unsigned int  m_summMaxMetricLsa;
	//unsigned int  m_routerLsa;
	unsigned int  m_isAllVrfMaxMetricSet;
	char config_replay[1];
} routerOSPFConfigMsg_t;

typedef struct OSPFRouterConfigReplayMsg_ {
	unsigned long long m_replayCmdCodeBitmap;
	unsigned int num_neighbors;
	unsigned int num_summary_addresses;

	char buf[1];
} OSPFRouterConfigReplayMsg_t;

typedef struct OSPFPermitDenyRedistributeConfigMsg_t_ {
    unsigned char opCode;
    unsigned char cmdCode;
	unsigned int  m_index;
	OSPFPermitDenyOption m_permitDenyOption;
	OSPFRouteOption m_routeOption;
	char   m_subNet[OSPF_IPV4_ADDR_STR_SIZE];
	char   m_mask[OSPF_IPV4_ADDR_STR_SIZE];
	unsigned int m_matchMetric;
	unsigned int m_setMetric;
} OSPFPermitDenyRedistributeConfigMsg_t;

#define CU_MD5_PASSWORD_SIZE            16
#define CU_MD5_PASSWORD_ENCRYPTED_SIZE  25
typedef struct interfaceOSPFConfigMsg_t_ {
    unsigned char opCode;
    unsigned char cmdCode;
            char  intfName[OSPF_DCM_IF_NAME_LEN];
    		unsigned int  intfType;
            char  areaId[OSPF_IPV4_ADDR_STR_SIZE];
            int  HelloInterval;
            int  RouterDeadInterval;
            int  RetransmitInterval;
            int  TransDelay;
            int  Cost;
            int  Priority;
            unsigned int  AuthType;
            int  EncryptionType;
            int  MD5EncryptionType;
            char AuthKey[OSPF_AUTH_KEY_SIZE];
            int  AuthChangeWaitTime;
            int  MD5KeyActWaitTime;
            int MD5KeyId;
            char MD5Key[OSPF_AUTH_KEY_SIZE];
            unsigned short  FilterAll;
            unsigned short  FilterOption;
            unsigned short  mtuIgnore;
            unsigned short  Active;
            unsigned short  Passive;
            unsigned int  NetworkType;
			unsigned int  replayCmdCodeBitmap; 
			unsigned int  ifindex;

} interfaceOSPFConfigMsg_t;


typedef enum
{
	OSPF_PERMIT_DENY_CMD_ROUTE_OPTION,
	OSPF_PERMIT_DENY_CMD_ADDRESS,
	OSPF_PERMIT_DENY_CMD_METRIC
} OSPFPermitDenyCmdCode;


typedef enum
{
    OSPF_NO_ENCR = 0,
    OSPF_SIMPLE_ENCR = 2,
    OSPF_GLOBAL_ENCR = 255,
    OSPF_INVALID_ENCR = 300
}OSPFEncrypType;

typedef enum
{
    OSPF_REDIST_OPTION
}OSPFRedistOption;


typedef enum
{
	OSPF_ROUTE_TYPE_NONE = 0,
    OSPF_ROUTE_TYPE_EXTERNAL,
    OSPF_ROUTE_TYPE_INTER_AREA,
    OSPF_ROUTE_TYPE_INTRA_AREA
}OSPFRouteType;

typedef enum
{
    OSPF_AREA_RANGE_EFFECT_NONE = 0,
    OSPF_AREA_RANGE_EFFECT_ADVERTISE,
    OSPF_AREA_RANGE_EFFECT_NOT_ADVERTISE
}OSPFAreaRangeEffect;


typedef enum {
	OSPF_ROUTER_CMD_REF_BW_VAL = 0,
	OSPF_ROUTER_CMD_REF_BW_USE_ACTIVE_PORTS,
	OSPF_ROUTER_CMD_DB_OVERFLOW_INTERVAL,
	OSPF_ROUTER_CMD_DEF_INFO_ORIGINATE,
	OSPF_ROUTER_CMD_DEFAULT_METRIC,
	OSPF_ROUTER_CMD_DEFAULT_PASSIVE_INTF,

	/* DistanceMO */
	OSPF_ROUTER_CMD_DISTANCE_EXTERNAL,
	OSPF_ROUTER_CMD_DISTANCE_INTER_AREA,
	OSPF_ROUTER_CMD_DISTANCE_INTRA_AREA,

	OSPF_ROUTER_CMD_DISTRIBUTE_LIST_ROUTE_MAP,
	OSPF_ROUTER_CMD_DOMAIN_ID,
	OSPF_ROUTER_CMD_DOMAIN_TAG,
	OSPF_ROUTER_CMD_EXTERNAL_LSDB_LIMIT,

	OSPF_ROUTER_CMD_GRES,
	OSPF_ROUTER_CMD_GRES_HELPER_DISABLE,
	OSPF_ROUTER_CMD_GRES_NOTIFY_TIME,
	OSPF_ROUTER_CMD_GRES_RESTART_TIME,
	//OSPF_ROUTER_CMD_MAX_METRIC_ALL_LSA,
	OSPF_ROUTER_CMD_MAX_METRIC_ROUTER_LSA,
	OSPF_ROUTER_CMD_NEIGHBOR_ADDRESS,
	OSPF_ROUTER_CMD_EXTERNAL_METRIC_TYPE,
	OSPF_ROUTER_CMD_NSSA_TRANSLATOR,
	OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED,
	OSPF_ROUTER_CMD_REDISTRIBUTION_CONNECTED_MAP,
	OSPF_ROUTER_CMD_REDISTRIBUTION_BGP,
	OSPF_ROUTER_CMD_REDISTRIBUTION_BGP_MAP,
	OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC,
	OSPF_ROUTER_CMD_REDISTRIBUTION_STATIC_MAP,
	OSPF_ROUTER_CMD_RFC1583_COMPAT,
	OSPF_ROUTER_CMD_RFC1587_COMPAT,
	OSPF_ROUTER_CMD_TIMER_THROTTLE,
	OSPF_ROUTER_CMD_TIMER_LSA_PACING,
	OSPF_ROUTER_CMD_SUMMARY_ADDRESS,
	OSPF_ROUTER_CMD_LOG_ADJACENCY,
	OSPF_ROUTER_CMD_LOG_DR_ONLY,
	OSPF_ROUTER_CMD_LOG_ALL,
	OSPF_ROUTER_CMD_LOG_BAD_PACKET,
	OSPF_ROUTER_CMD_LOG_CHECKSUM,
	OSPF_ROUTER_CMD_LOG_DATABASE,
	OSPF_ROUTER_CMD_LOG_RETRANSMIT,
	OSPF_ROUTER_CMD_VRF_LITE_CAPABILITY,
	OSPF_ROUTER_CMD_GLOBAL_CONFIG,
	OSPF_ROUTER_CMD_CONFIG_REPLAY,
	OSPF_ROUTER_CMD_MAX
}OSPFRouterConfigCmdCode;

typedef enum {
	OSPF_INVALID_MANAGED_OBJECT = 0,
	OSPF_CONFIG_MANAGED_OBJECT,
	OSPF_AREA_MANAGED_OBJECT,
	OSPF_DISTANCE_MANAGED_OBJECT,
	OSPF_PERMIT_REDISTRIBUTE_MANAGED_OBJECT,
	OSPF_DENY_REDISTRIBUTE_MANAGED_OBJECT,
	OSPF_SUMMARY_ADDRESS_MANAGED_OBJECT,
	OSPF_AREA_RANGE_MANAGED_OBJECT,
	OSPF_NEIGHBOR_IP_ADDRESS_MANAGED_OBJECT,
	OSPF_VIRTUAL_LINK_MANAGED_OBJECT
} OSPFManagedObjectId;

// Flavors of the 'show ip ospf' CLI
typedef enum {
	SHOW_IP_OSPF = 1000,
	SHOW_IP_OSPF_CONFIG,
	SHOW_IP_OSPF_SUMMARY,

	SHOW_IP_OSPF_DATABASE,
	SHOW_IP_OSPF_DATABASE_SUMMARY,
	SHOW_IP_OSPF_GRACE_LINK_STATE,

	// Add other 'show ip ospf <blah>' commands here
	SHOW_IP_OSPF_MAX
} ospf_show_cmd_code_t;

typedef enum OSPFShowCmdCode_ {
    OSPF_SHOW_CMD_AREA = 100,
    OSPF_SHOW_CMD_AREA_ID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ADVERTISE,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ASBR,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ASBR_LSID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ASBR_LDIS_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ASBR_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_EXTENSIVE,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_LSID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_NETWORK,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_NETWORK_ID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_NETWORK_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_NETWORK_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_NSSA,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_NSSA_ID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_NSSA_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_NSSA_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_OPAQUE_AREA,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_OPAQUE_AREA_ID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_OPAQUE_AREA_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_OPAQUE_AREA_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ROUTER_ID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ROUTER_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ROUTER_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_ROUTERID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_SELF_ORIGINATE,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_SEQUENCE_NUM,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_SUMMARY,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_SUMMARY_ID,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_SUMMARY_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_AREA_DATABASE_LSDB_SUMMARY_ADV_ROUTER,
    OSPF_SHOW_CMD_BORDER_ROUTER,
    OSPF_SHOW_CMD_BORDER_ROUTER_ID,
    OSPF_SHOW_CMD_CONFIG,
    OSPF_SHOW_CMD_DATABASE,
    OSPF_SHOW_CMD_DATABASE_SUMMARY,
    OSPF_SHOW_CMD_DATABASE_EXTERNAL_LINK_STATE,
    OSPF_SHOW_CMD_DATABASE_EXTERNAL_LINK_STATE_ADVERTISE,
    OSPF_SHOW_CMD_DATABASE_EXTERNAL_LINK_STATE_EXTENSIVE,
    OSPF_SHOW_CMD_DATABASE_EXTERNAL_LINK_STATE_ID,
    OSPF_SHOW_CMD_DATABASE_EXTERNAL_LINK_STATE_ROUTER_ID,
    OSPF_SHOW_CMD_DATABASE_EXTERNAL_LINK_STATE_SEQUENCE,
    OSPF_SHOW_CMD_DATABASE_GRACE_LINK_STATE,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ADVERTISE,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ASBR,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ASBR_ID,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ASBR_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ASBR_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_EXTENSIVE,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_LSID,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_NETWORK,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_NETWORK_ID,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_NETWORK_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_NETWORK_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_NSSA,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_NSSA_ID,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_NSSA_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_NSSA_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_OPAQUE_AREA,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_OPAQUE_AREA_ID,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_OPAQUE_AREA_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_OPAQUE_AREA_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ROUTER_ID,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ROUTER_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ROUTER_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_ROUTERID,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_SELF_ORIGINATE,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_SEQUENCE_NUM,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_SUMMARY,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_SUMMARY_ID,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_SUMMARY_ID_ADV_ROUTER,
    OSPF_SHOW_CMD_DATABASE_LINK_STATE_SUMMARY_ADV_ROUTER,
    OSPF_SHOW_CMD_DEBUG,
    OSPF_SHOW_CMD_DEBUG_APPENDIX_E,
    OSPF_SHOW_CMD_DEBUG_GRACEFUL_RESTART,
    OSPF_SHOW_CMD_DEBUG_INTERFACE,
    OSPF_SHOW_CMD_DEBUG_MEMORY,
    OSPF_SHOW_CMD_DEBUG_MISC,
    OSPF_SHOW_CMD_DEBUG_SUMMARY_ROUTES,
    OSPF_SHOW_CMD_DEBUG_OPAQUE_LINK,
    OSPF_SHOW_CMD_DEBUG_INTERFACE_ADDRESS,
    OSPF_SHOW_CMD_DEBUG_INTERFACE_PORTCHANNEL,
    OSPF_SHOW_CMD_DEBUG_INTERFACE_TENGIG,
    OSPF_SHOW_CMD_DEBUG_INTERFACE_ONEGIG,
    OSPF_SHOW_CMD_DEBUG_INTERFACE_FORTYGIG,
    OSPF_SHOW_CMD_DEBUG_INTERFACE_SVI,
    OSPF_SHOW_CMD_DEBUG_INTERFACE_LOOPBACK,
    OSPF_SHOW_CMD_DEBUG_IP_OSPF_ALL,
    OSPF_SHOW_CMD_INTERFACE,
    OSPF_SHOW_CMD_INTERFACE_ADDRESS,
    OSPF_SHOW_CMD_INTERFACE_ADDRESS_BRIEF,
    OSPF_SHOW_CMD_INTERFACE_BRIEF,
    OSPF_SHOW_CMD_INTERFACE_PORTCHANNEL,
    OSPF_SHOW_CMD_INTERFACE_PORTCHANNEL_BRIEF,
    OSPF_SHOW_CMD_INTERFACE_TENGIG,
    OSPF_SHOW_CMD_INTERFACE_TENGIG_BRIEF,
    OSPF_SHOW_CMD_INTERFACE_ONEGIG,
    OSPF_SHOW_CMD_INTERFACE_ONEGIG_BRIEF,
    OSPF_SHOW_CMD_INTERFACE_FORTYGIG,
    OSPF_SHOW_CMD_INTERFACE_FORTYGIG_BRIEF,
    OSPF_SHOW_CMD_INTERFACE_SVI,
    OSPF_SHOW_CMD_INTERFACE_SVI_BRIEF,
    OSPF_SHOW_CMD_INTERFACE_LOOPBACK,
    OSPF_SHOW_CMD_INTERFACE_LOOPBACK_BRIEF,
    OSPF_SHOW_CMD_NEIGHBOR,
    OSPF_SHOW_CMD_NEIGHBOR_EXTENSIVE,
    OSPF_SHOW_CMD_NEIGHBOR_PORTCHANNEL,
    OSPF_SHOW_CMD_NEIGHBOR_PORTCHANNEL_EXTENSIVE,
    OSPF_SHOW_CMD_NEIGHBOR_ROUTERID,
    OSPF_SHOW_CMD_NEIGHBOR_ROUTERID_EXTENSIVE,
    OSPF_SHOW_CMD_NEIGHBOR_TENGIG,
    OSPF_SHOW_CMD_NEIGHBOR_TENGIG_EXTENSIVE,
    OSPF_SHOW_CMD_NEIGHBOR_ONEGIG,
    OSPF_SHOW_CMD_NEIGHBOR_ONEGIG_EXTENSIVE,
    OSPF_SHOW_CMD_NEIGHBOR_FORTYGIG,
    OSPF_SHOW_CMD_NEIGHBOR_FORTYGIG_EXTENSIVE,
    OSPF_SHOW_CMD_NEIGHBOR_SVI,
    OSPF_SHOW_CMD_NEIGHBOR_SVI_EXTENSIVE,
    OSPF_SHOW_CMD_ROUTES,
    OSPF_SHOW_CMD_ROUTES_ADDRESS,
    OSPF_SHOW_CMD_REDISTRIBUTE_ROUTES,
    OSPF_SHOW_CMD_REDISTRIBUTE_ROUTES_ADDRESS,
    OSPF_SHOW_CMD_SUMMARY,
	OSPF_SHOW_CMD_TRAFFIC,
	OSPF_SHOW_CMD_TRAFFIC_PORTCHANNEL,
	OSPF_SHOW_CMD_TRAFFIC_TENGIG,
	OSPF_SHOW_CMD_TRAFFIC_SVI,
	OSPF_SHOW_CMD_TRAFFIC_LOOPBACK,
	OSPF_SHOW_CMD_TRAFFIC_ONEGIG,
	OSPF_SHOW_CMD_TRAFFIC_FORTYGIG,
    OSPF_SHOW_CMD_VIRTUAL_LINK,
    OSPF_SHOW_CMD_VIRTUAL_LINK_ID,
    OSPF_SHOW_CMD_VIRTUAL_NEIGHBOR,
    OSPF_SHOW_CMD_VIRTUAL_NEIGHBOR_ID,
    OSPF_SHOW_CMD,
    OSPF_SHOW_CMD_MAX
} OSPFShowCmdCode_t;

// Flavors of the 'clear ip ospf' CLI
typedef enum {
	CLEAR_IP_OSPF_ALL,
	CLEAR_IP_OSPF_NEIGHBOR,
	CLEAR_IP_OSPF_ROUTES,
	CLEAR_IP_OSPF_COUNTERS,
	// Add the others here
	CLEAR_IP_OSPF_MAX
} ospf_clear_cmd_code_t;


// Flavors of the 'debug ip ospf' CLI
typedef enum {
	DEBUG_IP_OSPF_ADJ,
	DEBUG_IP_OSPF_ALL,
	DEBUG_IP_OSPF_ERR,
	DEBUG_IP_OSPF_EVENTS,
	DEBUG_IP_OSPF_FLOOD,
	DEBUG_IP_OSPF_LOG_DBG_MSG,
	DEBUG_IP_OSPF_LOG_EMTY_LSA,
	DEBUG_IP_OSPF_LS_ID,
	DEBUG_IP_OSPF_LSA_GENERATION,
	DEBUG_IP_OSPF_MAX_METRIC,
	DEBUG_IP_OSPF_NEIGHBOR,
	DEBUG_IP_OSPF_PACKET,
	DEBUG_IP_OSPF_RETRX,
	DEBUG_IP_OSPF_ROUTE,
	DEBUG_IP_OSPF_SPF,
	// Add the others here
	DEBUG_IP_OSPF_MAX
} ospf_debug_cmd_code_t;

typedef struct {
    unsigned long size; // size of data that is sent
    void *data; //Ptr to cookie structure
} show_handle_t;

// Object filled and sent back by backend
// Union decided by ospf_show_cmd_code_t
typedef struct {

	OSPFShowCmdCode_t cmdCode;
	unsigned int size;
	unsigned int num_entries;

	int data_pending;
	show_handle_t page_handle;
    int retCode;

	/*
	 * Obsolete
	 */
	// Specifies which data struct has been sent back
	ospf_show_cmd_code_t cmd;

	char buf[1];
} ospf_show_out_msg_t;

/*
 * Show command message for backend
*/
typedef struct ospf_show_in_msg_ {
    OSPFShowCmdCode_t cmdCode;
    char area_id[OSPF_IPV4_ADDR_STR_SIZE];
    unsigned int intVal;
    unsigned int ipaddr1;
    unsigned int ipaddr2;
    char if_name[OSPF_DCM_IF_NAME_LEN];
	char strVal[OSPF_STR_64_LEN+1];
	int data_pending;
	show_handle_t page_handle;
	char m_vrf[NSM_INTF_NAME_SIZE];

	/*
	 * Obsolete
	 */
	ospf_show_cmd_code_t cmd;
	int summary;
	int argc;
	const char *argv[OSPF_CLI_MAX_ARGS];
} ospf_show_in_msg_t;



// Opcodes for the area cmd
typedef enum {
	OSPF_AREA_CONFIG = 0,
	OSPF_AREA_CONFIG_NSSA,
    OSPF_AREA_CONFIG_NSSA_NO_SUMMARY,
    OSPF_AREA_CONFIG_NSSA_DEF_INFO_ORIG,
	OSPF_AREA_CONFIG_STUB,
    OSPF_AREA_CONFIG_STUB_NO_SUMMARY,

	/*
	 * Area Range
	 */
	OSPF_AREA_CONFIG_RANGE,

	/*
	 * Vurtual Link
	 */
    OSPF_AREA_CONFIG_VIRTUAL_LINK,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_HELLO_INT,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_DEAD_INT,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_RETRANS_INT,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_TRANS_DLY,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_SWITCH_KEY,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_ENCRYPTTYPE,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_AUTH_KEY,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_ACT_WAIT_TIME,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_KEY_ID,
    OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY,
	
	/*
	 * Config Replay
	 */
	OSPF_AREA_CONFIG_REPLAY,
	OSPF_AREA_VIRTUAL_LINK_REPLAY,

	OSPF_AREA_CONFIG_MAX
} ospf_area_config_cmd_code_t;

#define OSPF_VL_MAX_PARAMS       4
#define OSPF_VL_MD5_KEYID_LOC    0  
#define OSPF_VL_MD5_KEY_INDEX    1 
#define OSPF_VL_MD5_KEYID_INDEX  3

// What the backend receives from CLI for an area cmd
typedef struct {
	ospf_area_config_cmd_code_t cmdCode;
    unsigned int opCode;
	char area_id[OSPF_IPV4_ADDR_STR_SIZE];
    unsigned int nssa;
    unsigned int stub;

    /*
	 * Variables for Virtual Link
	 */
    char vlink_routerid[OSPF_IPV4_ADDR_STR_SIZE];
	unsigned int  vlink_hello_interval;
	unsigned int  vlink_dead_interval;
	unsigned int  vlink_retransmit_interval;
	unsigned int  vlink_transmit_delay;
	int           vlink_auth_type;
	int           vlink_encr_type;
	char          vlink_auth_key[OSPF_AUTH_KEY_SIZE];
	unsigned int  vlink_md5_key_act_wait_time;
	unsigned char vlink_md5_key_id;
	char          vlink_md5_key[OSPF_AUTH_KEY_SIZE];

	/*
	 * Area Range
	 */
	char range_ip[OSPF_IPV4_ADDR_STR_SIZE];
	char range_mask[OSPF_IPV4_ADDR_STR_SIZE];
	unsigned int range_effect;
	unsigned int range_cost;

	/*
	 * VRF Name
	 */
	char m_vrf[NSM_INTF_NAME_SIZE];

	/*
	 * Config replay
	 */
	unsigned int cmd_code_bitmap; 
} ospf_area_config_in_msg_t;


// What the backend returns for an ospf area cmd (ideally nothing, only error string)
typedef struct {
	unsigned int size;
	char buf[OSPF_ERROR_MAX_OUTPUT_SIZE];
    unsigned char auth_type;
    char auth_key[CU_MD5_PASSWORD_SIZE+1];
    char encrypted_key[CU_MD5_PASSWORD_ENCRYPTED_SIZE+2];
} ospf_area_config_out_msg_t;

typedef struct {
	unsigned int size;
    unsigned char auth_type; 
    char auth_key[CU_MD5_PASSWORD_SIZE+1];
	char encrypted_key[CU_MD5_PASSWORD_ENCRYPTED_SIZE+2];
} ospf_intf_config_out_msg_t;

// The clear cmd the backend receives from CLI. This should have all the info
// it needs to determine the exact 'clear ip ospf' command executed.
// See if this should also be a union based on ospf_clear_cmd_code_t.
typedef struct {
    ospf_clear_cmd_code_t cmd;
    char if_name[OSPF_DCM_IF_NAME_LEN];
    char vrf_name[OSPF_DCM_IF_NAME_LEN + 1];
	// others
    int argc;
    const char *argv[OSPF_CLI_MAX_ARGS];
} ospf_clear_in_msg_t;

//To store the error mesg if any after executing clear ip ospf cli
typedef struct {
    unsigned int size;
    char buf[OSPF_ERROR_MAX_OUTPUT_SIZE];
} ospf_clear_out_msg_t;


// The debug cmd the backend receives from CLI. This should have all the info
// it needs to determine the exact 'clear ip ospf' command executed.
// See if this should also be a union based on ospf_debug_cmd_code_t.
typedef struct {
    ospf_debug_cmd_code_t cmd;
    char if_name[OSPF_DCM_IF_NAME_LEN];
    char vrf_name[OSPF_DCM_IF_NAME_LEN + 1];

	//int argc;
	//char **argv;
	// others
    int argc;
    const char *argv[OSPF_CLI_MAX_ARGS];
} ospf_debug_in_msg_t;

//To store the error mesg if any after executing debug ip ospf cli
typedef struct {
    unsigned int size;
    char buf[OSPF_ERROR_MAX_OUTPUT_SIZE];
} ospf_debug_out_msg_t;

#define OSPF_DCM_CLI_NEXT_LSA_ENTRY_OFFSET(ENT) \
			(ospf_show_lsdb_entry_t*)(((unsigned long)ENT) + \
									  sizeof(ospf_show_lsdb_entry_t) + \
									  ENT->num_entries * sizeof(ospf_show_lsa_entry_t))

#define OSPF_CLI_GET_NEXT_DEBUG_AREA_ENTRY(ENT) \
        (ospf_show_debug_area_entry_t*)((((unsigned long)(ENT)) + \
										 sizeof(ospf_show_debug_area_entry_t)) + \
										ENT->num_routes*sizeof(ospf_show_debug_route_entry_t))

#define OSPF_CLI_GET_NEXT_GRES_LSA_ENTRY(ENT) \
            (ospf_show_debug_gres_lsa_entry_t*)((unsigned long)(ENT + 1) + \
			                                                ENT->num_tlv*sizeof(ospf_show_debug_gres_lsa_tlv_t))

#define OSPF_CLI_AREA_ENTRY_TYPE_DATA		0
#define OSPF_CLI_AREA_ENTRY_TYPE_HEADER		1

typedef struct ospf_show_entry_ {
    unsigned long of_router_id;
    char of_router_id_ip_address_format;
    unsigned long of_domain_id;
    unsigned long of_domain_tag;
    char autonomous_system_border_router;
    char is_abr;
    char redistribute_static;
    char redistribute_connected;
    char redistribute_bgp;
    char default_information_originate;
    unsigned long spf_delay_time;
    unsigned long spf_hold_time;
    unsigned long spf_max_hold_time;
    unsigned long spf_next_run_wait;
    unsigned long ospfExternLsaCount;
    unsigned long ospfExternLsaCksumSum;
    unsigned long ospfOriginateNewLsas;
    unsigned long ospfRxNewLsas;
    unsigned long ospfExitOverflowInterval;
    unsigned int external_lsa_limit;
    char external_lsdb_overflow_state;
    char rfc1583_compatibility;
    char nssa_translator;
    char nsr_enabled;
    char spf_next_run_wait_valid;
	struct {
		unsigned int enabled:1;			
		unsigned int start_up:1;			
		unsigned int wait_for_bgp:1;		
		unsigned int active:1;				
		unsigned int all_link:1;			
		unsigned int ptp_link:1;			
		unsigned int stub_link:1;			
		unsigned int transit_link:1;		
		unsigned int external_lsa:1;		
		unsigned int summary_lsa:1;		
		unsigned int te_lsa:1;				
		unsigned int time;
  		unsigned int day;
		unsigned int hr;
		unsigned int min;
		unsigned int sec;
		unsigned int external_lsa_metric;	
		unsigned int summary_lsa_metric;	
		unsigned int te_lsa_metric;	
	}max_metric_data;
} ospf_show_entry_t;

typedef struct ospf_show_area_entry_ {
  unsigned int  entry_type;
  unsigned int  table_index;
  unsigned int  area_id; /* A 32-bit integer uniquely identifying an area */
  char          is_area_address_format;
  char          type[20];
  unsigned int  stub_metric;        /* Stub Area Metric Cost */
  unsigned int  shortest_path_first_run_count;
  unsigned short number_of_area_border_routers;
  unsigned short number_of_autonomous_system_boundary_routers;
  unsigned int  mib_number_of_link_state_advertisements;
  unsigned int  mib_checksum_sum;

  /*
   * header
   */
  unsigned int num_area;
  unsigned int num_nssa;
} ospf_show_area_entry_t;

typedef struct ospf_show_config_area_ {
  unsigned int area_id;
  char         is_area_address_format;
  char area_type[20];
  int  cost;
  int  summary;
} ospf_show_config_area_entry_t;

typedef struct ospf_show_config_area_range_ {
  unsigned int area_id;
  char         is_area_address_format;
  unsigned char advertise_address_range;
  unsigned int ip_address;
  unsigned int subnetmask;
  unsigned int config_cost;
} ospf_show_config_area_range_entry_t;

#define OSPF_CLI_AUTH_TYPE_NONE			0
#define OSPF_CLI_AUTH_TYPE_SIMPLE		1
#define OSPF_CLI_AUTH_TYPE_MD5			2

typedef struct ospf_show_config_interface_entry_ {
  unsigned int port;
  unsigned int area_id;
  unsigned short authentication_type;
  char auth_key[16];
  char md5_auth_key[16];
  int  md5_key_id;
  int  key_activation_wait_time;

  struct {
    int path_cost_configured:1;
    int router_dead_interval_configued:1;
    int hello_interval_configued:1;
    int priority_configured:1;
    int retransmit_interval_configured:1;
    int transmit_delay_configured:1;
    int is_area_address_format:1;
  } flags;

  unsigned int path_cost;
  unsigned int router_dead_interval;
  unsigned int hello_interval;
  unsigned int priority;
  unsigned int retransmit_interval;
  unsigned int transmit_delay;
} ospf_show_config_interface_entry_t;

typedef struct ospf_show_config_virtual_link_entry_ {
  unsigned short authentication_type;
  char auth_key[16];
  char md5_auth_key[16];
  int  md5_key_id;
  int  key_activation_wait_time;

  unsigned int transit_area_id;
  unsigned int neighbor_ip_address;
  int  router_dead_interval;
  int  hello_interval;
  int  retransmit_interval;
  int  transit_delay;

  unsigned int table_index;
  char state[10];
  char is_area_address_format;
  unsigned int events;
} ospf_show_config_virtual_link_entry_t;

typedef struct ospf_show_config_redist_address_range_entry_ {
  unsigned int ip_address;
  unsigned int subnetmask;
} ospf_show_config_redist_address_range_entry_t;

typedef struct ospf_show_config_redist_entry_ {
  unsigned char is_permit;
  unsigned int  index;
  char          protocol[16];
  int           rip_metric;
  int           ospf_metric_val;
  unsigned char address_configured;
  unsigned int  ip_address;
  unsigned int  subnet_mask;
} ospf_show_config_redist_entry_t;

typedef struct ospf_show_config_ {
  struct {
      int protocol_enabled:1;
      int graceful_restart:1;
      int graceful_restart_helper:1;
      int import_routes:1;
      int autocost_enabled:1;
      int rfc1583_compatibility:1;
      int if_state_change:1;
      int virtual_if_state_change:1;
      int neighbor_state_change:1;
      int virtual_neighbor_state_change:1;
      int if_config_error:1;
      int virtual_if_config_error:1;
      int if_auth_failure:1;
      int virtual_if_auth_failure:1;
      int if_rx_bad_packet:1;
      int virtual_if_rx_bad_packet:1;
      int if_retransmit:1;
      int virtual_if_retransmit:1;
      int originate_lsa:1;
      int max_age_lsa:1;
      int lsdb_overflow:1;
      int lsdb_approaching_overflow:1;
      int is_area_address_format:1;
	  int default_passive_interface:1;
	  int vrf_lite_capability:1;
  } flags;

  unsigned int  graceful_restart_time;
  unsigned int graceful_restart_notify_time;
  unsigned int default_ospf_metric;
  unsigned int autocost_refbw;
  unsigned int redistribution_metric_type;
  unsigned int external_lsdb_limit;
  unsigned int database_overflow_interval;
  unsigned int router_id;

  unsigned int num_area;
  unsigned int num_area_range;
  unsigned int num_interface;
  unsigned int num_virtual_link;
  unsigned int num_redist_address_range;
  unsigned int num_redistribution;

  char area_entry[1];
} ospf_show_config_t;

typedef struct ospf_show_lsa_entry_ {
  union {
    struct {
      unsigned int link_id;
      unsigned int link_data;
      char         link_type[32];
      unsigned int number_of_metrics;
      unsigned short tos0_metric;
      unsigned short local_metric;
    } router;

    struct {
      unsigned int link_id;
    } network;

    struct {
      unsigned char tos;
      unsigned int  metric;
    } summary;

    struct {
        unsigned char tos;
        unsigned int metric_type;
        unsigned int metric;
        unsigned int tos0_forwarding_address;
        unsigned int external_route_tag;
    } external;


    struct {
      /* not supported */
    } opaque_area;
  } u;
} ospf_show_lsa_entry_t;

#define OSPF_SHOW_LSTYPE_COMMON			1
#define OSPF_SHOW_LSTYPE_ROUTER			2
#define OSPF_SHOW_LSTYPE_NETWORK		3
#define OSPF_SHOW_LSTYPE_SUMMARY		4
#define OSPF_SHOW_LSTYPE_EXTERNAL		5
#define OSPF_SHOW_LSTYPE_OPAQUE			6

typedef struct ospf_show_lsdb_entry_ {
  unsigned int table_index;
  unsigned char  lstype;
  unsigned char  lsinfo_common;

  union {
    struct {
        unsigned int area_id;
        char         is_area_address_format;
        char         adv_type[20];
        unsigned int link_state_id;
        unsigned short age;
    } router;

    struct {
        unsigned int area_id;
        char         is_area_address_format;
        char         adv_type[20];
        unsigned int link_state_id;
        unsigned short age;
    } network;

    struct {
        unsigned int area_id;
        char         is_area_address_format;
        char         adv_type[20];
        unsigned int link_state_id;
        unsigned short age;
    } summary;

    struct {
        unsigned int age;
        unsigned int link_state_id;
        unsigned int advertising_router;
        unsigned int netmask;
        unsigned int metric;
        unsigned int flag;
        unsigned int forwarding_address;
    } external;

	struct {
		unsigned int area_id;
		char is_area_address_format;
		char         adv_type[20];
		unsigned int link_state_id;
		unsigned int router_id;
		unsigned int age;
		unsigned int sequence_number;
		unsigned int checksum;
	} common;
  } lsa;


  unsigned int   num_entries;

  union {
    struct {
      unsigned int options;
      unsigned int sequence_number;
      unsigned int length;
      unsigned int flags;
    } router;

    struct {
      unsigned int options;
      unsigned int sequence_number;
      unsigned int length;
      unsigned int network_mask;
    } network;

    struct {
      unsigned int options;
      unsigned int sequence_number;
      unsigned int length;
      unsigned int network_mask;
    } summary;

    struct {
      unsigned int age;
      unsigned int options;
      unsigned int sequence_number;
      unsigned int length;
      unsigned int network_mask;
      unsigned int retransmit_neighbor_count;
    } external;
  } hdr;

  ospf_show_lsa_entry_t lsa_entry[1];
} ospf_show_lsdb_entry_t;

typedef struct ospf_show_database_summary_entry_ {
  unsigned int area_id;
  char         is_area_address_format;
  unsigned int router_lsa_count;
  unsigned int network_lsa_count;
  unsigned int summary_network_lsa_count;
  unsigned int summary_boundary_router_lsa_count;
  unsigned int nssa_external_lsa_count;
  unsigned int opaque_area_lsa_count;
  unsigned int mib_number_of_link_state_advertisements;
} ospf_show_database_summary_entry_t;

typedef struct ospf_show_database_summary_ {
  unsigned int external_lsa_count;
  unsigned int total_router;
  unsigned int total_network;
  unsigned int total_summary;
  unsigned int total_summary_asbr;
  unsigned int total_nssa_ext;
  unsigned int total_opq_area;
  unsigned int total_subtotal;
  char is_mpls_enabled;

  unsigned int num_entries;

  ospf_show_database_summary_entry_t entry[1];
} ospf_show_database_summary_t;

typedef struct ospf_show_grace_link_entry_ {
  unsigned int area_id;
  char         is_area_address_format;
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int advertising_router;
  unsigned int lsa_age;
  unsigned int sequence_number;
  char gr_period[20];
  char gr_reason[20];
  char gr_intfip[20];
} ospf_show_grace_link_t;

#define OSPF_DCM_CLI_NEXT_INTF_ENTRY(ENT) \
	(ospf_show_interface_entry_t*)(((unsigned long)ENT) + \
	 sizeof(ospf_show_interface_entry_t) + \
	 (ENT)->num_non_broadcast_neighbors*sizeof(unsigned int) + \
	 (ENT)->num_neighbors*sizeof(ospf_show_interface_neighbor_entry_t))

typedef struct ospf_show_ip_address_entry_ {
  unsigned int ip_address;
} ospf_show_ip_address_entry_t;

typedef struct ospf_show_interface_neighbor_entry_ {
  unsigned int ip_address;
  unsigned int id;
} ospf_show_interface_neighbor_entry_t;
typedef struct ospf_show_interface_entry_ {
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int ip_address;
  unsigned int area_id;
  char state[40];
  char type[32];
  unsigned short rtr_priority;
  unsigned int   metric;
  unsigned int   options;
  unsigned int   events;

  unsigned int   transit_delay;
  unsigned int   retransmit_delay;
  unsigned int   hello_interval;
  unsigned int   dead_interval;

  unsigned int   designated_router_id;
  unsigned int   designated_router_address;
  unsigned int   backup_designated_router_id;
  unsigned int   backup_designated_router_address;
  unsigned int   number_of_neighbors;
  unsigned int   number_of_neighbors_in_full_state;

  unsigned char  authentication_type;
  char           auth_key[16];
  char           md5_auth_key[16];
  unsigned int   md5_auth_key_id;
  unsigned int   md5_auth_key_activation_wait_time;

  unsigned int   num_non_broadcast_neighbors;
  unsigned int   num_neighbors;

  struct {
      int admin_up:1;
      int oper_up:1;
      int bfd_enabled:1;
      int is_area_address_format:1;
      int filter_all_out:1;
      int filter_all_external:1;
      int filter_all_summary_and_external:1;
      int allow_default:1;
      int allow_type4:1;
      int is_if_non_broadcast:1;
	  int md5_auth_key_id_not_used:1;
  } flags;
} ospf_show_interface_entry_t;

typedef struct ospf_show_interface_brief_entry_ {
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned char is_area_address_format;
  unsigned int  area_id;
  unsigned int  ip_address;
  unsigned int  masklen;
  unsigned int  metric;
  char          state[32];
  unsigned int  number_of_neighbors_in_full_state;
  unsigned int  number_of_neighbors;
} ospf_show_interface_brief_entry_t;

typedef struct ospf_show_debug_ip_ospf_entry_ {
    unsigned long neighbor_ip_address;
    unsigned long ls_id;
    unsigned long route_ip_address;
    unsigned long   adj:1,
                    assert:1,
//#ifndef __NO_BFD__
                    bfd:1,
//#endif /*__NO_BFD__*/
                    database_timer:1,
                    dijkstra:1,
                    error:1,
                    events:1,
                    grace:1,
                    flood:1,
                    log_debug_message:1,
                    log_empty_lsa:1,
                    lsa_generation:1,
//#ifndef __NO_OSPF_MAX_METRIC__
                    max_metric:1,
//#endif /*__NO_OSPF_MAX_METRIC__*/
                    packet:1,
                    per_lsid_debugging:1,
                    retransmission:1,
                    sham_link:1,
                    shortcuts:1,
                    spf:1,
                    spf_short:1,
                    unused:12;
} ospf_show_debug_ip_ospf_entry;

typedef struct ospf_show_debug_route_entry_ {
  unsigned int id;
  unsigned int mask;
} ospf_show_debug_route_entry_t;

typedef struct ospf_show_debug_area_entry_ {
  unsigned int area_id;
  unsigned int num_routes;
} ospf_show_debug_area_entry_t;

typedef struct ospf_show_debug_missing_db_entry_ {
  unsigned int idx;
  unsigned int log_index;
  unsigned int entry_log;
} ospf_show_debug_log_db_entry_t;

typedef struct ospf_show_debug_appendix_e_ {
  int ospf_e_external_lsa_id_alloc_fail;
  int ospf_e_summary_lsa_id_alloc_fail;

  unsigned int num_ext_routes;
  unsigned int num_area;
  unsigned int num_missing_log;
  unsigned int num_unexpected_log;
} ospf_show_debug_appendix_e_t;

typedef struct ospf_show_debug_gres_helper_entry_ {
  char is_virtual_link;
  unsigned int address;
  unsigned int id;
  unsigned int area_id;
  unsigned int physical_port;
  unsigned int state;
  unsigned int grace_lsa_received;
  unsigned int in_helper_state;
  unsigned int gres_timer;
} ospf_show_debug_gres_helper_entry_t;

#define OSPF_CLI_GRES_TLV_UNKNOWN					0
#define OSPF_CLI_GRES_TLV_GRACE_PERIOD				1
#define OSPF_CLI_GRES_TLV_GRACE_RESTART_REASON		2
#define OSPF_CLI_GRES_TLV_INTERFACE_ADDRESS			3

typedef struct ospf_show_debug_lsa_tlv_ {
  unsigned int cli_type;
  unsigned int type;
  unsigned int length;
  unsigned int value;
} ospf_show_debug_gres_lsa_tlv_t;

typedef struct ospf_show_debug_gres_lsa_entry_ {
  unsigned char is_virtual_link;
  unsigned int  area_id;
  unsigned int  physical_port;
  unsigned int  id;
  unsigned int  type;
  unsigned int  age;
  unsigned int  max_age;
  unsigned int  sequence_number;
  unsigned int  interface;
  unsigned int  options;

  unsigned int num_tlv;
} ospf_show_debug_gres_lsa_entry_t;

typedef struct ospf_show_debug_graceful_restart_ {
  unsigned int mp_active;
  unsigned int standby_mp_up;
  unsigned int num_nbr_full_state;
  unsigned int num_gres_helper;
  unsigned int num_vi_nbr_full_state;
  unsigned int num_gres_vi_helper;
  unsigned int gres_enable;
  unsigned int gres_helper;
  unsigned int restart_time;
  unsigned int notify_time;
  unsigned int restart_up_time_count;
  unsigned int restart_detected;

  unsigned int num_helper_entry;
  unsigned int num_lsa_entry;
} ospf_show_debug_graceful_restart_t;

typedef struct ospf_show_debug_interface_entry_ {
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned char is_area_address_format;
  unsigned int address;
  unsigned int area_id;
  struct {
      int filter_lsa:1;
      int filter_all_external:1;
      int filter_all_summary_and_external:1;
      int allow_default:1;
      int allow_type4:1;
  } flags;

  unsigned int route_lsa_count;
  unsigned int network_lsa_count;
  unsigned int summary_network_lsa_count;
  unsigned int summary_as_boundary_router_count;
  unsigned int external_lsa_count;
  unsigned int nssa_external_lsa_count;
  unsigned int opaque_lsa_count;
} ospf_show_debug_interface_entry_t;

typedef struct ospf_show_debug_mem_pool_info_ {
  unsigned int block_size;
  unsigned int number_of_blocks;
  unsigned int number_of_blocks_in_use;
  unsigned int number_of_blocks_free;
  unsigned int number_of_time_memory_alloc_failed;
  unsigned int sptr_memory_start;
  unsigned int sptr_memory_list;
} ospf_show_debug_mem_pool_info_t;

typedef struct ospf_show_debug_mem_pool_type_info_ {
  unsigned int ospf_memory_block_use_count;
  unsigned int ospf_memory_block_allocation_error;
  unsigned int ospf_memory_block_allocation_ever;
  unsigned int ospf_memory_block_freed_ever;
  char ospf_mem_block_type[64];
} ospf_show_debug_mem_pool_type_info_t;

#define OSPF_CLI_MAX_MEM_POOLS 16
#define OSPF_CLI_MAX_MEM_POOL_TYPE  60

typedef struct ospf_show_debug_mega_mem_info_ {
  unsigned int number_of_mega_blocks_allocated;
  unsigned int number_of_block_corruptions;
} ospf_show_debug_mega_mem_info_t;

typedef struct ospf_show_debug_memory_ {
  unsigned int total_memory_use;
  unsigned int mem_quota;
  unsigned int default_lsdb_limit;

  unsigned int num_mem_pools;
  ospf_show_debug_mem_pool_info_t mem_pools[OSPF_CLI_MAX_MEM_POOLS];
  unsigned int total_blocks_allocated;

  unsigned int num_mega_mem_pools;
  ospf_show_debug_mega_mem_info_t mega_pools[OSPF_CLI_MAX_MEM_POOLS];

  ospf_show_debug_mem_pool_type_info_t pool_type_info[OSPF_CLI_MAX_MEM_POOL_TYPE];
  unsigned int num_mem_type_pools;
  unsigned int total_mem_type_blocks_allocated;

  unsigned int main_routing_table_address;
  unsigned int trie_node_count;
  unsigned int top_address;
  unsigned int default_valid;
  unsigned int default_route;
} ospf_show_debug_memory_t;

#define OSPF_CLI_ACL_NAME_LEN   32
#define OSPF_CLI_MAX_POLICY_NAME_LEN    90

typedef struct ospf_show_debug_misc_ {
  unsigned int fwd_addr_count;
  unsigned int num_routes_imported;
  unsigned int ext_route_flap_count;
  unsigned int nssa_route_flap_count;
  unsigned int ext_lsa_count;
  unsigned int nssa_lsa_count;
  unsigned int phase_number;
  unsigned int area_id;
  unsigned int next_chunk_id;
  unsigned int time_consumed;
  unsigned int max_age_ext_lsa_count;
  unsigned int total_ext_lsa_count;

  char         dist_list_valid;
  char         acl_enabled;
  char         routemap_enabled;
  char         acl_name[OSPF_CLI_ACL_NAME_LEN];
  char         distlist_routemap_names[OSPF_CLI_MAX_POLICY_NAME_LEN];
  unsigned int acl_id;
  unsigned long acl_pointer;
  unsigned long distlist_routemaps_pointer;
} ospf_show_debug_misc_t;

typedef struct ospf_show_debug_opaque_link_ {
  char is_virtual_link;
  unsigned int physical_port;
  unsigned int area_id;
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int ls_id;
  unsigned int advertising_router;
  unsigned int type;
  unsigned int age;
  unsigned int sequence_number;
  unsigned int options;
  char lsa_ifname[OSPF_DCM_IF_NAME_LEN + 1];
} ospf_show_debug_opaque_link_t;

typedef struct ospf_show_debug_summary_route_ {
  unsigned int destination;
  unsigned int address_mask;
  unsigned int outgoing_interface;
} ospf_show_debug_summary_route_t;

typedef struct ospf_show_instance_summary_entry_ {
  unsigned int table_index;
  char vrf_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int num_interface;
  unsigned int num_neighbors;
  unsigned int num_neighbors_in_full_state;
  unsigned int lsa_count;
  unsigned int route_count;
} ospf_show_instance_summary_entry_t;

typedef struct ospf_show_neighbor_summary_entry_ {
  unsigned int num_neighbors;
  unsigned int num_neighbors_in_full_state;
}ospf_show_neighbor_summary_entry_t;

typedef struct ospf_show_sham_link_entry_ {
  char nbr_state[12];
  char state[12];
  char is_down;
  char is_area_address_format;
  char vrf_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int dest_address;
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int area_id;
  unsigned int source_address;
  unsigned int cost;
  unsigned int transit_delay;
  unsigned int hello_int;
  unsigned int rtr_dead_int;
  unsigned int retrans_delay;
  unsigned int events;
} ospf_show_sham_link_entry_t;

typedef struct ospf_show_virtual_nbr_entry_ {
  char state[20];
  char is_area_address_format;
  unsigned int table_index;
  unsigned int area_id;
  unsigned int neigh_rtr_id;
  unsigned int neigh_addr;
  unsigned int options;

  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int ip_address;
  unsigned int events;
  unsigned int retransmit_queue_count;

  char grace_lsa_received;
  unsigned int nbr_in_helping_state;
  unsigned int gres_timer;
} ospf_show_virtual_nbr_entry_t;

typedef struct ospf_show_neighbor_entry_ {
  char state[20];
  char rt_state[20];
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int ip_address;
  unsigned int priority;
  unsigned int neighbor_address;
  unsigned int neighbor_router_id;
  unsigned int events;
  unsigned int options;
  unsigned int retransmit_queue_count;
  char grace_lsa_received;
  unsigned int nbr_in_helper_state;
  unsigned int gres_timer;

  /*
   * detail output
   */
  char is_neighbor_state_full;
  unsigned int d1;
  unsigned int h1;
  unsigned int m1;
  unsigned int s1;

  unsigned int d2;
  unsigned int h2;
  unsigned int m2;
  unsigned int s2;
} ospf_show_neighbor_entry_t;

typedef struct ospf_show_route_path_info_ {
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  unsigned int nexthop_router_ip_address;
  char fwd_type[12];
  unsigned char fwd_state[2];
} ospf_show_route_path_info_t;

#define OSPF_SHOW_ROUTE_ENTRY_ASBR              1
#define OSPF_SHOW_ROUTE_ENTRY_NON_TUNNEL_ASBR   2
#define OSPF_SHOW_ROUTE_ENTRY_AREA_SUMMARY      3
#define OSPF_SHOW_ROUTE_ENTRY_EXTERNAL_SUMMARY  4
#define OSPF_SHOW_ROUTE_ENTRY_NON_TUNNEL        5
#define OSPF_SHOW_ROUTE_ENTRY_REGULAR           6

typedef struct ospf_show_route_entry_ {
  unsigned int entry_type; /* SET: first entry; UNSET: succeeding entry */
  unsigned int area_id;
  char is_area_address_format;
  char path_type[12];
  unsigned int destination_id;
  unsigned int address_mask;
  unsigned int path_cost;
  unsigned int type2_cost;

  char dest_type[12];
  char state[12];
  unsigned int advertising_router;
  unsigned int link_state_id;
  unsigned int tag;
  unsigned int flags;

  unsigned int num_entries;

} ospf_show_route_entry_t;

typedef struct ospf_show_redistribute_route_entry_ {
  unsigned int destination_id;
  unsigned int destination_mask;
  unsigned int forwarding_address;
  unsigned int metric;
  unsigned char fwd_addr_reachable;
  char protocol[10];
} ospf_show_redistribute_route_entry_t;

typedef struct ospf_show_bdr_router_entry_ {
  char router_type[20];
  unsigned int router_id;
  unsigned int nexthop_router_ip_address;
  char outgoing_intf_is_tunnel;
  unsigned int tunnel_id;
  unsigned int table_index;
  char if_name[OSPF_DCM_IF_NAME_LEN + 1];
  char is_area_address_format;
  unsigned int area_id;
} ospf_show_bdr_router_entry_t;

#define OSPF_SHOW_TRAFFIC_MAX_PACKET_TYPE	(OSPF_LINK_STATE_ACKNOWLEDGEMENT_PACKET + 1)
#define OSPF_SHOW_TRAFFIC_PKT_STR_LEN		32

typedef struct ospf_show_traffic_entry_ {
    char pkt_type_str[OSPF_SHOW_TRAFFIC_MAX_PACKET_TYPE][OSPF_SHOW_TRAFFIC_PKT_STR_LEN];
	unsigned int rx_pkt_cnt[OSPF_SHOW_TRAFFIC_MAX_PACKET_TYPE];
	unsigned int tx_pkt_cnt[OSPF_SHOW_TRAFFIC_MAX_PACKET_TYPE];
	char pkt_error_type[OSPF_MAX_ERROR][OSPF_SHOW_TRAFFIC_PKT_STR_LEN];
	unsigned int pkt_error_cnt[OSPF_MAX_ERROR];
} ospf_show_traffic_entry_t;

//#define DEFAULT_VRF_NAME         "default-vrf"

#endif	/* __OSPF_MESSAGE_DEF_H__ */

