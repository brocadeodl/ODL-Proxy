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

/*
 *    Copyright (c) 1996-2008 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Module name: fabcfg.h
 *
 *    Description:
 *        This file contains the generic Fabos Configuration definitions
 *        adapted for use in the DCM environment
 */


#ifndef __SYS_FABCFG_H__
#define	__SYS_FABCFG_H__
#ifdef __cplusplus
extern "C"
{
#endif


#include <fc/wwn.h>

/*
 * Port configurations: data structure and bitmap access macros
 * If you want to add more bits here, add them at the end.
 * Change distance to 4 bits from 2 bits since we could possibly
 * have upto PORT_DISTANCE_SIZE (16 now) long dist levels.
 */
typedef struct _pcfg_bm {
	unsigned int old_distance : 2;	/* old long distance config. bits */
	unsigned int gport_lockdown : 1;	/* lock down a port as a GPORT  */
	unsigned int tport : 1;		/* trunking enable/disable */
	unsigned int speed : 3;		/* TX & RX speed mode */
	unsigned int fcacc : 1;	/* FC Acceleration configuration */
	unsigned int disable_eport : 1;	/* Disable E_Port */
	unsigned int lport_lockdown : 1;	/* lock down a port as an LPORT */
	unsigned int priv_lport_lockdown : 1; /* lock down as an private LPORT */
	unsigned int vcxlt_linit : 1;	/* enable vc xlt link init. protocol */
	unsigned int delay_flogi : 3;	/* delay flogi acc until route done */
	unsigned int isl_interop : 1;	/* ISL Interop Mode */
	/*
	 * 1 = persistently disable the port.
	 * 0 = enabled
	 */
	unsigned int _pvt_port_persistently_disabled : 1;
	unsigned int distance: 4;		/* Long dist config bits */
	unsigned int BufStarvFlag: 1;	/* =1 if port is offline due to insufficient buffers */
	unsigned int lport_halfduplex: 1;	/* =1 if lport is half-duplex mode */
	unsigned int lport_fairness: 1; /* =1 if lport is full-duplex fairness mode */
	unsigned int credit_sharing: 1;	/* credit sharing on */
	unsigned int _pvt_port_permanently_disabled : 1; /* overrides persist enable */
	/*
	 * An additional bit npiv_configured is used
	 * to qualify the npiv_enabled bit as shown
	 * below because the default value for BLOOM
	 * is to disable NPIV versus enable by default
	 * for Condor and later chips.
	 *
	 * npiv_configured	npiv_enabled
	 * ---------------	------------
	 *	0			X	Assume default value based
	 *					on the port capability.
	 *	1			0	NPIV explicitly disabled.
	 *	1			1	NPIV explicitly enabled.
	 */
	unsigned int npiv_enabled: 1;	/* N-port ID Virtualization enabled */
	unsigned int npiv_configured: 1;	/* explicitly configured as NPIV port */
	unsigned int rscn_suppress: 1;      /* rscn suppress on/off */
	unsigned int ex_port: 1;				/* is EX-Port */
	unsigned int mirror_port: 1;		/* is Mirror Port */
	unsigned int soft_neg: 1;    		/* If soft_neg set to 1, the Auto-negotiation
									   state machine is completely Software controlled
									   Def:  # 64791 */
    unsigned int qos_enabled: 1;		/* Quality of Service (QOS) enabled */
	unsigned int qos_configured: 1;		/* explicitly configured as QOS port */
	unsigned int rlimit: 4; 			/* Rate Limit settings */
    unsigned int alpa13: 1; 			/* AL_PA offset: 0 for 0x0, 1 for 0x13 */
	unsigned int export_mode: 3; 		/* Interopmode for EXports, int value allows up to 8 modes */
	unsigned int losig:1;				/* For loss of signal config to Condor */
	unsigned int asn_frc_hwretry:1;		/* Auto-neg BUT force HWSN to retry even if success (86791 fix) */
	unsigned int cr_recov:1;			/* For credit recovery */
	unsigned int auto_disable:1;		/* for port auto disable */
	unsigned int fport_buffers:1;		/* for configuring fport buffers */
	unsigned int fill_word:2;			/* use ARB-ARB, IDLE-IDLE, or IDLE-ARB */
	unsigned int eport_lockdown:1;		/* To configure a port as E-port only */
} pcfg_bm_t;

/*
 * GE port config bitmap with 32-bits.
 */
#define	GE_PCFG_MASK_DISABLE	0x0001	/* b0: persistent disable */
#define	GE_PCFG_MASK_MEDIA		0x0002	/* b1: media copper */
#define	GE_PCFG_MASK_10G_MODE	0x0004 /* b2: mode 10G */
#define	GE_PCFG_MASK_ALL		-1	/* all bits */

typedef	union	{
	unsigned int	u32;			/* 32-bit integer as a whole */
	struct {
	    unsigned int	reserved: 29;		/* reserved */
		unsigned int	mode:1;			/* 0: 1G, 1:10G */
	    unsigned int	media: 1;		/* 0: optic, 1: copper */
	    unsigned int	disable: 1;		/* 0: enable, 1: disable */

	} prop;
} ge_pcfg_bm_t;

#define PORT_CFG_ENTRY_SZ       36  /* entry size per port cfg string */

/*
 * The private configuration bitmap will have the following defintions.
 * TBD: the BufStarvFlag will be moved out of pcfg_bm_t later and stored
 * int the private bitmap instead.
 */
#define priv_pcfg_t		u_int 	/* private configutation bitmap */

#define	BUF_STARV_FLAG	0x1	/* if port is offline due to insufficient buffers */
#define BUF_DEGR_FLAG	0x2	/* if port is degraded due to lack of bufs */

typedef enum {
	WR_PRIV_CFG = 0,	/* Clear the whole priv conf bitmap */
	SET_PRIV_CFG,		/* Set a bit to the priv conf bitmap */
	CLR_PRIV_CFG		/* Clear a bit in the priv conf bitmap */
} priv_pcfg_upd_t;


/*
 * All 'features' used to support Port Configurations must be
 * enumerated here. This enumeration should be immutable across
 * versions of Fabos; therefore new types should be added at the end,
 * with specifically assigned values. If a feature becomes obsolete,
 * it should be left in the list, with the name altered to reflect the
 * obsolete status.
 */

typedef enum {
	FEATURE_INVALID = 0,
	FEATURE_FCSW = 1,
	FEATURE_ISCSI = 2,
	FEATURE_FCIP = 3,
	FEATURE_NPORT = 4,
	FEATURE_SASPORT = 5,
	FEATURE_LAST = 5
} feature_code_t;

/*
 * The supported levels associated with each 'feature' should be
 * defined here. Note that Level One support of feature 'FC' is
 * defined as including those capabilities that are configurable through
 * the fields of the pcfg_bm_t structure alone.
 */

#define FEATURE_FCSW_LEVEL_1	1
#define FEATURE_FCSW_LEVEL_2	2	/* supports FC and vports */
#define FEATURE_ISCSI_LEVEL_1	1
#define FEATURE_FCIP_LEVEL_1	1
#define FEATURE_NPORT_LEVEL_1	1
#define FEATURE_SASPORT_LEVEL_1	1

typedef enum {
   	SFP_DEFAULT = 0,
    SFP_AVAGO,
	SFP_PICOLIGHT,
    SFP_FINISAR,
    SFP_INTEL,
    SFP_MOLEX,
    SFP_AMPHENOL,
	NUM_SFP_VENDORS 
} SFP_VENDORS;

typedef struct port_feature_fcsw_s {
	unsigned int	fcsw_minimum_level;
	/* All fcsw elements are currently found in the common area */
} port_feature_fcsw_t;

typedef struct port_feature_iscsi_s {
	unsigned int	iscsi_minimum_level;
	struct {
		unsigned int	iscsi_data_digest : 1;
		unsigned int	iscsi_header_digest : 1;
	} iscsi_options;
	unsigned int	iscsi_ip_address;
	unsigned int	iscsi_subnet_mask;
	unsigned int	iscsi_gateway_address;
	unsigned int	iscsi_version;
	/*
	 * The ip_port element is optional. If the release does
	 * not support configuration of this port, or if no
	 * configuration is specified, this configured value will
	 * be set to zero and the standard port number defined
	 * by iSCSI will be used.
	 */
	unsigned int	iscsi_ip_port;
} port_feature_iscsi_t;

typedef struct port_feature_fcip_s {
	unsigned int		fcip_minimum_level;
	struct {
		unsigned int	fcip_initiator : 1;
	} fcip_options;
	unsigned int	fcip_ip_address;
	unsigned int	fcip_subnet_mask;
	unsigned int	fcip_gateway_address;
	unsigned int	fcip_remote_ip_address;
} port_feature_fcip_t;

/*
 * This structure covers both FEATURE_NPORT and
 * FEATURE_SASPORT
 */
typedef struct port_feature_vport_s {
	unsigned int	vport_minimum_level;
	struct {
		unsigned int	vport_fc4vu : 1; /* FC-4 Vendor Unique Protocol */
		/*
		 * The SCSImode of an nport may be 0, 1, or 2.
		 * 0	Both Initiator and Target
		 * 1	Initiator Only
		 * 2	Target Only
		 */
		unsigned int	vport_SCSImode;
		unsigned int	vport_port_name_configured : 1;
		unsigned int	vport_node_name_configured : 1;
	} vport_options;
	wwn_t		vport_port_name;
	wwn_t		vport_node_name;
	unsigned int	vport_ip_addr;
} port_feature_vport_t;

/*
 * This structure containing port configurable and
 * private parameters.  It is used to push port
 * parameters from config database into switch
 * driver.
 *
 * The logical state bitmap (lgcl_pcfg_bm) is added to
 * provide a mechanism to save the state of a feature
 * across failover states. For example, a port may
 * be persistently disabled, but the logical state
 * could make the port logically online. After a failover,
 * the port would still be logically online. After a system
 * reboot, the port would then be persistently disabled.
 */
typedef struct _pt_cfg_prop_t {
	int		port;
	/*
	 * The port configuration bitmap for the following attributes:
	 * long distance level
	 * Port locked down as G-Port
	 * Trunking enabled/disabled
	 * Tx & Rx speed mode
	 * Port to loopback MCAST frames
	 * Disable a port from becoming E-Port
	 * Port locked down as L-Port
	 */
	pcfg_bm_t	pcfg_bm;	/* bitmap of port config params */
	priv_pcfg_t	priv_pcfg_bm;	/* bitmap of port's private config params */
	int		estd_dist;	/* estimated distance for LD mode */
	int		fport_buffers;	/* user configured fport required buffers */
	pcfg_bm_t	lgcl_pcfg_bm;	/* logical state bm config params */

	/* extcfg_portrec_t parameters */
	feature_code_t	pcfg_mode;
	unsigned int	pcfg_minimum_level;
	union {
		port_feature_fcsw_t	fcsw;
		port_feature_fcip_t	fcip;
		port_feature_iscsi_t	iscsi;
		port_feature_vport_t	vport;
	} feature;
} pt_cfg_prop_t;


typedef struct sys_pcfg_bm {
	int		sw_num;
	int		num_ports;
	pt_cfg_prop_t	*entries;
	int		num_ge_ports;
	pt_cfg_prop_t	*ge_entries;
} sys_pcfg_bm_t;

/*
 * Used to save the estimated distance and actual credits of
 * LD or degraded long distance ports persistently.
 */
typedef struct ld_info {
	u_short	est_dist;		/* estimated distance */
	u_short	alloc_cred;		/* actual credits allocated for the port */
} ld_info_t;

/* Used in static port_distance array */
#define INVLD_DIST_STRING	"??"
#define INVLD_DIST_DESCRN	"unknown"

/*
 * For long distance levels:
 * converting config arguments to two character or long description
 */
typedef struct port_dist {
	const char    *dist_level_string;
	const char    *dist_level_2ch_code;
	const char    *dist_level_description;
} port_dist_t;


#define	PORT_ENOUGH_BUFFER		0
#define	PORT_INSUFFICIENT_BUFFER	1

/*
 * Port CFG Distance Levels/strings
 */
/*
 * MAKE SURE THAT THE FOLLOWING defines match the entries
 * of the port_distance array in lib/fabos/switch/portcfg.c
 * ALSO BEWARE THAT THESE VALUES ARE USED TO INDEX INTO
 * ercc_vc array in bloom_vc.c.
 * ALSO BEWARE THAT THESE VALUES ARE SENT OUT IN ELP OPMODE
 * for Long Distance (fabric)E-Port negotiation.
 * So these should be in sync with 3.x/non-4.x for long
 * distance interoperability.
 */
#define PORT_DISTANCE_NORMAL    	0   	/* no license required */
#define PORT_DISTANCE_LONG		1	/* > 10km <= 50km */
#define PORT_DISTANCE_SUPERLONG 	2	/* > 50km <= 100km */
#define PORT_DISTANCE_EXT		3	/* <= 10km, no lic required */
#define PORT_DISTANCE_OLD_TOTAL		4	/* total levels */

#define PORT_DISTANCE_MEDIUM		4	/* > 10km <= 25km */
#define PORT_DISTANCE_AUTO		5	/*  LD dynamic */
#define PORT_DISTANCE_STATIC		6	/* LS static distance mode. */
						/* It replaces L1, L2 etc. */

/*
 * Define new distance levels here.
 * For each new entry define a string
 * in port_distance array in portcmds.c
 */
#define PORT_DISTANCE_TOTAL		7	/* total levels */
/*
 * Make sure PORT_DISTANCE_SIZE is power of 2 always
 * since PORT_DISTANCE_SIZE -1 is used to derive bitmask 0xffff
 * for ldlevel obtained from pcfg_bm_t.
 * Also make sure number of bits representing the long distance setting
 * is big enough for PORT_DISTANCE_SIZE in pcfg_bm_t.
 */
/* size of port_distance array */
#define PORT_DISTANCE_SIZE		16

/*
 * This change is done in response to the requirement in Quantum
 * (release v5.2.x) MRD to remove long distance levels L1, L2 and
 * L0.5 from extended fabric features, which are special cases of
 * the newly introduced level LS in v5.1.x release.  The LS level
 * can be used with finer granularity than the levels removed.
 */
#define PORT_DISTANCE_VALID(level)	(((level) >= PORT_DISTANCE_NORMAL) && \
					 ((level) < PORT_DISTANCE_TOTAL) && \
					 ((level) != PORT_DISTANCE_LONG) && \
					 ((level) != PORT_DISTANCE_MEDIUM) && \
					 ((level) != PORT_DISTANCE_SUPERLONG))

#define PORT_DISTANCE_NORMAL_STR    	"L0"
#define PORT_DISTANCE_LONG_STR		"L1"
#define PORT_DISTANCE_SUPERLONG_STR 	"L2"
#define PORT_DISTANCE_EXT_STR		"LE"
#define PORT_DISTANCE_25KM_STR		"L0.5"
#define PORT_DISTANCE_AUTO_STR		"LD"
#define PORT_DISTANCE_STATIC_STR	"LS"


/*
 * Port CFG Speed Levels
 * 1Gbps/2Gbps Auto Speed Negotiation settings
 * NOTE: this value is written into pcfg_bm_t's "speed" field which is only
 * 		 3 bits; thus only value of 0 ... 7 is allowed.
 */
typedef enum {
	AUTO_SPEED_NEGO = 0,    /* auto negotiate for highest speed */
	LOCK_AT_1GBPS   = 1,    /* lock TX & RX speed at 1Gbps only */
	LOCK_AT_2GBPS   = 2,    /* lock TX & RX speed at 2Gbps only */
	LOCK_AT_3GBPS   = 3,    /* INTERNAL link only:lock TX & RX speed at 3Gbps */
	LOCK_AT_4GBPS   = 4,    /* lock TX & RX speed at 4Gbps only */
	LOCK_AT_8GBPS   = 5,    /* lock TX & RX speed at 8Gbps only */
	LOCK_AT_10GBPS  = 6,    /* lock TX & RX speed at 10Gbps only */
	AUTO_SPEED_SOFT = 9,    /*
							 * Control AutoNegotiation through software:
							 * - not a user exposed command;
							 * - created in v520 (v5.2.0_pit_a/3, 14-Jun-2006)
							 *   for engr debug as an isolation tool if remote
							 *   port failed our HW auto speed negotiation;
							 * - was 8 (conflicting with cassini's 8G port)
							 *   in v520; now 9 for v530 & up.
							 */
	ASN_FRC_HWRETRY = 11,   /*
							 * auto negotiate for highest speed BUT
							 * force HW-SN to retry even if successful;
							 * this is for netApp 86791 fix.
							 */
	SPEED_INVALID   = -1    /* speed invalid */
} portSpeedLevel;

/*
 * Port CFG Delay Arb Mode:
 */
typedef enum {
	FILLWD_IDLE_IDLE = 0,	/* use IDLE  in link init & IDLE  as fill word */
	FILLWD_ARBFF_ARBFF = 1,	/* use ARBFF in link init & ARBFF as fill word */
	FILLWD_IDLE_ARBFF = 2, 	/* use IDLE  in link init & ARBFF as fill word */
	FILLWD_AA_THEN_IA = 3 	/* use mode 1 first & then mode 2 */
} portFillWordMode;


/*
 * Port CFG Persistent Settings
 */
#define PERSISTENTLY_ENABLE_PORT	0 /* Persistently enable a port */
#define PERSISTENTLY_DISABLE_PORT	1 /* Persistently disable a port */
#define NO_SYNC_PHY_LGCL_PCFGBM		0 /* Do not synchronize */
#define SYNC_PHY_LGCL_PCFGBM		1 /* Synchronize the physical and */
					  /* logical port config bitmaps */
/*
 * Port CFG Block Settings
 */
#define UNBLOCK_PORT	0	/* Unblock port, allowing port to enable */
#define BLOCK_PORT	1	/* Block port, do not allow port to  enable */
#define FMS_CHECK_SKIP	0	/* Do not check for FMS mode */
#define FMS_CHECK	1	/* Check for FMS mode */

/*
 * Port Loop port duplex and fairness settings
 */
#define LPORT_DEFAULT_FULLDUPLEX	0 /* default: FL port full duplex mode unfair */
#define LPORT_HALFDUPLEX_FAIRNESS	1 /* FL port half-duplex with fairness */
#define LPORT_FULLDUPLEX_FAIRNESS	2 /* FL port full-duplex with fairness */

/*
 * Port CFG data field access macros
 */
#define PORTCFG_AUTO_DISABLE(cfg)	((cfg).auto_disable)
#define PORTCFG_ALPA13(cfg)         ((cfg).alpa13)
#define PORTCFG_DELAY_FLOGI(cfg)	((cfg).delay_flogi)
#define PORTCFG_VCXLT_LINIT(cfg)	((cfg).vcxlt_linit)
#define PORTCFG_OLD_DISTANCE(cfg)	((cfg).old_distance)
#define PORTCFG_DISTANCE(cfg)		((cfg).distance)
#define PORTCFG_GPORTLOCK(cfg)		((cfg).gport_lockdown)
#define PORTCFG_LPORTLOCK(cfg)		((cfg).lport_lockdown)
#define PORTCFG_PRIV_LPORTLOCK(cfg)	((cfg).priv_lport_lockdown)
#define PORTCFG_EPORTDISABLE(cfg)	((cfg).disable_eport)
#define PORTCFG_TPORT(cfg)			((cfg).tport)
#define PORTCFG_ISL_INTEROP(cfg)	((cfg).isl_interop)
#define PORTCFG_SPEED(cfg)			((cfg).speed)
#define PORTCFG_FILL_WORD(cfg)		((cfg).fill_word)
#define PORTCFG_SOFT(cfg)      		((cfg).soft_neg)
#define PORTCFG_ASN_FRC_HWRETRY(cfg) ((cfg).asn_frc_hwretry)
#if 0	/* retired */
#define PORTCFG_PERSISTENTLY_DISABLED(cfg) ((cfg)._pvt_port_persistently_disabled)
#endif
#define	PORTCFG_BUFONLINE(cfg)		((cfg).BufStarvFlag)
#define PORTCFG_LPORT_HALFDUPLEX(cfg)	((cfg).lport_halfduplex)
#define PORTCFG_LPORT_FAIRNESS(cfg)	((cfg).lport_fairness)
#define PORTCFG_CREDITSHARE(cfg)	((cfg).credit_sharing)
#define PORTCFG_EXPORT(cfg)		((cfg).export)
#define PORTCFG_EXPORT_MODE(cfg)	((cfg).export_mode)
#define PORTCFG_NORSCN(cfg)		((cfg).rscn_suppress)
#define PORTCFG_NO_LOSSIG(cfg)	((cfg).losig)
#define PORTCFG_NPIV_ENABLED(cfg)	((cfg).npiv_enabled)
#define PORTCFG_NPIV_CONFIGURED(cfg)	((cfg).npiv_configured)
#define portCfgIsNport(cfg)        	\
	((PORTCFG_EPORTDISABLE(cfg) != 0) && (PORTCFG_GPORTLOCK(cfg) != 0))

#define PORTCFG_MIRROR_PORT(cfg)	((cfg).mirror_port)
#define PORTCFG_CREDITRECOVERY_PORT(cfg)	((cfg).cr_recov)
/*
** borrowing the functionality of the currently unused bit.
*/
#define PORTCFG_FCACCPORT(cfg)		((cfg).fcacc)
#define PORTCFG_FPORT_BUFFER_PORT(cfg)	((cfg).fport_buffers)

/* QOS related definitions */
#define PORTCFG_QOS_ENABLED(cfg)	((cfg).qos_enabled)
#define PORTCFG_QOS_CONFIGURED(cfg)	((cfg).qos_configured)
#define PORT_QOS_DEFAULT	0
#define PORT_QOS_DISABLE	1
#define PORT_QOS_ENABLE		2
#define PORT_QOS_SETRATELIMIT	3
#define PORT_QOS_RESETRATELIMIT	4

/* port beacon definition */
#define PORT_BEACON_ENABLE	2
#define PORT_BEACON_DISABLE	0

#define portCfgIsGportLocked(cfg)	(PORTCFG_GPORTLOCK(cfg) != 0)
#define portCfgIsLportLocked(cfg)	(PORTCFG_LPORTLOCK(cfg) != 0)
#define portCfgIsPrivLportLocked(cfg)	(PORTCFG_PRIV_LPORTLOCK(cfg) != 0)
#define portCfgIsEportDisabled(cfg)	(PORTCFG_EPORTDISABLE(cfg) != 0)
#define portCfgIsTport(cfg)		(PORTCFG_TPORT(cfg) != 0)
#define portCfgIsMcastLB(cfg)		(PORTCFG_MCASTLB(cfg) != 0)
#define portCfgIsVcXltLinkInit(cfg)	(PORTCFG_VCXLT_LINIT(cfg) != 0)
#define portCfgIsDelayFlogi(cfg)	(PORTCFG_DELAY_FLOGI(cfg) != 0)
#define portCfgIsIslInterop(cfg)	(PORTCFG_ISL_INTEROP(cfg) != 0)
#define portCfgIsPersistentlyDisabled(cfg) \
			(((cfg)._pvt_port_persistently_disabled != 0) || \
			 ((cfg)._pvt_port_permanently_disabled != 0))
#define portCfgIsPermanentlyDisabled(cfg) \
			 ((cfg)._pvt_port_permanently_disabled != 0)
#define portCfgIsNPIVConfigured(cfg)	(PORTCFG_NPIV_CONFIGURED(cfg) != 0)


#define portCfgIsRSCNSuppressed(cfg)	(PORTCFG_NORSCN(cfg) != 0)

#define portCfgIsNormalDistance(cfg)	\
	(PORTCFG_DISTANCE(cfg) == PORT_DISTANCE_NORMAL)
#define portCfgIsLongDistance(cfg)	\
	(PORTCFG_DISTANCE(cfg) == PORT_DISTANCE_LONG)
#define portCfgIsSLongDistance(cfg)	\
	(PORTCFG_DISTANCE(cfg) == PORT_DISTANCE_SUPERLONG)
#define portCfgIsExtDistance(cfg)	\
	(PORTCFG_DISTANCE(cfg) == PORT_DISTANCE_EXT)
#define portCfgIsAutoDistance(cfg)	\
	(PORTCFG_DISTANCE(cfg) == PORT_DISTANCE_AUTO)
#define portCfgIsStaticDistance(cfg)	\
	(PORTCFG_DISTANCE(cfg) == PORT_DISTANCE_STATIC)

#define portCfgIsSoftSpeed(cfg)     (PORTCFG_SOFT(cfg) == 1)
#define portCfgIsAutoSpeed(cfg)		(PORTCFG_SPEED(cfg) == AUTO_SPEED_NEGO)
#define portCfgIsAsnFrcHwretry(cfg)	(PORTCFG_ASN_FRC_HWRETRY(cfg) == 1)
#define portCfgIs1GSpeed(cfg)		(PORTCFG_SPEED(cfg) == LOCK_AT_1GBPS)
#define portCfgIs2GSpeed(cfg)		(PORTCFG_SPEED(cfg) == LOCK_AT_2GBPS)
#define portCfgIs4GSpeed(cfg)		(PORTCFG_SPEED(cfg) == LOCK_AT_4GBPS)
#define portCfgIs8GSpeed(cfg)		(PORTCFG_SPEED(cfg) == LOCK_AT_8GBPS)
#define portCfgIs10GSpeed(cfg)		(PORTCFG_SPEED(cfg) == LOCK_AT_10GBPS)
#define portCfgIsIdleIdleFW(cfg)	(PORTCFG_FILL_WORD(cfg) == FILLWD_IDLE_IDLE)
#define portCfgIsArbffArbffFW(cfg)	(PORTCFG_FILL_WORD(cfg) == FILLWD_ARBFF_ARBFF)
#define portCfgIsIdleArbffFW(cfg)	(PORTCFG_FILL_WORD(cfg) == FILLWD_IDLE_ARBFF)
#define portCfgIsAaThenIaFW(cfg)	(PORTCFG_FILL_WORD(cfg) == FILLWD_AA_THEN_IA)
#define	portCfgIsBufOnline(cfg)		(PORTCFG_BUFONLINE(cfg) == 0)
#define portCfgIsLportHalfDuplex(cfg)	(PORTCFG_LPORT_HALFDUPLEX(cfg) != 0)
#define	portCfgIsCreditShare(cfg)	(PORTCFG_CREDITSHARE(cfg) == 1)
#define portCfgIsLportFairness(cfg)	(PORTCFG_LPORT_FAIRNESS(cfg) != 0)
#define portCfgIsEXport(cfg)		(PORTCFG_EXPORT(cfg) == 1)
#define portCfgIsMirrorPort(cfg)	(PORTCFG_MIRROR_PORT(cfg) == 1)
#define portCfgIsCreditRecoveryPort(cfg)	(PORTCFG_CREDITRECOVERY_PORT(cfg) == 0)
#define portCfgIsFCAccPort(cfg)		(PORTCFG_FCACCPORT(cfg) == 1)
#define portCfgIsAlpa13(cfg)        (PORTCFG_ALPA13(cfg) == 1)
#define portCfgIsAutoDisable(cfg)	(PORTCFG_AUTO_DISABLE(cfg) == 1)
#define portCfgIsFportBuffers(cfg)	(PORTCFG_FPORT_BUFFER_PORT(cfg) == 1)

#ifdef __cplusplus
}
#endif

#endif	/* __SYS_FABCFG_H__ */

