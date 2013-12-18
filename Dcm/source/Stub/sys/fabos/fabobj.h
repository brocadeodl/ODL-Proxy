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

/***************************************************************************
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Kirthikaa                                                    *
 *   Description: This header file contains stub structures                *
 *                of FOS fabobj.h file                                     *
 ***************************************************************************/

#ifndef __SYS_FABOBJ_H__
#define __SYS_FABOBJ_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Reason codes includes:
 *  a) switch disable/enable reasons
 *  b) blade disable/enable reasons
 *  c) port disable/enable reasons
 *  d) slot off/on reasons
 *  e) HA recovery failure reasons
 * Also note that these reasons are because
 * (typically) of user commands
 * ALWAYS ADD new Reasons for port disable at the end.
 * ALSO make sure the macro R_PORT_DISAB_REASON_VALID
 * is updated to include the latest/max reason.
 */

typedef enum {
	R_RECOVER_FAIL = -1,    /* general recovery failure */
	R_INVALID_REASON = 0,   /* invalide reason */
	R_FORCED = 1,       /* forced to disable */
	R_SW_DISABLED,      /* Switch got disabled */
	R_BL_DISABLED,      /* Blade got disabled */
	R_SLOT_OFF,     /* Slot is turned OFF */
	R_SW_ENABLED,       /* Switch got enabled */
	R_BL_ENABLED,       /* Blade got enabled */
	R_SLOT_ON,      /* Slot turned ON */
	R_PERSISTID,        /* Port Disabled: persistant D_ID */
	R_SW_VIOLATION,     /* Port Disabled: Switch sec violation */

	/*
	 * Following reason codes are defined for online recovery.
	 * They represent the different priority by their order.
	 * Please don't change the order for following FOUR tokens.
	 */
	R_PORT_DATA_FAIL,   /* Online chip ASIC data fail */
	R_ONLINE_INCOMPLETE,    /* Online incompleted */
	R_ONLINE_ROUTE_FAIL,    /* Online route recovery failure */
	R_INCONSISTENT,     /* Port failed consistency check */

	R_SET_VCC_FAIL,     /* Set VCC Credits ioctl failed */
	R_ECP_IN_TESTING,   /* Recvd ECP in switchtesting state */
	R_ELP_IN_TESTING,   /* Recvd ELP in switchtesting state */
	R_ECP_RETRIES_EXCEEDED, /* ECP retries exceeeded */
	R_INVALID_ECP_STATE,    /* ECP retr in invalid state */
	R_BAD_ECP_RCVD,     /* ECP fctl len not valid */
	R_SEND_RTMARK_FAIL, /* Send Roundtrip mark ioctl failure */
	R_SEND_ECP_FAIL,    /* Send ECP failure */
	R_SAVE_LINK_RTT_FAIL,   /* Saving Link Roundtrip time failure */
	R_EM_INCNST,    /* EM has a different state than blade/PIC */
	R_EM_BLD_INCNST = R_EM_INCNST,  /* backward compatibility */
	R_PCI_ATTACH_FAIL,  /* PCI bus scan/attach failed */
	R_BUF_STARV,            /* Port Disabled: Due to buffer starvation */
	R_ELP_FCTL_MISMATCH,    /* Flow control mode different in ELP ACC */
	R_EPORT_DISABLED,   /* E-Port Disabled */
	R_TRUNK_WITH_VCXLT, /* Trunk and vcxltInit enabled */
	R_SW_FL_PORT_NOT_READY, /* SW not ready for F or L ports */
	R_LINK_REINIT,      /* Port Disabled: Due to too many link re-inits */
	R_DOMAIN_ID_CHANGE, /* Domain ID change */
	R_AUTH_REJECTED,    /* authentication is rejected */
	R_AUTH_TIMEOUT,     /* authentication timeout */
	R_AUTH_FAIL_RETRY,  /* fail to retry authentication */
	R_FCR_CONF_MISMATCH1, /* Brcd mode, MCDATA switch connected */
	R_FCR_CONF_MISMATCH2, /* MCDT mode, BRCD switch connected */
	R_FCR_PORT_LD_MODE_MISMATCH, /* Port is not configured LD */
	R_FCR_LD_CREDIT_MISMATCH, /* Port in LD mode, incoming ELP is R_RDY */
	R_FCR_SET_VCC_FAILED, /* Setting VCC failed for the port */
	R_FCR_SET_RTC_FAILED, /* Setting RT credit failed for the port */
	R_FCR_ELP_VER_INCONSIS, /* Version of ELP is inconsistent */
	R_FCR_ELP_FCTL_MISMATCH, /* Invalid flow control parameter */
	R_FCR_PID_MISMATCH, /* PID mismatch */
	R_FCR_TOV_MISMATCH, /* TOV mismatch */
	R_FCR_LD_INCOMPAT, /* Long distance incompatability */
	R_FCR_ISOLATED, /* Long distance incompatability */
	R_ELP_RETRIES_EXCEEDED, /* Exceeded max ELP retries */
	R_FCR_EXPORTS_EXCEEDED, /* FCR too many ex ports online */
	R_FCR_LICENSE,          /* FCR not licensed */
	R_FCR_CONF_EX,          /* FCR conflicting ex_port aleady online*/
	R_FCR_FTAG_OVER,        /* FCR Fabric ID oversubscribed */
	R_FCR_FTAG_CONFLICT,    /* FCR Fabric ID conflict */
	R_FCR_FOWNER_CONFLICT,  /* FCR Fabric Owner conflict */
	R_FCR_ZONE_RESOURCE,    /* FCR Exceeded max number of zone resources */
	R_FCR_PORT_STATE_TO,    /* FCR Failed to init in time */
	R_FCR_AUTHN_REJECT,     /* FCR Authentication Failure */
	R_FCR_SEC_FCS_LIST,     /* FCR Invalid member(s) in FCS list */
	R_FCR_SEC_FAILURE,      /* FCR Fail to join secure fabric */
	R_FCR_INCOMPATIBLE_MODE,    /* FCR Incompatible port mode */
	R_FCR_SEC_SCC_LIST,     /* FCR Invalid member(s) in SCC list */
	R_FCR_GENERIC,          /* FCR generic error (should not happen) */
	R_SW_EX_PORT_NOT_READY,
	R_FCR_CLASS_F_INCOMPAT, /* CLASS_F incompatability */
	R_FCR_CLASS_N_INCOMPAT, /* CLASS_N incompatability */
	R_FCR_INVALID_FLOW_RCVD, /* Invalid flow control recieved */
	R_FCR_STATE_DISABLED,
	R_FDD_STRICT_EXIST, /* Strict fabric wide policy, peer is non-ACL switch */
	R_LAST_PORT_DISABLE_MSG,
	R_SW_L_PORT_NOT_SUPPORT, /* Port does not support L-Port */
	R_PEER_PORT_IN_DI_ZONE, /* zone member of peer port is on pre-Quantum switch */
	R_ZONE_INCOMPAT,        /* neighbor cannot handle zone db and won't segment */
	R_SW_CONFIG_L_PORT_NOT_SUPPORT, /* port does not support configed L-port */
	R_SW_PORT_MIRROR_CONFIGURED,    /* Port Disabled: Due to Port Mirroring configured */

	/*
	 * Beginning of Access Gateway reason codes
	 *
	 * NOTE: Please don't add any reason code in the range of Access
	 * Gateway related reason code
	 */
	R_NPORTLOGIN_INPROGRESS,/* Port Disabled: N-port Login in progress */
	R_NONPIV,               /* Port Disabled: No NPIV support */
	R_NOMAPPING,            /* F-Port is not mapped to a N-Port */
	R_NPORTOFFLINE,         /* N-Port for this F-Port is Offline */
	R_FLOGIFAILED,          /* Fabric Login failed for port */
	R_NPORTBUSY,            /* N-Port is busy handling other login requests */
	R_NOFLOGI,              /* No FLOGI received on F-Port */
	R_NOFLOGIRESP,          /* No FLOGI response received on N-Port */
	R_FLOGIDUPALPA,         /* FDISC response received duplicate/zero AL_PA */
	R_LOOPCFG,              /* Detected loop configuration */
	R_NOENCLICENSE,         /* N_Port's configured FI_Port not licensed (ENC1&2) */
	R_NOFIPORTMAPPING,      /* N_Port has no configured FI_Port */
	R_BRCDFABCONN,          /* N_Port connected to Brocade Fabric - not supported on AGMace */
	R_PORT_RESET,           /* The port needs to be disabled and enabled */
	R_FLOGINPORT,           /* FLOGI received on port configured as N_Port */
	/* End of Access Gateway reason codes */

	R_FDD_STRICT_CONFLICT,      /* Fabric wide strict policy conflict */
	R_FDD_CFG_CONFLICT,     /* Fabric wide cfg conflict */
	R_FDD_CFG_CONFLICT_NA_NEIGH,    /* Fabric wide cfg conflict, neighbor is non-ACL switch */
	R_FCR_INSISTENT_FRONT_DID_MISMATCH, /* Port Disabled */
	R_FCR_FABRIC_BINDING_FAILURE,       /* Port Disabled */
	R_FCR_NON_STANDARD_DOMAIN_OFFSET, /* Port disabled: non-standard domain offset in ESC */
	R_AREA_IN_USE,          /* Area in use */
	R_MSTR_DIFF_PG,         /* Master has different port group than slave */
	R_MSTR_DIFF_AREA,       /* Master has different area than slave */
	R_TA_NOT_SUPPORTED,     /* Trunk area not supported */
	R_EPORT_NOT_SUPPORTED,  /* E_port not suppored on port type */
	R_FPORT_NOT_SUPPORTED,  /* F_port not suppored on port type */
	R_CFG_NOT_SUPPORTED,    /* Port Cfg not supported */
    R_PEER_VCS_CLUSTER_ADD_FAILURE, /* Peer switch fails to join the VCS cluster, segment it */
        
	/* Start of Fabric Watch reason codes */
	R_PORT_LL_TH_EXCEEDED,  /* Port class Link Loss threshold exceeded */
	R_PORT_SYNL_TH_EXCEEDED,/* Port class Sync Loss threshold exceeded */
	R_PORT_PE_TH_EXCEEDED,  /* Port class Protocol errors threshold exceeded */

	F_PORT_DISABLE_NO_TRK_LIC, /* No trunking license and F port trunk enabled */

	/* Additional Fabric Watch reason codes */
	R_PORT_INW_TH_EXCEEDED, /* Port class Invalid words threshold exceeded */
	R_PORT_CRC_TH_EXCEEDED, /* Port class Invalid CRC threshold exceeded */

	F_PORT_TR_DISABLE_SPEED_NOT_OK, /* Speed is not 2,4 or 8G */
	R_PORT_AUTO_DISABLE,        /* Disabled due to port auto disable */

	R_FCR_EXPORT_IN_NON_BASE_SW, /* Ex port is online in non base switch */
	R_BASE_SWITCH_SUPPORTS_NO_DEVICE,   /* base switch does not support device */
	R_PORT_TRUNK_PROTO_ERROR,   /* Invalid trunk state for AC_PORT */

	R_NO_AREA_AVAIL,        /* no more area is available for dynamic assignment */
	R_CANNOT_UNBIND_EXISTING_AREA,
	R_CANNOT_USE_10BIT_AREA,
	R_AUTHENTICATION_REQUIRED,  /* HBA Authentication required */

	/* Additional Fabric Watch reason codes */
	R_PORT_LR_TH_EXCEEDED, /* Port class Link Reset threshold exceeded */
	/* EX and LF conflict */
	R_FCR_EXPORT_LF_CONFLICT,
	/* Following reason codes are added for logical port segment disabled */
	R_INCOMPAT,             /* switch incompatible */
	R_DID_OVERLAP,          /* Domain id overlap */
	R_ZONE_CONFLICT,        /* Zone overlap */
	R_EPORT_SEG,            /* Generic Eport segment */
	R_NO_LICENSE,           /* No License */
	R_PLATFORM_DB,          /* incompatible platform DB */
	R_SEC_INCOMPAT,         /* Security incompatibility */
	R_SEC_VIOLATION,        /* Security Violation */
	R_ECP_LONGDIST,         /* ECP rejected or retries exceeded */
	R_DUP_WWN,              /* Duplication WWN in fabric */
	R_EPORT_ISOLATED,       /* Eport Isolated */
	R_AD,                   /* AD header conflict */
	R_ESC_DID_OFFSET,       /* Domain ID Offset conflict */
	R_ESC_ETIZ,         /* ESC Enhanced TI Zone conflict */
	R_ESC_FID,          /* ESC Fabric ID conflict */
	R_SAFE_ZONE,            /* Mcdata SafeZone conflict */
	R_VF,                   /* VF AD Conflict */
	R_VF_BS_INCOMPAT,       /* VF BS Conflict */
	R_PERS_PID_ON_LPORT,    /* Persistent PID isnt supported on L port */
	R_PERS_PID_PORTADDR_COLLISION,  /* Pers PID and port address binding collision */
	R_PERS_PID_PORT_ON_SAME_AREA,   /* Another port online with same area */
	R_PERS_PID_PORT_ADDR_BND,       /* Pers PID and port address binding collision,port addr bound */
	R_MSFR,                 /* MSFR/RD Conflict */
	R_SW_HALFBW_LIC,        /* Half bandwidth license only */
	R_1G_MODE_INCOMPAT,     /* The port is incompatible in the 1G mode */
	R_10G_MODE_INCOMPAT,    /* The port is incompatible in the 10G mode */
	R_DUAL_MODE_INCOMPAT,   /* The port is incompatible in the 10G mode */
	R_IMPLICT_PLT_SERVICE_BLOCK, /* Enabling of Platform Service Implicitly was blocked */
	R_PORT_ST_TH_EXCEEDED, /* Port class ST threshold exceeded */
	R_PORT_C3TXTO_TH_EXCEEDED, /* Port class C3 TX TO threshold exceeded */
	R_EPORT_NOT_SUPPORTED_DEF_SW, /* Uppermost 8 ports on Quattro blade on DCX on def-sw have no Eport supp */
	R_EPORT_LL_TH_EXCEEDED, /* ePort class Link Loss threshold exceeded */
	R_EPORT_SYNL_TH_EXCEEDED,/* ePort class Sync Loss threshold exceeded */
	R_EPORT_PE_TH_EXCEEDED,  /* ePort class Protocol errors threshold exceeded */
	R_EPORT_INW_TH_EXCEEDED, /* ePort class Invalid words threshold exceeded */
	R_EPORT_CRC_TH_EXCEEDED, /* ePort class Invalid CRC threshold exceeded */
	R_EPORT_LR_TH_EXCEEDED, /* ePort class Link Reset threshold exceeded */
	R_EPORT_ST_TH_EXCEEDED, /* ePort class ST threshold exceeded */
	R_EPORT_C3TXTO_TH_EXCEEDED, /* ePort class C3 TX TO threshold exceeded */
	R_FOPPORT_LL_TH_EXCEEDED, /* fopPort class Link Loss threshold exceeded */
	R_FOPPORT_SYNL_TH_EXCEEDED,/* fopPort class Sync Loss threshold exceeded */
	R_FOPPORT_PE_TH_EXCEEDED,  /* fopPort class Protocol errors threshold exceeded */
	R_FOPPORT_INW_TH_EXCEEDED, /* fopPort class Invalid words threshold exceeded */
	R_FOPPORT_CRC_TH_EXCEEDED, /* fopPort class Invalid CRC threshold exceeded */
	R_FOPPORT_LR_TH_EXCEEDED, /* fopPort class Link Reset threshold exceeded */
	R_FOPPORT_ST_TH_EXCEEDED, /* fopPort class ST threshold exceeded */
	R_FOPPORT_C3TXTO_TH_EXCEEDED, /* fopPort class C3 TX TO threshold exceeded */
	R_FCUPORT_LL_TH_EXCEEDED, /* fcuPort class Link Loss threshold exceeded */
	R_FCUPORT_SYNL_TH_EXCEEDED,/* fcuPort class Sync Loss threshold exceeded */
	R_FCUPORT_PE_TH_EXCEEDED,  /* fcuPort class Protocol errors threshold exceeded */
	R_FCUPORT_INW_TH_EXCEEDED, /* fcuPort class Invalid words threshold exceeded */
	R_FCUPORT_CRC_TH_EXCEEDED, /* fcuPort class Invalid CRC threshold exceeded */
	R_FCUPORT_LR_TH_EXCEEDED, /* fcuPort class Link Reset threshold exceeded */
	R_FCUPORT_ST_TH_EXCEEDED, /* fcuPort class ST threshold exceeded */
	R_FCUPORT_C3TXTO_TH_EXCEEDED, /* fcuPort class C3 TX TO threshold exceeded */
	R_PORT_NO_AREA_AVAIL_PERS_DISABLE, /* Out of area */
	R_EPORT_LOCKED,                 /* F-Port Disabled: Due to port is locked
	                                   as E-port */
	R_ENH_TIZONE,           /* Enhanced TI Zone Conflict */
	R_SW_PORT_SWAP_NOT_SUPPORTED, /* Port swap not supported in Quattro */
	R_FPORT_SLOW_DRAIN_CONDITION, /* F-Port is experiencing slow drain condition */
	R_ZONE_FDS,	/* Zoning conflict due to FDS configuration */
	/* End Logical port segment reason codes */

	R_PORT_DISABLE_LAST     /* Last port disable reason */
	/* Add new port disable reasons before R_PORT_DISABLE_LAST only. */
} reason_t;

#ifdef __cplusplus
}
#endif

#endif // __SYS_FABOBJ_H__
