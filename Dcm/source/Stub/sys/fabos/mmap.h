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
 *    Copyright (c) 1996-2011 Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Module name: mmap.h
 *
 *    Description:
 *	FabOS MMAP definition header file.
 *
 * Purpose:
 *	Some of the kernel data is frequently accessed by applications
 *	in user space. This results in performance degradation, since
 *	kernel data access is through ioctls, which basically involve
 *	copying data from/to user space.
 *		To alleviate this problem, the most frequently used
 *	kernel data is memory mapped to the userspace applications virtual
 *	address. All access(by applications) to data residing in the
 *	mmap'ed memory will result in direct access to the memory
 *	instead of ioctls.
 *	The MMAP layout:
 *	--------------------------------
 *	|   mmap_switch_info_t
 *	|	1. fabOP_t
 *	|       2. sw_domain
 *	|	3. sw_master
 *	|       4. sw_nports
 *	|       5. switchLimits[FABOS_RLIMIT_NLIMITS]
 *	|	6. fab_state
 *	---------------------------------
 *	|   Array of mmap_port_info_t(one entry per port)
 *	|   mmap_port_info_t
 *	|       1. p_flags
 *	|       2. p_id.
 *	|	3. p_state
 *	|	4. gbic data
 *	|	5. p_disable_reason
 *	----------------------------------
 *
 */
#ifndef	__SYS_FABOS_MMAP_H__
#define	__SYS_FABOS_MMAP_H__

#include <sys/port/gbic.h>
#if 0
#include <sys/switch/rlimit.h>
#include <sys/fabos/fabpid.h>

#include <sys/fc/fcls.h>
#include <sys/asic/filter.h>
#include <sys/fabos/fabobj.h>

#include <sys/swc/swc_fss.h>
#endif
/*
 * Defns. to get the address of the different structures part of
 * the MMAPed memory(given the base address of MMAPed memory)
 */
/* For Kernel Modules to reference switch info */
#define SYS_MMAP_SWITCH_INFO(mmap_info, switch) ((mmap_switch_info_t *) mmap_info + switch)

#define	MMAP_SWITCH_INFO(mmap_info) ((mmap_switch_info_t *) mmap_info + MYSWITCH)
#define	MMAP_ESWITCH_INFO(mmap_info) ((mmap_eswitch_info_t *) mmap_info)

#define	MMAP_PORT_INFO(mmap_info, port)	\
	(((mmap_port_info_t *)(mmap_info + sizeof (mmap_switch_info_t) * MAX_FCSW_PARTITIONS)) + port)

#define	MMAP_IF_INFO(mmap_info, if_idx, max_ports)	\
	(((mmap_if_info_t *)(((mmap_port_info_t *) (mmap_info + sizeof (mmap_switch_info_t) * MAX_FCSW_PARTITIONS)) + SMI_MAX_PORTS)) + if_idx)

#define	MMAP_GE_PORT_INFO(mmap_info, ge_port, max_ports)	\
	(((mmap_ge_port_info_t *)(((mmap_if_info_t *)(((mmap_port_info_t *) (mmap_info + sizeof (mmap_switch_info_t) * MAX_FCSW_PARTITIONS)) + SMI_MAX_PORTS)) + SMI_MAX_PORTS)) + ge_port)

#define MMAP_DCE_PORT_INFO(mmap_info, dce_port)  \
  ((mmap_dce_port_info_t *)(mmap_info + sizeof (mmap_eswitch_info_t))+ dce_port)

/* current operational values for switch */
#define SW_BBCREDIT     \
(MMAP_SWITCH_INFO(mmap_info) == NULL ? 0 : MMAP_SWITCH_INFO(mmap_info)->switchLimits[FABOS_RLIMIT_BBCREDIT].setting)

#define SW_DOMAINS      \
(MMAP_SWITCH_INFO(mmap_info) == NULL ? 0 : MMAP_SWITCH_INFO(mmap_info)->switchLimits[FABOS_RLIMIT_DOMAINS].setting)

#define SW_MCAST_ADDRS      \
(MMAP_SWITCH_INFO(mmap_info) == NULL ? 0 : MMAP_SWITCH_INFO(mmap_info)->switchLimits[FABOS_RLIMIT_MCAST_ADDRS].setting)

#define SW_PORTS        \
(MMAP_SWITCH_INFO(mmap_info) == NULL ? 0 : MMAP_SWITCH_INFO(mmap_info)->switchLimits[FABOS_RLIMIT_PORTS].setting)

/* operational maximum values */
#define SW_BBCREDIT_MAX     \
(MMAP_SWITCH_INFO(mmap_info) == NULL ? 0 : MMAP_SWITCH_INFO(mmap_info)->switchLimits[FABOS_RLIMIT_BBCREDIT].maximum)

/* logical port ranges */
#define LGC_PT_MAX(sw_info) (((mmap_switch_info_t *)sw_info)->sw_nports)
#ifdef __KERNEL__
#define LGC_PT_MIN(sw_info) fabSysMaxPhyPorts()
#endif

#define MAX_VC_CNTR 16
#define FABOS_RLIMIT_BBCREDIT       0
#define FABOS_RLIMIT_DOMAINS        1
#define FABOS_RLIMIT_MCAST_ADDRS    2
#define FABOS_RLIMIT_PORTS      3

#define FABOS_RLIMIT_NLIMITS        4

typedef struct rlimit_s {
    long     setting;
    long     minimum;
    long     maximum;
} rlimit_t;
typedef int if_id_t;
#if 0
typedef struct port_tuple_s {
	int blade;
	int port;
} port_tuple_t;

typedef enum {
	eINT_FC,
	eINT_ET,
	eINT_FCOE,
	eINT_INVALID
} interface_type_t;

typedef enum {
	eTYPE_1GE,
	eTYPE_10GE,
	eTYPE_40GE,
	eTYPE_100GE,
	eTYPE_ETH,
	eTYPE_FC,
	eTYPE_INVALID
} interface_display_type_t;
typedef struct external_interface_s {
	port_tuple_t 				user_tuple;
	port_tuple_t				phy_tuple;
	interface_type_t			type;
	interface_display_type_t	display_type;
} external_interface_t;

typedef struct neighbor_external_interface_s {
	port_tuple_t 				user_tuple;
	int							domain;
	interface_type_t			type;
	interface_display_type_t	display_type;
        int                         qsfp_idx;

} neighbor_external_interface_t;
#endif


/*
 * fabric operating parameters
 * this switch's operating parameters are stored in flash in the config struct
 * and are exchanged with neighboring switches through the ELP exchange process
 */
typedef struct {
    u_int   bb_credit;  /* buffer to buffer credit */
    u_int   df_size;    /* receive data field size */
    u_int   r_a_tov;    /* R-A timeout value */
    u_int   e_d_tov;    /* E-D timeout value */
    u_int   op_mode;    /* operating mode (see below) */
} fabOP2_t;


/*
 * never attempt to change this struct
 * as it is part of the iu
 * if the size change, the size of the iu will change
 * error will occur
 */
typedef struct {
    u_int   bb_credit;      /* buffer to buffer credit */
    u_int   df_size;        /* receive data field size */
    u_int   r_a_tov;        /* R-A timeout value */
    u_int   e_d_tov;        /* E-D timeout value */
    u_int   op_mode;        /* operating mode (see below) */
    u_int   vc_link_ctl;        /* VC for link controls [0..1] */
    u_int   vc_class2;      /* VC for Class 2 [2..5] */
    u_int   vc_class3;      /* VC for Class 3 [2..5] */
    u_int   vc_multicast;       /* VC for multicast [6..7] */
    u_int   vc_config;      /* VC priorities */
    u_int   vc_pid_map;     /* VC based on Port ID */
    u_short vc_credits[8];      /* VC credits [0..7] */
    u_int   req_credit;     /* credits required for full
                     * bandwidth utilization */
    u_int   op_mode2;       /* Opmode2 for opmode extension */
    u_int   reserved[3];        /* Cleared only from Cassini onwards,
                     * unused space */
} fabOP_t;

/* add a new struct to support interop mode */

typedef struct {
    u_int   interop_mode;
    u_int   fl_bbc;         /* use Alt BB Credit */
    u_int   fl_fan;         /* FAN Frame support */
    u_int   fl_cls;         /* open Send CLS */
    u_int   fl_rscn;        /* always Send RSCN */
    u_int   lab_mode;       /* support lab debugging mode */
    u_int   domain_id_offset;   /* Domain ID offset */
    u_int   mcdt_fabric_mode;   /* mcdata fabric mode of 0 or 1 */
    u_int   mcdt_default_zone;  /* mcdata default zone flag of 0 or 1 */
    u_int   mcdt_safe_zone; /* mcdata safe zone flag of 0 or 1 */
} fabInterOP_t;


/*
 * Additional attributes as extension to fabric operating parameters
 * that could not be included in fabOP_t.  This structure is used by
 * config and pdm to push these values to kernel.
 *
 * Note: The fabOP_t structure can not be modified (as per comment in
 * sys/include/fabos/fabop.h) because it is exchanges as part of ELP.
 * Any change to this structure will break backward compatibility. The
 * fabAttrib_t structure is defined to hold additional fabric attributes.
 */
typedef struct {
    u_int   wan_tov;
    u_int   max_hops;
} fabAttrib_t;


/*
 * Structure to store the per port and per switch
 * maximum values of virtual PIDs configured.
 */
typedef struct {
    u_int   per_port_max;   /* Max virtual PIDs configured per port. */
    u_int   per_switch_max; /* Max virtual PIDs configured per switch. */
    u_int   flogis_per_sec; /* FLOGIs per second for staged port bring up */
    u_int   stage_interval; /* Time interval for F-port staging. */
    u_int   free_fdiscs;    /* No. of FDISC logins allowed without staging. */
    u_int   enforce_login;  /* Allow FLOGI/FDISC duplicate port WWN to login into switch */
} fabLoginParam_t;

/*
 * switch info MMAPed to user space
 * added fabinterop to support interop mode
 */
typedef	struct	_mmap_switch_info {
	fabOP_t		sw_fabop;
	fabInterOP_t	sw_fabinterop;
	int		sw_domain; /* switch's current domain */
	int		sw_master;	/* principal domain of the fabric */
	int		sw_nports;	/* maximum number of user ports */
	int		sw_n_ge_ports;	/* maximum number of GigE user ports */
	int		sw_chassis_nslots; /* number of slots in a chassis */
	int		sw_ports_slot;	/* user ports per slot */
	int		sw_ge_ports_slot; /* user GigE ports per slot */
	int		sw_pics_slot;	/* pics per slot */
	rlimit_t	switchLimits[FABOS_RLIMIT_NLIMITS];
	int		fab_state;
	int		stage;		/* HA stage */
	int		substage;	/* HA sub-stage */
	fabAttrib_t	sw_fabattr;	/* Additional fabric attributes */
	fabLoginParam_t	sw_loginparam;	/* configured login params */
	int		sw_n_fi_ports;	/* maximum number of FI ports
					 * these are not user ports but
					 * it needs to be exposed to user
					 * space as NS is used it */
	int		sw_fds_state;	/* NOS: Switch FDS State */
} mmap_switch_info_t;


/*
 * ethernet switch info MMAPed to user space
 */
typedef struct  _mmap_eswitch_info {
    int     sw_dce_nports;  /* maximum number of DCE FE ports */
	int		sw_n_dce_fi_ports;	/* maximum number of DCE FI ports */
    int     sw_dce_chassis_nslots; /* number of slots in a chassis */
    int     sw_dce_ports_slot; /* user DCE ports per slot */
    int     sw_dce_pics_slot;   /* pics per slot */
    rlimit_t  eswitchLimits[FABOS_RLIMIT_NLIMITS];
    int     stage;      /* HA stage */
    int     substage;   /* HA sub-stage */
} mmap_eswitch_info_t;




/*
 * Interface info MMAPed to user space
 */
typedef	struct	_mmap_if_info {
	if_id_t		if_id;
} mmap_if_info_t;


#define SWC_SYNC_BEGIN     // ;
#define SWC_SYNC_END       // ;

typedef struct {
    u_int   ls_code;    /* Code - ELS_ACC */
    u_int   ls_link_failure;
    u_int   ls_loss_of_sync;
    u_int   ls_loss_of_sig;
    u_int   ls_protocol_err;
    u_int   ls_invalid_word;
    u_int   ls_invalid_crc;
} lsRLSACC_t;


/* port level info MMAPed to user space */
typedef struct _mmap_port_info {
	SWC_SYNC_BEGIN		/* object id for sync update */

	/*
	 * When a port is disabled we only send the event and not all the
	 * data to the standby.  It is therefore important for the standby,
	 * upon receiving the control update, to set the relevant fields
	 * according to what port_disable would do.  Currently, p_flags,
	 * p_type, and p_state are such relevant fields.
	 *
	 * In the future, if you add more bits to p_flags, p_type, or
	 * new field here, make sure you update them in sw_update_pt_disable
	 * accordingly.
	 */

	/* TBD: revisit assignment of bits to p_flags vs. p_type */
	u_int	p_flags;
	u_int	p_type;
	u_int	p_id;		/* port address ID */
	u_char	p_state;	/* SNMP port state */

	gbic_t		gbic;	/* raw gbic data */
	reason_t	p_disable_reason;	/* disable reason */
	int	area;
	if_id_t	p_if_id;	/* if_id for the user port */
	wwn_t	p_phy_port_wwn; /* Physical Neighboring Port WWN */
	int     reserved[1];

	SWC_SYNC_END		/* end of sync delimiter */

	/*
	 * the following structures has been added in mmap
	 * because it's been found that these are the most
	 * frequently used statistics by the damons. inorder
	 * to improve the performance, they have been modified
	 * so that they can be accessed directly from user
	 * space through mmap.
	 */
	u_int		p_rxperf;	/* words rx in last second */
	u_int		p_txperf;	/* words tx in last second */
	u_int		p_transc;	/* state trans in last second */

	u_int	p_rxperfli;	/* words rx in last 64 seconds */
	u_int	p_txperfli;	/* words tx in last 64 seconds */

	u_int	p_frmrxperf;	/* frames rx in last second */
	u_int	p_frmtxperf;	/* frames tx in last second */
	u_int	p_frmrxperfli;	/* frames rx in last 64 seconds */
	u_int	p_frmtxperfli;	/* frames tx in last 64 seconds */

	struct timeval p_vctxcrdz_time[MAX_VC_CNTR]; /* time BB credit = 0 time stamp	*/
	u_int	txcrdz_time_diff[MAX_VC_CNTR];	/* in microseconds	*/
	u_int   p_vctxcrdz[MAX_VC_CNTR];	/* time BB credit = 0 in last second per VC */
	u_int	p_txcrdz;	/* time BB credit = 0 in last second */
	u_int	p_txcrdzli;	/* time BB credit = 0 in last 64 seconds */

    /*
     * Additional statistics added for FICON, but any user space application
     * can access these values -> see ./fabos/src/include/switch/port.h
 	 * (The value in parenthesis is the FICON Stat counter ID)
     */
    u_int   p_txwords;            // words transmitted (901)
    u_int   p_rxwords;            // words received (902)
    u_int   p_txframes;           // frames transmitted (903)
    u_int   p_rxframes;           // frames received (904)
    u_int   p_rxfrm_c2;           // Class 2 frames received (905)
    u_int   p_rxfrm_c3;           // Class 3 frames received (906)
    u_int   p_rxfrm_lnkctrl;      // Link Control Frames received (907)
    u_int   p_rxfrm_multi;        // Multi cast Frames received (908)
    u_int   p_tx_zero_cred;       // ZeroTxBbCred - used for Frame Pacing Time (909)
    u_int   p_infrm_enc_err;      // Disparity Errors within Frames (910)
    u_int   p_rxfrm_crc_err;      // CRC Errors (911)
    u_int   p_rxfrm_too_long_err; // Frames > Maximum size (912)
    u_int   p_rxfrm_trunc_err;    // Frames < Minimum size (913)
    u_int   p_rxfrm_bad_eof;      // Frames with bad or missing EOF (914)
    u_int   p_outfrm_enc_err;     // Disparity errors outside frames (915)
    u_int   p_rx_inv_ordered_set; // Invalid Ordered Sets outside frames (916)
    u_int   p_rxfrm_c3_discard;   // class 3 frames discarded (917)
    u_int   p_link_failures;      // link failures (920)
    u_int   p_sync_losses;        // Loss of Sync detected by port (921)
    u_int   p_sig_losses;         // Loss of Signal detected by port (922)
    u_int   p_prim_seq_proto_err; // Primitive sequence protocol errors (923)
    u_int   p_invalid_txwords;    // Invalid Transmition errors (924)
    u_int   p_address_iderrors;   // Address ID errors (925)
    u_int   p_tx_link_reset;      // LRR issued by port (926)
    u_int   p_rx_ols_ins;         // OLS received by port (927)
    u_int   p_tx_ols;             // OLS issued by port (928)


	lsRLSACC_t	lesb;		/* link err status block vals */
	int		gen_num; /* port's generation number */

	u_char 	p_status_led_state; 	/* port status led state */
	u_char	p_speed_led_state; 		/* port speed led state */

	/*
	 * User port's connection type -- INTERNAL or EXTERNAL (regular FE ports).
	 *
	 * External ports a.k.a. FE ports are manageable by the switch driver and
	 * configurable by the user.
	 * Internal ports a.k.a. FI ports are manageable by the switch driver but
	 * not configurable by the user.
	 */
	bool	p_is_port_internal;

	/* used for a blade greater than 32 ports */
	int		p_shared_prop;	/* Bit map of shared properties */
	int		p_resource_peer;	/* Other port sharing same redirect resource */
	u_char	p_alpa_base;	/* base ALPA of port */
	u_char	p_alpa_mask;	/* ALPA mask of port */
	int		p_index;		/* logical port index, added in Positron */
	int		p_trunk_area;	/* trunk area of trunk group */
	int		p_lgc_port;		/* logical port that represents trunk area trunk group */
	external_interface_t ext_int_data; /* external interface information about the port for DCM */
	neighbor_external_interface_t neighbor_ext_int_data; /* external interface information about the neighbor port for DCM */
} mmap_port_info_t;

/* GE port level info MMAPed to user space */
typedef struct _mmap_ge_port_info {

	/* TBD: revisit assignment of bits to p_flags vs. p_type */
	u_int	p_flags;
	u_int	p_type;
	u_char	p_state;	/* SNMP port state */

	gbic_t		gbic;	/* raw gbic data */
	reason_t	p_disable_reason;	/* disable reason */

	u_char 	p_status_led_state; 	/* port status led state */
	u_char	p_speed_led_state; 		/* port speed led state */

} mmap_ge_port_info_t;

/* DCE port level info MMAPed to user space */
typedef struct _mmap_dce_port_info {
	SWC_SYNC_BEGIN		/* object id for sync update */
    u_int   p_flags;
    u_int   p_type;
    u_char  p_state;    /* SNMP port state */
    gbic_t      gbic;   /* raw gbic data */
	SWC_SYNC_END		/* end of sync delimiter */
    u_char  p_status_led_state;     /* port status led state */
    u_char  p_speed_led_state;      /* port speed led state */
} mmap_dce_port_info_t;


#endif /*  __MMAP_H__ */
