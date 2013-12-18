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

/**
 *	@file include/iswitch/isw_ipc.h
 *
 *	Header file relevant to External API interfaces
 *
 *  Copyright (c) 2001-2004 by Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 */
#ifndef _ISW_IPC_H
#define _ISW_IPC_H
#include "sys/port/gbic.h"
#include "switch/gbic.h"
#include "sys/fabos/mmap.h"
#include "sys/switch/port.h"

#define ISWIPCNAME					"ISWIPC"
#define ISWIPCINST					1024

/**
 * Description:
 *	Hex code to represent Powered state
 *	for Port module for GMODULE.
 *
 * Doxygen:
 *	@ingroup GMODULE2_API.
 */
#define MS_PMODULE_STATE_POWERED_ON	0xE56

/**
 * Description:
 *	Hex code to represent Port status
 *	for Port module for GMODULE.
 *
 * Doxygen:
 *	@ingroup GMODULE2_API.
 */
#define MS_PMODULE_STATUS_OK        0xE63

/**
 * Description:
 *	Max length for Port Name
 *	for GPORT4.
 *
 * Doxygen:
 *	@ingroup GPORT4_API.
 */
#define ISW_API_PORT_NAME_MAX		36

/**
 * Description:
 *	Max length for License name 
 *	for GSWITCH2.
 *
 * Doxygen:
 *	@ingroup GSWITCH2_API.
 */
#define ISW_RPC_LICENSE_LEN_MAX		64		

/**
 * Description:
 *	Max length for Serial number 
 *	for GSWITCH2.
 *
 * Doxygen:
 *	@ingroup GSWITCH2_API.
 */
#define ISW_MAX_SERIAL_NUMBER_LEN	64

/*
 * ISwitch IPC Callback codes
 */
typedef enum {
	CB_ISW_IPC,
	ISW_IPC_SEND_PAUSE,
	ISW_IPC_SEND_RESUME,
	CB_ISW_MAX_TYPE
} IswIpcCbCode;

/**
 * Description:
 *   Length of the reachable domain list, 32 bytes to
 *   represent 256 domains(only 239 is used).
 *
 * Doxygen:
 *	@ingroup MISC_FCR_API. 
 */
#define ISW_DOMAIN_LIST_LEN         32

enum {
	ISW_IPC_EXPORT,
	ISW_IPC_IMPORT,
	ISW_IPC_DEFAULT,
	ISW_IPC_GET_EX_CFG,
	ISW_IPC_SET_EX_CFG,
	ISW_IPC_GET_FAB_TAG,
	ISW_IPC_SET_FAB_TAG,
	ISW_IPC_GET_RES,
	ISW_IPC_GET_ROUTES,
	ISW_IPC_GET_DEV,
	ISW_IPC_GET_DEV_NODE,
	ISW_IPC_GET_DEVS,
	ISW_IPC_GET_GROUPS,
	ISW_IPC_GET_LSANS,
	ISW_IPC_GET_PPIDS,
	ISW_IPC_ADD_PPID,
	ISW_IPC_REM_PPID,
	ISW_IPC_GET_XLATES,
	ISW_IPC_ADD_XLATE,
	ISW_IPC_REM_XLATE,
	ISW_IPC_GET_PRIMARY,
	ISW_IPC_GET_VSTAMP,
	ISW_IPC_PORT_DEFAULT,
	ISW_IPC_CHDIR,
	ISW_IPC_GET_EV_FILTERS,
	ISW_IPC_SET_EV_FILTER,
	ISW_IPC_GET_ATTACHED_PN2,
	ISW_IPC_GET_GPORT4,
	ISW_IPC_GET_GPL,
	ISW_IPC_GET_GSWITCH,
	ISW_IPC_GET_GMODULE,
	ISW_IPC_LOGICAL_REACHABLE,
    ISW_IPC_GET_LASTERROR,
	ISW_IPC_GET_NBRWWN_INFO,
	ISW_IPC_GET_FCR_STATE,
	ISW_IPC_SET_FCR_STATE,
	ISW_IPC_SET_FCR_STATE_FORCE,
	ISW_IPC_GET_OUTPUT_PORT,
	ISW_IPC_GET_RPORT_COST,
	ISW_IPC_SET_RPORT_COST,
	ISW_IPC_GET_ACTIVE_FD,
	ISW_IPC_DEVS_LOGIN_CLR,
	ISW_IPC_GET_EDGEWWN_INFO,
	ISW_IPC_CFG_REMOVE,
	ISW_IPC_GET_LSAN_COUNT,
	ISW_IPC_SET_LSAN_COUNT,
	ISW_IPC_GET_LSAN_MATRIX,
	ISW_IPC_SET_LSAN_MATRIX_CACHE,
	ISW_IPC_SET_LSAN_MATRIX_ACTION,
	ISW_IPC_QUICK_LSAN_MATRIX_SETUP,
	ISW_IPC_VERIFY_LSAN_MATRIX_CACHE,
	ISW_IPC_GET_BCAST_CFG,
	ISW_IPC_SET_BCAST_CFG,
	ISW_IPC_LOG_LSDB,
	ISW_IPC_LOG_BCAST,
	ISW_IPC_XLATE_RDI_DONE,
	ISW_IPC_VALIDATE_FID_CONFLICT,
	ISW_IPC_GET_MAX_BASE_WWNS,
	ISW_IPC_GET_HA_AVAIL,
	ISW_IPC_SET_FCR_MATRIX_CACHE,
	ISW_IPC_SET_FABDEV_MATRIX_CACHE,
	ISW_IPC_SET_FCR_MATRIX_ACTION,
	ISW_IPC_VERIFY_FCR_MATRIX_CACHE,
	ISW_IPC_GET_FCR_MATRIX,
	ISW_IPC_QUICK_FCR_MATRIX_SETUP,
	ISW_IPC_SET_ALL_MATRIX_ACTION,
	ISW_IPC_SET_TI_FCR_DB,
	ISW_IPC_SET_IOD,
	ISW_IPC_SET_LSAN_TAG,
	ISW_IPC_GET_LSAN_TAG,
	ISW_IPC_GET_PERSISTXD_STATE,
	ISW_IPC_SET_PERSISTXD_STATE,
	ISW_IPC_GET_EX_CFG_ALL,
	ISW_IPC_MAX_TYPE
};

/**
 * Scope:
 *
 *  EX_Port Attributes. 
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GPSL_API.
 *	@ingroup GEXPARG_API.
 */
typedef struct _isw_ex_cfg {
	uint8_t		admin;
	uint16_t	fid;
	uint32_t	r_a_tov;
	uint32_t	e_d_tov;
	uint8_t		config_domain;
	uint8_t		wwn[8];
	uint8_t		p_domain;
	uint32_t	state;
	uint8_t		domain;
	uint32_t	type;
	uint8_t		p_wwn[8];
	uint8_t		mode;
	uint8_t		auto_elp;
	uint32_t	authn_attr;
	uint8_t		nbr_nwwn[8];
	uint8_t		nbr_pwwn[8];
	uint8_t		offset_mode;
} isw_ex_cfg;

typedef struct _isw_ex_cfg_groups {
	isw_ex_cfg ex_cfg;
	uint32_t    ex_port;
} isw_ex_cfg_groups;


/**
 * Scope:
 *
 *  FCR Back-bone resource Attributes. 
 *
 * Links:
 *	Contained within isw_ipc_resources.
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup MISC_FCR_API.
 */
typedef struct _isw_ipc_resources {
	int	cur_zone;
	int	cur_dev;
	int	cur_slot;
	int	cur_node_wwn;
	int	cur_wwn;
	int	cur_proxy[896];
	int	cur_router[896];
} isw_ipc_resources;

/**
 * Scope:
 *
 *  Back-bone Route Attributes. 
 *
 * Links:
 *	Contained within isw_ipc_routes.
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GFCRROUTE_API.
 */
typedef struct _isw_ipc_route {
	int			fid;	/* Fabric ID */
	int			pid;	/* Router Port ID */
	uint16_t		flag;	/* flag related to router port */
	uint16_t		cost;	/* Cost of IFL */
	char		wwn[8];	/* Principal WWN */
	char		nbr_name[64];	/* SwitchName of Attached Edge switch */
	uint32_t	nbr_ipaddr;		/* IP of Attached Edge switch */
	uint32_t	nbr_ipaddrV6[4];	/* IPV6 address of edge switch */
	char		nbr_wwn[8];		/* WWN of Attached Edge switch */
	int		port_index;
} isw_ipc_route;

/**
 * Scope:
 *
 *  Back-bone Routes Attributes. 
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GFCRROUTE_API.
 */
typedef struct _isw_ipc_routes {
	int				num;
	isw_ipc_route	route[1];
} isw_ipc_routes;

/**
 * Scope: 
 *
 *	Proxy Device attributes imported/exported by FCR.
 *
 * Links:
 *	Contained within isw_ipc_devs.
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GFCRPHYD_API.
 *	@ingroup GFCRPHYDL_API.
 *	@ingroup GFCRPHYDBN_API.
 *	@ingroup GAFCRPHYD_API.
 *	@ingroup GAFCRPHYDL_API.
 *	@ingroup GAFCRPHYDBN_API.
 */
typedef struct _fcrclidev_t {
	uint16_t	fid;
	char		wwn[8];
	uint16_t	rfid;
	uint32_t	rpid;
	uint32_t	ppid;
	uint32_t	state;
	uint32_t	type;
	char		node_wwn[8];
	uint16_t	login_try;
	uint16_t	login_lcheck_failure;
	uint16_t	login_rcheck_failure;
} fcrclidev_t;

/**
 * Scope: 
 *
 *	Proxy Devices attributes imported/exported by FCR.
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GFCRPHYD_API.
 *	@ingroup GFCRPHYDL_API.
 *	@ingroup GFCRPHYDBN_API.
 *	@ingroup GAFCRPHYD_API.
 *	@ingroup GAFCRPHYDL_API.
 *	@ingroup GAFCRPHYDBN_API.
 */
typedef struct _isw_ipc_devs {
	int			num;
	fcrclidev_t dev[1];
} isw_ipc_devs;


/**
 * Scope:
 *
 *  FCR Fabric Attributes. 
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup MISC_FCR_API.
 */
typedef struct _isw_ipc_group {
	int	fid;
	int	did;
} isw_ipc_group;

/**
 * Scope:
 *
 *  FCR Proxy PID Attributes.
 *
 * Links:
 *	Contained within isw_ipc_proxy_pids.
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup MISC_FCR_API.
 */
typedef struct _isw_ipc_proxy_pid {
	uint16_t	fid;
	uint8_t		wwn[8];
	uint32_t	id;
} isw_ipc_proxy_pid;

/**
 * Scope:
 *
 *  FCR Proxy PIDs Attributes.
 *
 * Links:
 *	Contained within isw_ipc_proxy_pids.
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup MISC_FCR_API.
 */
typedef struct _isw_ipc_proxy_pids {
	int				num;
	isw_ipc_proxy_pid	ppid[1];
} isw_ipc_proxy_pids;

/**
 * Scope: 
 *
 *	Xlate Domain attributes for fabric shared across FCR. 
 *
 * Links:
 * Contained within isw_ipc_xlates.
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GPSL_API.
 */
typedef struct _isw_ipc_xlate {
	uint16_t	e_fid;	/* Edge FID where xlate switch is projected */
	uint16_t	r_fid;	/* Remote FID, xlate switch is representing */
	uint8_t		c_did;	/* Configured Domain ID of xlate switch*/ 
	uint8_t		did;	/* Domain ID assigned to xlate switch */
	uint32_t	owner_did;	/* Front Domain ID owning Remote Fabric */
	uint8_t		wwn[8];		/* WWN of xlate switch */
} isw_ipc_xlate;

/**
 * Scope: 
 *
 *	Xlate Domains attributes for fabrics shared across FCR. 
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GPSL_API.
 */
typedef struct _isw_ipc_xlates {
	int				num;
	isw_ipc_xlate	xlate[1];
} isw_ipc_xlates;

/**
 * Scope:
 *
 *	LSAN zone attribute for fabric connected to FCR.
 *
 * Links:
 *	Contained within isw_ipc_lsanzones.
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GLSAN_API.
 *	@ingroup GALSAN_API.
 */
typedef struct _isw_ipc_lsanzone {
	char		name[64];		/* 64 comes from name_len_max */
	uint32_t	fid;
	char		wwn[8 * 200];	/* 200 comes from ISW_MAX_IDEVS_PER_IZ */
} isw_ipc_lsanzone;

/**
 * Scope:
 *
 *	LSAN zone attributes for fabrics connected to FCR.
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *	@ingroup GLSAN_API.
 *	@ingroup GALSAN_API.
 */
typedef struct _isw_ipc_lsanzones {
	int					num;
	isw_ipc_lsanzone	lsan[1];
} isw_ipc_lsanzones;

typedef struct _isw_ev_filter {
	int type;
	int show;
} isw_ev_filter;

/**
 * Descritption:
 *  WWN format for GAPNL2 information.
 *  @ref GAPNL2_API.
 *
 * Doxygen:
 *  @ingroup GAPNL2_API.
 *  @ingroup GPL2_API.
 *  @ingroup GPORT4_API.
 */
typedef u_char iwwn_t[8];

/**
 * Descritption:
 *  IP format for GSWITCH2.
 *
 * Doxygen:
 *  @ingroup GSWITCH2_API.
 */
typedef u_char idotIP[16];

/**
 * Scope:
 *
 *  Request for GAPNL2 information.
 *  @ref GAPNL2_API.
 *
 * Links:
 *
 *  Contained within isw_gapnl2_resp_list.
 *
 * Lifetime:
 *
 *  See @ref GAPNL2_API.
 *
 * Doxygen:
 *  @ingroup GAPNL2_API.
 */
typedef struct _isw_gapnl2_req_t {
	iwwn_t wwn_input_switch;
    iwwn_t wwn_port;
} isw_gapnl2_req_t;

/**
 * Scope:
 *
 *  GAPNL2_API
 *  @ref GAPNL2_API.
 *
 * Links:
 *
 *  Contained within isw_gapnl2_resp_list.
 *
 * Lifetime:
 *
 *  See @ref GAPNL2_API.
 *
 * Doxygen:
 *  @ingroup GAPNL2_API.
 */
typedef struct _isw_gapnl2_resp {
    iwwn_t wwn_output_switch;
    iwwn_t wwn_output_port;
    int port_number;
    int port_type;
} isw_gapnl2_resp;

/**
 * Scope:
 *
 *  GAPNL2_API
 *  @ref GAPNL2_API
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GAPNL2_API.
 */
typedef struct _isw_gapnl2_resp_list {
    u_int num;
    isw_gapnl2_resp gapnl2_resp_list_val[1];
} isw_gapnl2_resp_list;

/**
 * Scope:
 *
 *  GPORT4_API
 *  @ref GPORT4_API.
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GPORT4_API.
 */
typedef struct _isw_gport4_req_t {
    iwwn_t switch_wwn; /* used only for vswitch supported switches */
    iwwn_t port_wwn;
} isw_gport4_req_t;

/**
 * Scope:
 *
 *  GPORT4_API
 *  @ref GPORT4_API.
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GPORT4_API.
 */
typedef struct _isw_gport4_msg {
    char wwn[8];
    unsigned int port;
    unsigned int set_attribute;
    unsigned int port_state;
    unsigned int port_status;
    unsigned int port_phys_state;
    unsigned int port_type;
    unsigned int port_speed;
    unsigned int port_distance;
    unsigned int tx_link_route_cost;
    unsigned int gbic_type;
    char gbic_serial[16];
    char gbic_vendor[16];
    unsigned int cos;
    unsigned int pid;

    /* ver 2.0_API */
    unsigned int trunk_master;
    unsigned int qloopEnabled;

    /* ver 3.0_API */
    unsigned int port_cfg_flag;
    char port_name[ISW_API_PORT_NAME_MAX];

    /* Smart SFP attributes */
    int     port_SFP_Temp;
    int     port_SFP_Rxp;
    int     port_SFP_Txp;
    int     port_SFP_Current;

    /* just in case masks are different cross FOS */
    int nFirmwareVersion;

    /* raw port state flag can be used
    by host library to determine port state
    refer mask defined in port.h for p_flags
    to decode this flag */

    unsigned int uiPortFlag;

    /* port flag can be used to mask out
    segmentation reason fabric/fabric.h */

    unsigned int uiFabPortFlag;

    unsigned int trunk_deskew;
    unsigned int port_area;
    unsigned int port_user;
    char port_phys_location[16];
} isw_gport4_msg;

/**
 * Scope:
 *
 * Response structure for one FX_Port.
 *
 * Links:
 *	Contained within isw_gpl_fxport_list.
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GPL2_API.
 */
typedef struct _isw_gpl_fxport_entry {
    iwwn_t fxport_name;
    unsigned char reserved1;
    unsigned char fxport_module_type;
    unsigned char fxport_tx_type;
    unsigned char fxport_type;
} isw_gpl_fxport_entry;

/**
 * Scope:
 *
 * Response structure for one FX_Port.
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GPL2_API.
 */
typedef struct _isw_gpl_fxport_list {
    u_int num;
    isw_gpl_fxport_entry gpl_fxport_list_val[1];
} isw_gpl_fxport_list;

/**
 * Scope:
 *
 * License key.
 *
 * Links:
 *	Contained within isw_license_key_set.
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GSWITCH2_API.
 */
typedef struct _isw_license_key
{
    char    key[ISW_RPC_LICENSE_LEN_MAX];
} isw_license_key;

/**
 * Scope:
 *
 * License keys.
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GSWITCH2_API.
 */
typedef struct _isw_license_key_set {
    u_int license_key_set_len;
    isw_license_key license_key_set_val[1];
} isw_license_key_set;

/**
 * Scope:
 *
 * Product ID.
 *
 * Links:
 *	Contained within isw_gswitch_msg2.
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GSWITCH2_API.
 */
typedef struct _isw_product_id_set {
	u_int product_id_set_len;
    int product_id_set_val[1];
} isw_product_id_set;

/**
 * Scope:
 *
 * Response structure GSWITCH2.
 *
 * Links:
 *	isw_gswitch_msg2_ng.
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GSWITCH2_API.
 */
typedef struct _isw_gswitch_msg2 {
    unsigned int    ver;
    unsigned int    length;
    char            wwn[8];
    unsigned int    set_attribute;
    unsigned int    did;
    unsigned int    isPrincipal;
    char            ib_ip[16];
    char            ob_ip[16];
    char            ib_mask[16];
    char            ob_mask[16];
    char            gateway[16];
    unsigned int    isManagable;
    unsigned int    switch_state;
    unsigned int    switch_status;
    unsigned int    switch_id;
    unsigned int    switch_time;
    unsigned int    switch_tz;
    char            vendor_name[32];
    char            model[32];
    char            vsinfo[128];
    char            firmware_version[32];
    char            switch_name[32];
    unsigned int    isZoningInstalled;
    char            login_name[32];
    char            login_password[32];
    unsigned int    max_num_modules;
    unsigned int    port_per_module;
    unsigned int    port_count;

    unsigned int    start_slot;

	/**
	 * 0-not, 1 : s_32_1, 2:s_64_1
	 */
    unsigned int    isGrouped;

    isw_license_key_set licenseKeySet;
    isw_product_id_set  productIDSet;

	/**
	 *_API2 - zone
	 */
    unsigned int    HWZoningCapabilities;
    unsigned int    SWZoningCapabilities;
    unsigned int    CurrentZoningEnforcement;

    unsigned int    trunkingCapable;
    unsigned int    fwEventsEnable;
    unsigned int    EmbeddedSwitchType;
    char            EmbSwInvariantName[8];
    unsigned int    EmbSwSlotId;
    char            EmbSwBaseIPAddr[16];
    unsigned int    EmbSwBaseDomainId;
    unsigned int    EmbSwEportMode;
    char            EmbSwBoxSerialNumber[ISW_MAX_SERIAL_NUMBER_LEN];
    unsigned int    ValidCertificationInstalled;
    char            MfgSerialNumber[32];

	/**
     * quick loop support
	 * 0-disabled, 1-single, 2-dual
	 */
    unsigned int    qloopMode;
    char                    peerWwn[8];
    char                    portQlEnableBitMap[32];
    char                    setPortQlBitMask[32];
    char                    alpaBitMap[16];

	/**
	 *_API 1.0 missing attributes
	 */
    char            mfg_name[32];
    char            mfg_model[32];
} isw_gswitch_msg2;

/**
 * Scope:
 *
 * Response structure for GSWITCH2.
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GSWITCH2_API.
 */
typedef struct _isw_gswitch_msg_ng {
    isw_gswitch_msg2 gswitch_msg2;

	/**
     * NG2 MRD attributes
	 */
    u_short beaconEnable;
    u_int   upTime;
    char    bootUpReason[32];
    int     nAddressKnown;
    u_int   lastConfigChangeTime;
    u_short rlsServiceEnable;
    u_short portLogEnable;

    u_int   msCapability;
    char    domainName[60];
    char    nameServer[36];

	struct {
        u_int syslogIP_len;
        idotIP syslogIP_val;
    } syslogIP;

	char tsClockServer[4];
} isw_gswitch_msg_ng;


/**
 * Scope:
 *
 * Internal response structure for Port Module.
 *
 * Links:
 *	Contained within isw_slot_resp.
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GMODULE2_API.
 */
typedef struct _isw_portinfo {
    int slotNum;
    int slotState;
    int slotStatus;
    int minPort;
    int maxPort;
    char brcdPN[16];
    char brcdSN[16];
    char oemPN[24];
    char oemSN[24];
    char oemRev[8];
    char mfgLocation[24];
    char mfgDate[24];
	struct {
		u_int tempSensors_len;
        int tempSensors_val[1];
    } tempSensors;
} isw_portinfo;

/**
 * Scope:
 *
 * Response structure for Port Module.
 *
 * Links:
 *
 * Lifetime:
 *
 * Doxygen:
 *  @ingroup GMODULE2_API.
 */
typedef struct _isw_slot_resp {
    char wwn[8];
	struct {
        u_int slotInfo_len;
        isw_portinfo slotInfo_val[1];
    } slotInfo;
} isw_slot_resp;

/**
 * Scope:
 *
 * Response structure of edge neighbor wwn.
 *
 * Links:
 *
 */
typedef struct _isw_edge_nbrwwn_info_t {
	int domain;
	int fid;
	char name[68];
} isw_edge_nbrwwn_info_t;

/**
 * Scope:
 *
 * Response structure of edge neighbor swwn, pwwn
 *
 * Links:
 *
 */
typedef struct _isw_edge_wwn_info_t {
	int dom;
	int fid;
	wwn_t swwn;
	wwn_t pwwn;
} isw_edge_wwn_info_t;

/**
 * Scope:
 *
 * Request structure of FCR matrix
 *
 * Links:
 *
 */
typedef struct _isw_fcr_pair_matrix_ {
    wwn_t wwn1;
    wwn_t wwn2;
	int flag;
} isw_fcr_pair_matrix_t;

/**
 * Scope: 
 *
 *   List of fcr matrix pair	
 *
 * Links:
 *
 * Lifetime:
 *
 */
typedef struct _isw_fcr_pair_list_ {
	int			num;
	isw_fcr_pair_matrix_t entry[1];
} isw_fcr_pair_list_t;

/**
 * Scope:
 *
 * Request structure of fabric device matrix
 *
 * Links:
 *
 */
typedef struct _isw_fabdev_pair_matrix {
	int fabric1;
	int fabric2;
    wwn_t wwn1;
    wwn_t wwn2;
    int flag;
} isw_fabdev_pair_matrix_t ;

/*
 * Pause/Resume related declarations go here.
 */

typedef enum iswPauseRC {
        ISW_PAUSE_RC_SUCCESS,
        ISW_PAUSE_RC_TIMEOUT,
        ISW_PAUSE_RC_RETRY_EXCEEDED,
        ISW_PAUSE_RC_FAIL
} iswPauseRC_t;

/**
 * Scope:
 *
 * Structure to update the domain cost
 *
 * Links:
 *
 */
typedef struct _isw_domain_cost_t {
	int domain;
	int cost;
} isw_domain_cost_t;

extern int iswPause();
extern int iswIsHaAvailable();
extern char *iswExport(u_int *);
//extern cfgerr_t isw_cfg_upload(cfgupld_t *upld);
extern int   iswImport(char *, char *);
extern int   iswConfigDefault();
extern int   iswPortCfgDefault(int);
extern int   iswConfigRemove();
extern void  config_remove_fcr(int, int, int);

static inline  int   isw_get_ex_cfg(uint32_t port, isw_ex_cfg *pexCfg)
{
	return (0);
}
extern int   isw_set_ex_cfg(uint32_t, isw_ex_cfg *);
extern int   isw_get_fabric_tag(u_short *);
extern int   isw_set_fabric_tag_with_check(u_short);
extern int   isw_set_fabric_tag(u_short);
extern int   isw_get_fcr_state(u_short *);
extern int   isw_set_fcr_state(u_short);
extern int   isw_set_fcr_state_by_force(u_short);
extern int   isw_get_resources(isw_ipc_resources *);
extern int   isw_get_routes(isw_ipc_routes **);
extern int   isw_get_device(uint16_t, uint8_t *, fcrclidev_t *);
extern int   isw_get_devices(isw_ipc_devs **);
extern int   isw_devices_login_clr();
extern int   isw_get_groups(isw_ipc_group **);
extern int   isw_get_lsanzones(isw_ipc_lsanzones **);
extern int   isw_get_proxy_pid(isw_ipc_proxy_pids **);
extern int   isw_create_proxy_pid(uint16_t, uint8_t *, uint32_t);
extern int   isw_remove_proxy_pid(uint16_t, uint8_t *);
extern int   isw_get_xlate_attributes(uint16_t, uint16_t, isw_ipc_xlates **);
extern int   isw_create_xlate_attributes(uint16_t, uint16_t, uint8_t, int);
extern int   isw_remove_xlate_attributes(uint16_t, uint16_t);
extern int	 isw_get_primary_switch(int port, char *primaryWwn);
extern int   isw_get_vStamp(int port, char *vStamp);
extern int   isw_ipc_chdir(char *);
extern int   isw_get_events(isw_ev_filter **);
extern int   isw_set_event(isw_ev_filter *);
extern int   isw_get_attached_port_names_2(isw_gapnl2_req_t *, isw_gapnl2_resp_list **);
extern int   isw_get_GPort4(isw_gport4_req_t *, isw_gport4_msg *);
extern int   isw_get_Gpl(char *, isw_gpl_fxport_list **);
extern int   isw_get_GSwitch(char *, isw_gswitch_msg_ng *);
extern int   isw_get_GModule(char *, isw_slot_resp *);
extern int   isw_set_logical_reachable_domains(char *);
extern int   isw_get_err_string(int, char *, int);
extern int   isw_get_edge_nbrwwn_info(wwn_t *, isw_edge_nbrwwn_info_t *);
extern int   isw_get_edge_wwn_info(int port, isw_edge_wwn_info_t *);
extern int   isw_get_rport_cost(int pnum, int *cost);
extern int   isw_set_rport_cost(int pnum, int cost);
extern int   setLinkCostInternal(int port, u_int cost);
extern int   isw_get_active_fd(int port, int fabric, int *active_fd);
extern int   isw_get_lsan_count(int * count);
extern int   isw_set_lsan_count(int count);
extern int	 isw_get_lsan_matrix(int fabric, int *fabrics, int active_copy);
extern int	 isw_set_lsan_matrix_cache(int fabric1, int fabric2, int val);
extern int	 isw_set_lsan_matrix_action(int to_apply);
extern int   isw_set_quick_lsan_matrix(void);
extern int   isw_verify_lsan_matrix();
extern int   isw_set_fcr_matrix_cache(wwn_t wwn1, wwn_t wwn2, int flag);
extern int   isw_set_fcr_matrix_action(int flag);
extern int   isw_verify_fcr_matrix();
extern int   isw_get_fcr_matrix(int, isw_fcr_pair_list_t **);
extern int   isw_set_fabdev_matrix_cache(int, wwn_t, int, wwn_t, int);
extern int   isw_set_quick_fcr_matrix();
extern int   isw_set_all_matrix_action(int flag);
extern int   isw_log_lsdb();
extern int   isw_log_bcast();
extern int   isw_set_bcast_config(int fid, int opt);
extern int   isw_get_bcast_config(int *isw_bcast_cfg);
extern int   isw_ls_validate_fid_conflict(int fabric);
extern int   isw_get_max_base_wwns(int slot, int *max_base);
//extern int	fcrRefreshTIZone(sw_ti_t *elem, uint32_t num_elements);
extern int	 isw_set_iod(int value);
extern int   isw_set_lsan_tag(char *str, int val, int typ);
extern int   isw_get_lsan_tag(char **);
extern int   isw_get_persistxd_state(u_short *);
extern int   isw_set_persistxd_state(u_short);

#endif

/* vim:set ts=4 sw=4 noexpandtab tw=79: */
