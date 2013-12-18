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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef __ESS_H__
#define __ESS_H__

#include "fabric/getid.h"
#include "sys/fablog/fabmod.h"
#include "sys/fc/fcsw.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define ESS_ENOTSUPPORTED	-1002

/* ESS Well Known Addresses: represent the type of service that a
 * capability object represents.
 */
typedef enum essWka {
	PAUSE_SUPPORT		= 0x01,
	FAB_OP_SUPPORT		= 0x20,
	VENDOR_SPECIFIC		= 0xE0,
	KEY_SERVER		= 0xF7,
	QOS_FACILITATOR		= 0xF9,
	MGMT_SERVER		= 0xFA,
	DIR_SERVER		= 0xFC,
	FABRIC_CTRL		= 0xFD,
	LOGIN_SERVER		= 0xFE,
	VENDOR_UNIQUE   = 0x01 /* McData Vendor Unique capabilities */
} essWka_t;

/* ESS Well Known Address Subtypes: represent the component providing
 * a capability. Check the values of ESS_FAB and ESS_PSD.
 * NOTE !!! no new subtupes can be greater than 0x31, otherwise it will
 * cause 3.2 assert and 4.4 VERIFY
 */
typedef enum essWkas {
	ESS_PAUSE		= 0x01,
	ESS_NS			= 0x02,
	ESS_ZONE		= 0x03,
	ESS_ACL			= 0x10, /* Access Control List */
	ESS_ISCSI		= 0x11,
	ESS_AD			= 0x12, /* Admin Domain */
	ESS_ISNSC		= 0x1A,
    ESS_FCOE        = 0x26, /* FCoE daemon */
	ESS_SEC			= 0x27, /* security daemon, temporary value */
	ESS_CAL			= 0x28, /* cal support */
	ESS_RCS			= 0x29, /* RCS */
	ESS_NOS			= 0x2A, /* Brocade Network OS related capabilities */
	ESS_FAB			= 0x30,	/* fabric daemon */
	ESS_PSD			= 0x31,	/* performance monitor */
	ESS_PR          = 0x01, /* pause-restart */
	ESS_MCDT_MISC   = 0x02 /* McDT Misc capabilities */
} essWkas_t;


/* ESS Brocade specific types */
typedef enum essVendInfo {
	ESS_ENTRY_NONE		= 0,

	/* ESS_NS WKAS entries */
	ESS_NS_START			= ESS_ENTRY_NONE,
	ESS_NS_BCAP				= 3,
	ESS_NS_DUMMY			= 6,
	ESS_NS_LAST_ENTRY = ESS_NS_DUMMY,

	/* ESS_ZONE WKAS entries */
	ESS_ZONE_START			= ESS_ENTRY_NONE,
	ESS_ZN_MAX_PORT			= 1,
	ESS_ZN_MAX_CFG_SIZE		= 2,
	ESS_ZN_RCS_ENABLE		= 5,
	ESS_ZN_MAX_CFG_BUF_SIZE	= 8,
	ESS_AD_CAP				= 18,
	ESS_BCAST_CAP   		= 28,
	ESS_IPV6_CAP   			= 29,
	ESS_ZN_MISC_SUPPORT		= 43,
	ESS_ZONE_LAST_ENTRY = ESS_ZN_MISC_SUPPORT,

	/* ESS_ACL WKAS entries */
	ESS_ACL_START			= ESS_ENTRY_NONE,
	ESS_ACL_SCC_MD5_HASH	= 14,
	ESS_ACL_DCC_MD5_HASH	= 15,
	ESS_ACL_PWD_MD5_HASH	= 16,
	ESS_ACL_VERSION			= 20,
	ESS_ACL_FAB_MD5_HASH	= 21,
	ESS_ACL_SCC_HASH_PRSNT	= 22,
	ESS_ACL_DCC_HASH_PRSNT	= 23,
	ESS_ACL_PWD_HASH_PRSNT	= 24,
	ESS_ACL_FAB_HASH_PRSNT	= 25,
	ESS_ACL_FCS_HASH_PRSNT	= 30,
	ESS_ACL_FCS_MD5_HASH	= 31,
	ESS_ACL_MISC_SUPPORT	= 42,
	ESS_ACL_LAST_ENTRY = ESS_ACL_MISC_SUPPORT,

	/* ESS_ISCSI WKAS entries */
	ESS_ISCSI_START			= ESS_ENTRY_NONE,
	ESS_ISCSI_ENABLED		= 17,
	ESS_ISCSI_LAST_ENTRY = ESS_ISCSI_ENABLED,

	/* ESS_AD WKAS entries */
	ESS_AD_START			= ESS_ENTRY_NONE,
	ESS_AD_HDR_MD5_HASH		= 19,
	ESS_AD_LAST_ENTRY = ESS_AD_HDR_MD5_HASH,

	/* ESS_ISNSC WKAS entries */
	ESS_ISNSC_START			= ESS_ENTRY_NONE,
	ESS_ISNSC_ENABLED		= 26,
	ESS_ISNSC_LAST_ENTRY = ESS_ISNSC_ENABLED,

	/* ESS_FCOE WKAS entries */
	ESS_FCOE_START			= ESS_ENTRY_NONE,
	ESS_FCOE_CAPS			= 32,
	ESS_FCOE_LAST_ENTRY = ESS_FCOE_CAPS,

	/* ESS_SEC WKAS entries */
	ESS_SEC_START			= ESS_ENTRY_NONE,
	ESS_SEC_MAX_POLICY_SZ	= 9,
	ESS_SEC_LAST_ENTRY = ESS_SEC_MAX_POLICY_SZ,

	/* ESS_CAL WKAS entries */
	ESS_CAL_START			= ESS_ENTRY_NONE,
	ESS_CAL_ENABLED			= 10,
	ESS_CAL_LAST_ENTRY = ESS_CAL_ENABLED,

	/* ESS_RCS WKAS entries */
	ESS_RCS_START			= ESS_ENTRY_NONE,
	ESS_RCS_ENABLED			= 11,
	ESS_RCS_CAPABILITIES	= 12,
	ESS_RCS_VERSION			= 13,
	ESS_RCS_LAST_ENTRY 		= ESS_RCS_VERSION,

	/* ESS_NOS WKAS entries */
	ESS_NOS_START			= ESS_ENTRY_NONE,
	ESS_NOS_MISC_CAPS		= 1,
	ESS_NOS_LAST_ENTRY 		= ESS_NOS_MISC_CAPS,

	/* ESS_FAB WKAS entries */
	ESS_FAB_START			= ESS_ENTRY_NONE,
	ESS_FAB_INQ				= 4,
	ESS_FAB_MAX_IU_SZ		= 7,
	ESS_FAB_INQ2			= 27,
	ESS_FAB_VF_MODE			= 32,
	ESS_FAB_VF_FID			= 33,
	ESS_FAB_SW_MODE			= 34,
	ESS_FAB_CS_WWN			= 35,
	ESS_FAB_CS_NAME			= 36,
	ESS_FAB_LOG_SW_CNT		= 37,
	ESS_FAB_MAX_LOG_SW		= 38,
	ESS_FAB_VF_SUPRTD		= 39,
	ESS_FAB_MAX_PHY_PORT	= 40,
	ESS_FAB_FICON_MISC		= 41,
	ESS_FAB_PHY_PORT_RANGE	= 44,
	ESS_FAB_LAST_ENTRY 		= ESS_FAB_PHY_PORT_RANGE,

	/* For now just equate this to whichever is highest number */
	ESS_LAST_VEND_INFO 		= ESS_FAB_LAST_ENTRY
} essVendInfo_t;

static inline int
essGetMaxPhyPorts(int dom, int *maximum_phy_ports,
       module_t *mod)
{
    if (dom == 7) {
        *maximum_phy_ports = 120;
    } else {
        *maximum_phy_ports = 48;
    }

	return (0);
}

static inline
int essIsFabDistSvcSupported(module_t *mod)
{
    return 1;
}

static inline
int essGetIsCOSDomain(int dom, int *is_Cos, module_t *mod)
{
    *is_Cos = true;

    return (0);
}

static inline
int	essQueryVendUniqCap(int domain, essWka_t wkaType,
			essWkas_t wkaSubType, essVendInfo_t attr, void *buf,
			int *leng, module_t *mod)
{
	return (0);
}

static inline
int essIsDomainXlatePhantom(int domain, module_t *mod)
{
	return (0);
}


#ifdef __cplusplus
}
#endif

#endif // __ESS_H__
