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

/*****************************************************************************
 *
 * Copyright (c) 1996-2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 *
 * public.h : AG Stub code for DCMd compilation
 *
 ****************************************************************************/
#ifndef _AGD_PUBLIC_H
#define _AGD_PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fc/wwn.h"
#include "fabric/getid.h"
#include "fabos/license.h"

/* Please set the following macros as desired for the x86 testing */

/* Set this to 1 or 0 to enable/disable AG mode in x86 environment */
#define STUB_AG_MODE_ENABLED   0

/* Max number of user ports on the platform. On Callisto-F we got 24 TE ports, 24 ISLs and 8 FC ports */ 
#define MAX_PHY_PORT   56
#define MAX_FI_PORT    0
#define FCOE_DB_DEFAULT_NUM_AG_VX_PORTS 24 /* Change to 60 for CF-60 */
#define MAX_AG_PORT 	(MAX_PHY_PORT + MAX_FI_PORT + FCOE_DB_DEFAULT_NUM_AG_VX_PORTS)
#define AG_MAX_PORTS    MAX_AG_PORT
#define AGD_MAX_FPORTS	MAX_AG_PORT


#define MYSWITCH 1
#define FOS_AG_FNAME		"/etc/fabos/ag_platform"
#define BITMAP_STR_LEN	(11*NUM_WORDS(MAX_AG_PORT))

#define MAX_PGID        (MAX_AG_PORT - 1)
#define PGNAME_LEN 64
#define PGMODE_LEN 64

#define AG_PGDFLT_MODE      0
#define AG_PGFLG_LB         0x00000001  /* login Balancing */
#define AG_PGFLG_MFNM       0x00000002  /* Managed Fabric Name Monitoring */
#define AG_PGFLG_NPORT_UP_NO_REBAL 0x00000004 /* rebalance when N-port comes up */
#define AG_PGFLG_LB_AB_F    0x00000008  /* Rebalance on F_Port event */
#define AG_PGFLG_LB_AB_N    0x00000010  /* Rebalance on N_Port event */

#define AG_PGFLG_LB_NAME   "lb"
#define AG_PGFLG_MFNM_NAME "mfnm"
#define AG_PGFLG_NPORT_UP_NO_REBAL_NAME "nunr"


/** Bit manipulation function for policies */
#define AG_POLICY_IS_SET(bm, policy) \
        (bm & policy)
#define AG_POLICY_SET_BIT(bm, policy) \
        (bm |= policy)
#define AG_POLICY_CLEAR_BIT(bm, policy) \
        (bm &= ~policy)
#define AG_POLICY_CLEAR_BITMAP(bm) memset(&(bm), 0, sizeof(bm))

#define  pwwn_list agd_adslist_t


typedef enum {
        AG_POLICY_START_BIT = 0,
        AG_POLICY_PG_BIT = AG_POLICY_START_BIT,
        AG_POLICY_AUTO_BIT = 1,
        AG_POLICY_ADS_BIT = 2,
        AG_POLICY_MAX_BIT = 3
} agd_policy_bits;

typedef enum {
        AG_POLICY_START = 1<<0,
        AG_POLICY_PG = AG_POLICY_START,   //port grouping
        AG_POLICY_AUTO = 1<<1, //automatic port type detection & configuration
        AG_POLICY_ADS = 1<<2, //automatic port type detection & configuration
        AG_POLICY_MAX = 1<< 3
} agd_policy_t;

enum {
	AGMODE_SUCCESS = 0,  /* agmode changed successfully */
	AGMODE_ENOCHG = -1, /* agmode is already set to requested value */
	AGMODE_EFAILED = -2, /* agmode change failed due to unknown error */
	AGMODE_ESWONLINE = -3, /* agmode change failed since switch is in ONLINE
							  state */
	AGMODE_ESECMODE = -4, /* agmode change failed since switch is in secure
							 mode */
	AGMODE_ENOSYS = -5, /* agmode change failed since switch hardware
						  doesn't support AG mode */
	AGMODE_EZNDCLR = -6, /* Failed to clear Zoning/AD configuration */
	AGMODE_EMSPLDB = -7, /* Management Server platform service is not
			       disabled */
	AGMODE_EPODLIC = -8,  /* POD1 or POD2 license is not installed on 200E */
	AG_EPGFAILED = -9, /* default error value for port group operation */
	AG_EMSGFAILED = -10, /* default error value for port group operation */
	AG_ENOMEM = -12, /* No memory,  argument */
	AG_EPOLICY = -13, /* COnflicts with some existing policy */
	AG_EPREF = -14,	/* conflicts with preferred settings */
	AG_EPRINOTSET = -15,	/* Failed to set preferred N_Port
				   as F_Port is not mapped  */
	AG_EPRISAME = -16,	/* Failed to set preferred N_Port as
				   Primary and Preferred N_Port are same */ 	
	AG_EPGACROSS = -17, /* Preferred N_Port failed to set as preferred
			       N_Port is not in same PG*/
	AG_EPREFMAPPED = -18, /* All the F_ports have the specified preferred */
						/* N_Port already configured */
	AG_EPGEXISTS = -19, /* POrt Group already exists */
	AG_EPGPOLICYNOTSET = -20, /*PG Policy is not set */
	AG_EPGDOESNTEXIST = -21, /* Port group doesn't exist */
	AG_EPGINVALIDPGID = -22, /* Invalid pgid */
	AG_EINVALIDFPORTLIST = -23, /*F_Port not mapped to previous N_Port */
	AG_EFPORTNOTMAPPED = -24, /*F_port not mapped to the N_Port */
	AG_EPREFPORTSET = -25, /* Preferred N_port is set */
	AG_EDEFPGRENAME = -26,
	AG_EINVAL = -27, /* Invalid argument */
	AGMODE_EINTEROP = -28, /* switch is in interop mode */
	AGMODE_ENODISABLE = -29,  /* agmode disable is not supported on
				     superhawk */
	AGMODE_ETAASSIGNED = -30, /*AG mode enable is not allowed if TA is
				   assigned to any of the port */	
	AG_EPGNOTLB = -31, /* Port group is not in login balancing mode */
	AG_EPGMODESET = -32, /* Port group mode already set */
	AG_EPGMODENOTSET = -33, /* Port group mode not set */
	AG_EAUTOPOLICYSET = -34, /* Auto policy is enabled */
	AG_EFPORTMAPPED = -35, /*F_port not mapped to the N_Port */
	AG_EPORTNOTFPORT = -36, /* Given port is not F-port */
	AG_EINVALPORTNUM = -37, /* Invalid port number */
	AG_ENOTAGMODE = -38, /* Not in AG mode */
	AG_EPORTNOTNPORT = -39, /* Given port is not N-port */
        AG_ENPORTINDIFFPG = -40, /* The N-port belongs to another PG*/
        AG_EREMFAILED = -41, /* N-port remove from pg operation failed */
        AG_EPGINFORETRFAIL = -42, /* PG info retrieval failed */
        AG_EADDFAILED = -43, /* N-port add to pg operation failed */
        AG_EMODEALREADYSET = -44, /* Mode to set has already been set */
        AG_EMODEALREADYUNSET = -45, /* Mode to unset has already been unset */
        AG_ENOCFGCHANGE = -46, /* no change in configuration applied */
        AG_EMEMBERFTRUNK = -47, /* Trunk members, can not move individual port of the trunk */
        AG_EAUTOPOLICYNOTSET = -48,  /*Auto policy not enabled */
        AG_ENOTPORTCFGNPORT = -49  /* Port type cannot be changed by user */
};

#define MAX_AG_POLICIES 8
#define MAX_AG_POLICY_NAME 50
#define MAX_AG_POLICYFLAG_NAME 20
#define AG_DEFAULT_PREFNPORT UCHAR_MAX
#define AG_DEFAULT_PGID UCHAR_MAX
#define AG_DEFAULT_POLICY AG_POLICY_PG


typedef unsigned long * BitMap_t;

/*
 * a set of definitions macros (prefixed with BM, Bit Manipulation or Bit Map)
 * to manipulate an arbitrary size array of bits.
 */
#define BM_NBBY 8                                       /* # bits per byte */
#define BM_NBPF (sizeof (unsigned long) * BM_NBBY)      /* # bits per field */
/* set bit 'n' in the given byte array pointed by 'p' */
#define BM_SetBitN(n, p)        ((p)[(n)/BM_NBPF] |= \
                         ((unsigned)(1 << ((n) % BM_NBPF))))
/* clear bit 'n' in the given byte array pointed by 'p' */
#define BM_ClrBitN(n, p)        ((p)[(n)/BM_NBPF] &= \
        ~((unsigned)(1 << ((n) % BM_NBPF))))
/* check if bit 'n' in the given byte array pointed by 'p' is set */
#define BM_isBitNSet(n, p)      ((p)[(n)/BM_NBPF] & \
                                 ((unsigned)(1 << ((n) % BM_NBPF))))


#define BM_ARRAY_SIZE(num_bits) (((num_bits) % BM_NBPF) ? \
                                                (((num_bits) / BM_NBPF) + 1) : \
                                                ((num_bits) / BM_NBPF))
#define SIZEOF_BM(num_bits) (sizeof (u_long) *\
                                                        BM_ARRAY_SIZE(num_bits))

#define NUM_WORDS(num_bits) ((sizeof (u_long) *\
                                                        BM_ARRAY_SIZE(num_bits)) / 4)

#define BM_Malloc(num_bits) ((BitMap_t)malloc(SIZEOF_BM(num_bits)))
#define BM_Calloc(num, num_bits) ((BitMap_t)calloc(num, \
                                                        SIZEOF_BM(num_bits)))
#define BM_Clear(x, num_bits) memset((void *)x, 0,\
                                                        SIZEOF_BM(num_bits))
#define BM_Copy(x, y, num_bits) memcpy((void *)x, (void *)y,\
                                                        SIZEOF_BM(num_bits))
#define BM_Equal(x, y, num_bits) memcmp((void *)x, (void *)y,\
                                                        SIZEOF_BM(num_bits))
#define BM_And(rOut, pIn, num_bits) \
do { \
        size_t          arrSz;  \
        unsigned long    bitMask = ~0;   \
        unsigned int    ii;     \
        \
        arrSz = BM_ARRAY_SIZE(num_bits);        \
        \
        /* Merge array except for last index */ \
        for (ii = 0; ii < (arrSz - 1); ii++)    \
                rOut[ii] &= pIn[ii];    \
        \
        /* Merge the last index of array up to the specified bit */     \
        bitMask >>= ((num_bits) % BM_NBPF) ? BM_NBPF - ((num_bits) % BM_NBPF):0; \
        rOut[arrSz - 1] &= pIn[arrSz -1] & bitMask;     \
} while (0)

#define BM_Invert(rOut, pIn, num_bits) \
do { \
        size_t          arrSz;  \
        unsigned long    bitMask = ~0;   \
        unsigned int    ii;     \
        \
        arrSz = BM_ARRAY_SIZE(num_bits);        \
        \
        /* Merge array except for last index */ \
        for (ii = 0; ii < (arrSz - 1); ii++)    \
                rOut[ii] = ~pIn[ii];    \
        \
        /* Merge the last index of array up to the specified bit */     \
        bitMask >>= ((num_bits) % BM_NBPF) ? BM_NBPF - ((num_bits) % BM_NBPF):0; \
        rOut[arrSz - 1] = ~pIn[arrSz -1] & bitMask;     \
} while (0)


static inline int portNportEnabled(int);
static inline int 
portNportEnabled(int swport)
{
	return (STUB_AG_MODE_ENABLED);
}

static inline int agd_is_accessgateway(void);
static inline int
agd_is_accessgateway(void)
{
	return (STUB_AG_MODE_ENABLED);
}

typedef enum {
    PG_FNM_IDLE = 0x0,
    PG_FNM_ERROR,
    PG_FNM_HANDLED
} fnm_state_t;

typedef enum {
    AG_ALL_ACCESS,  /* Allow all the devices to login */
    AG_NO_ACCESS,   /* Allow none of the devices to login */
    AG_LIST_ACCESS  /* Allow the devices in ADS table to login */
}  ag_access_t;

typedef struct {          // Per policy info
    int status;
    char policyname[MAX_AG_POLICY_NAME];
    char policyflag[MAX_AG_POLICYFLAG_NAME];
} agd_perpolicyinfo_t;

typedef struct {
    int num_policies;
    agd_perpolicyinfo_t policyinfo[MAX_AG_POLICIES];
} agd_policystatus_t;

#ifdef __cplusplus
}
#endif

#endif /* _AGD_PUBLIC_H */
