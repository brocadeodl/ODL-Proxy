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
 *   Author : James Chen                                                   *
 *   Description:                                                          *
 *       This header file contains HASM related stub routines              *
 *       and structures                                                    *
 ***************************************************************************/

#ifndef __BCCB_HASM_HASM_H__
#define __BCCB_HASM_HASM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <hasm/smi.h>

#define MAX_NODES   (SMI_SLOTS * 2)

/*
 * HAM State definitions. CHANGING THE FOLLOWING BITS MAY BREAK THE
 * BACKWARD COMPATIBILITY.
 */
#define HAM_STATE_ACTIVE            0x00000001	/* active cp */
#define HAM_STATE_REDUNDANT         0x00000002	/* htbt up to peer */
#define HAM_STATE_SYNCHRONIZED      0x00000004
#define	HAM_STATE_SLOTBASED			0x00000020	/* slot-based chassis */
#define HAM_STATE_ENABLED           0x00000040
#define HAM_STATE_DUAL              0x00000080	/* dual ha node */
#define HAM_STATE_DUAL_CP           0x00000080	/* alias, dual cp */
#define HAM_STATE_DUAL_BP           0x00000080	/* alias, dual bp */
#define HAM_STATE_BP                0x00000200	/* port blade */
#define HAM_STATE_HEARTBEAT_UP      0x00000400	/* htbt up to peer */
#define HAM_STATE_SUPPRESS_HA       0x00000800
#define HAM_STATE_VERSION41         0x00008000
#define HAM_STATE_BOOTING           0x00100000
#define HAM_STATE_WARM_RECOVERY     0x00200000
#define HAM_STATE_ON_RECOVERING     0x00400000
#define HAM_STATE_STDBYCP_HEALTHY   0x08000000	/* updated by EM only */
#define HAM_STATE_UPGRADE       	0x00000010  /* on upgrade */
#define HAM_STATE_TAKEOVER      	0x00008000  /* allow take-over */
#define HAM_STATE_FAILOVER      	0x00000008  /* failover startup */



static inline int ham_get_cpid(void)
{
	return (0);
}

static inline int ham_get_peer_slotno(void)
{
	return (0);
}

static inline int ham_is_dual(void) 
{
	return (0);
}

static inline int ham_is_slotbased(void)
{
	return (0);
}

static inline int ham_is_active(void)
{
	return (0);
}

static inline int ham_get_state(void)
{
    return (HAM_STATE_ENABLED | HAM_STATE_ACTIVE | HAM_STATE_REDUNDANT | HAM_STATE_SYNCHRONIZED |
            HAM_STATE_DUAL_CP | HAM_STATE_HEARTBEAT_UP | HAM_STATE_STDBYCP_HEALTHY);
}

typedef	struct	{
	int		type;		/* node type, TYPE_CP/BP */
	union {
	    struct {
		int	cp_lnid;	/* local cp node id */
		int	cp_pnid;	/* peer cp node id */
		int	cp_lnip;	/* local cp ip */
		int	cp_pnip;	/* peer cp ip */
		int	cp_subnet;	/* cp subnet */
		int	cp_ha_lnip;	/* local ha ip */
		int	cp_ha_pnip;	/* peer ha ip */
	    } cp;
	    struct {
		int	bp_lnid;	/* local bp node id */
		int	bp_pnid;	/* peer bp node id */
		int	bp_lnip;	/* local bp node ip */
		int	bp_pnip;	/* peer bp node ip */
		int	cp_anid;	/* active cp node id */
		int	cp_anip;	/* active cp node ip */
		int	cp_swbd;	/* cp swbd# */
	    } bp;
	};
} nin_t;

static inline int ham_get_nin(nin_t *nin )
{
	return (0);
}

#define HAS_NSET_ALL        1
#define HAS_NSET_ACTIVE     2
#define HAS_NSET_BP_ALL     3
#define HAS_NSET_BP_ACTIVE  4
#define ON_UPGRADE(st)  ((st) & HAM_STATE_UPGRADE)
#define ON_TAKEOVER(st) ((st) & HAM_STATE_TAKEOVER)

static inline int has_get_nodeset(int *nodes, int set)
{
    nodes[0] = 0x0201;
    nodes[1] = 0x0202;
    nodes[2] = 0x0206;

    return (3);
}

static inline int ham_get_swbd(void)
{
    return (62);
}

static inline int get_cp_ha_peerip(void)
{
	return (0x0100007f);
}

static inline int get_cp_ha_localip(void)
{
    return (0x0100007f);
}

static inline int has_set_feature(char *feature, int val)
{
    return (0);
}


static inline int has_get_slport(int slot, int a, int b, int bdid, int prot, pbmp_t *pbmp)
{
    int i;

    PBMP_ZERO(pbmp);
    PBMP_SETPROT(pbmp, prot);

    for (i = 0; i < 64; i++) {
        PBMP_SET(pbmp, i);
    }

    return (a);
}

static inline int has_tell_status(int slot, int status)
{
	return (0);
}

static inline int on_upgrade()
{
    return (0);
}

#define HAS_CONFIG_COMPLETE 3
#define HAS_CLUSTER_READY   4
#define HAS_CLUSTER_UNREADY 5

#ifdef __cplusplus
}
#endif

#endif

