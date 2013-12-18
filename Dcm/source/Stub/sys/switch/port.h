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
 *    Copyright (c) 1996-2006 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Module name: Switch driver
 *
 *    Description:
 *      Port Driver header file
 */


#ifndef __PORT_H__
#define	__PORT_H__

#include <sys/fabos/mmap.h>
#include <sys/fabos/fabha_db_sys.h>
#include <sys/fc/npv.h>
#include <sys/port/admin.h>
#include <sys/port/pstats64.h>
//#include <linux/spinlock.h>
//#include <linux/proc_fs.h>
#include <sys/port/timer.h>
#include <sys/switch/fcr_ioctl.h>
#include <sys/asic/ptio_codes.h>
#include <sys/fabos/fabcmds.h>
//#include <thresh/fwdPub.h>



#define     PT_SHARE_AREA   0x00000001  /* Port shares an area */
#define     PT_PRIMARY      0x00000002  /* Primary Port */
#define     PT_IS_UPPER     0x00000004  /* Port has highest base area */
#define     PT_SHARE_AREA_FPORT_TRUNK 0x00000008    /* Shared area port suppor*/
#define IF_ID_T uint32_t
typedef uint32_t opaque_t;

typedef struct fc_header {
    u_int           fc_d_id;     /* Destination ID */

}  fch_t;

typedef struct kiu {
    fch_t           *iu_hdr;        /* Pointer to the FC header */
    u_int            iu_flags;      /* Flag */
} kiu_t;

/*
 * Generic message type:
 */
#define GENERIC_MSGBLK \
	struct _km * km_next;   /* Forward linked list pointer */ \
    struct _km * km_prev;   /* Backword linked list pointer */ \
    int32_t km_type;    /* message type (see above) */ \
    int32_t km_refc;    /* reference count */ \
    uint32_t km_len;    /* message length specifying payload length(not including header) */
typedef struct _km {
    GENERIC_MSGBLK
} km_t;

typedef uint32_t pm_event_t;
typedef u_short     xid_t;      /* Exchange ID */

typedef struct  {
    wwn_t   port_name;  /* FLOGI port worldwide name */
    wwn_t   node_name;  /* FLOGI node worldwide name */
    int topology;   /* Port type (F/FL: 0/1) */
    u_char  fc4map;     /* Procotols supported */
    u_char  cos;        /* Classes of service supported */
    u_int   f_scr;      /* state change registration */
    u_int   hide_device;    /* Switch driver wants to hide this from ULPs */
} npInfo_t;

#define BM_NBBY 8                   /* # bits per byte */
#define BM_NBPF (sizeof (unsigned long) * BM_NBBY)  /* # bits per field */
typedef unsigned long * BitMap_t;

/* Typedefs for Domain and Port Bitmaps */
typedef unsigned long domainBitMap_t[256 / BM_NBPF];

/* Typedefs and defines for managing port bitmaps */
typedef BitMap_t portBitMap_t;
typedef unsigned long portBitMapArr_t[64];
/*
 * interrupt statistics
 */
typedef struct {
    u_int   p_interrupts;   /* total interrupt counter */
    u_int   p_unknown;  /* unknown interrupts */
    u_int   p_lli;      /* LLI interrupts */
    u_int   p_proc_rqrd;    /* processing required interrupts */
    u_int   p_timed_out;    /* timed out interrupts */
    u_int   p_rx_flushed;   /* rx flushed interrupts */
    u_int   p_tx_unavail;   /* tx unavailable interrupts */
    u_int   p_free_buffer;  /* free buffer interrupts */
    u_int   p_overrun;  /* overrun interrupts */
    u_int   p_suspended;    /* suspended interrupts */
    u_int   p_parity_err;   /* parity error interrupts */
    u_int   p_secondary_parity_err; /* secondary tx parity error intr */
                    /*
                     * Note: see bloomPortReceive() for
                     * explanation on real vs. secondary
                     * parity errors.
                     */
    u_int   p_cmi_bus_err;  /* CMI bus error */
} istat_t;


typedef struct  {
    wwn_t   port_name;  /* port worldwide name */
    wwn_t   node_name;  /* node worldwide name */
    wwn_t   logical_port_name;  /* local logical port name */
    int type;   /* Port type (N/F: 1/2) */
    int port_id;        /* port id */
} fnport_Info_t;

typedef struct {
    GENERIC_MSGBLK     /* generic message header */
    opaque_t msg_handle;    /* port handle */
    pm_event_t msg_state;   /* port media state code */
    u_long msg_data;    /* message specific data */
    int     msg_age;    /* number of attempts to process the message */
} pmMsg_t;

/*
 * Data needed for phantom translation
 */
typedef struct xlate_id {
    u_int   pre_s_id;
    u_int   pre_d_id;
    u_int   post_s_id;
    u_int   post_d_id;
    u_int   new_entry;  /* Set if a new entry is created */
} xlate_id_t;

#define portPhysName(port) fportPhysName(FabOS_SW_FD, port)
#define portClrStats(port) fportClrStats(FabOS_SW_FD, port)
#define portEportDisabled(port) fportEportDisabled(FabOS_SW_FD, port)
/*
 * Port ioctl argument structure:
 */
typedef struct _portIoc {
    int port;       /* Port number */
    int arg1;       /* First arguement */
    int arg2;       /* Second arguement */
    int arg3;       /* Third arguement */
    int arg4;       /* Fourth argument - also used for Interface id */
    int result;     /* Returned result */
    if_id_t ifid;   /* Ifid of the port */
} portIoc_t;

/*
 * often used macro on segment reason
 * declared so that user space can use too
 */
#define SEG_REASON_VALID(r) ((r) >= SEG_REASON_MIN && \
                 (r) <= SEG_REASON_MAX)
/*
 * High 16 bits in seg_reason will be used
 * for detailed reason for RFE 2183
 */
#define SEG_GET_DET_REASON(r) (((r) & 0xffff0000) >> 16)
#define SEG_GET_SHORT_REASON(r) ((r) & 0x0000ffff)

typedef enum {
    SEG_REASON_MIN = 1,
    SEG_LOOPBACK = SEG_REASON_MIN,  /* port has a loopback cable */
    SEG_INCOMPAT,       /* switches are incompatible */
    SEG_OVERLAP,        /* overlapping domains */
    SEG_ZONE,       /* overlapping zones */
    SEG_EPORT_SEG,      /*
                 * Generic segment bit (used to be
                 * SEG_FSPF but never used).  Reason we
                 * need this bit is that if user adds new
                 * segment reasons, a generic bit must
                 * be set so that the CP with older
                 * code will recognize a common bit
                 * which means the port has been segmented.
                 * This bit should never be pushed down to
                 * kernel because if it does, the SEG_FSPF
                 * bit will be translated on older code
                 * as an FSPF mismatch segmentation reason.
                 */
    SEG_LICENSE,        /* no license */
    SEG_EPORT_DISABLED, /* This port is disabled to be an E_Port */
    SEG_PLATFORM_DB,    /* incompatible platform database */
    SEG_SEC_INCOMPAT,   /* Security incompatibility */
    SEG_SEC_VIOLATION,  /* Security Violation */
    SEG_ECP_LONGDIST,   /* ECP rejected or retries exceeded */
    SEG_DUP_WWN,        /* Duplicate WWN in fabric */
    SEG_EPORT_ISOLATED, /* Eport Isolated */
    SEG_AD,             /* AD header conflict */
    SEG_ESC_DID_OFFSET, /* Domain ID Offset conflict */
    SEG_SAFE_ZONE,      /* McData SafeZone conflict */
    SEG_VF,         /* VF AD conflict */
    SEG_MSFR,       /* MSFR/RD Host and Target WWN conflict */
    SEG_ENH_TIZONE,     /* Enhanced TI Zone Incompatibility */
    SEG_ESC_CONFLICT,   /* ESC detected conflict */
    SEG_ENCRY_CONFLICT, /* Encryption conflict */
    SEG_COMP_CONFLICT,  /* Compression conflict */
    SEG_ENC_COMP_RES_CONFLICT,  /*
                     * Encryption/Compression bandwidth
                     * unavailablitiy
                     */
	SEG_FDS_CONFLICT,
    /*
     * Always add new segment reasons at the end of the list
     * and equate it to SEG_REASON_MAX defined below.
     *
     * Note: Please remember to update the seg_reasons array
     * in fabric/fabric.c file with string names corresponding
     * to the new segment reasons added here.
     */
    SEG_REASON_MAX = SEG_ENC_COMP_RES_CONFLICT
} seg_reason;
#define SEG_REASONS (SEG_REASON_MAX - SEG_REASON_MIN + 1)


/*
 * Alpa information
 */
typedef struct alpa_info {
    u_char alpa;    /* alpa value */
    u_char xnew; /* is alpa newly created */
} alpa_info_t;

/*
 * In port fault case, msg_topo field stores faulty reason
 * defined as port_fault_reason_t.
 * In port OFFLINE case, msg_top field stores LINK_DOWN reason
 * define as port_down_reason-t.  This only applies to old drivers.
 * For new drivers, the link change reason will be stored in
 * the msg_reason field, using the port_change_reason_t reasons.
 */
/*
 * Used for LINK_UP, LINK_DOWN, PORT_DISABLED, etc.
 */
typedef struct {
    GENERIC_MSGBLK     /* generic message header */
    opaque_t msg_handle;    /* link state port handle */
    int  msg_state; /* link state code, LINK_UP, LINK_DOWN, etc. */
    int  msg_topo;  /* link topology */
    IF_ID_T msg_if_id;   /* interface id corresponding to Logical Port */
    IF_ID_T msg_prev_if_id;  /* previous interface id */
    int  msg_mode; /* loopback mode */

    /*
     * The following two fields apply to LINK_UP and LINK_DOWN only.
     */

    /* current master of msg_if_id, equal to msg_handle for non-slaves */
    opaque_t master_handle;
    /*
     * speed of msg_if_id (if LINK_UP), msg_prev_id_if (if LINK_DOWN).
     * 1=1Mb/sec.  Speed of msg_if_id is known to be zero if LINK_DOWN
     */
    int bandwidth;
    int msg_reason;     /* link state change reason */
    int msg_data;       /* additional data */
} lsMsg_t;



typedef enum {
    FAB_UNKNOWN,
    FAB_HA,     /* re-visit needed */
    FAB_FC,
    FAB_TYPE_MIN = FAB_FC,
    FAB_SWITCH,
    FAB_BLADE,
    FAB_PIC,
    FAB_MINIS,
    FAB_CHIP,
    FAB_IF,
    FAB_PORT,
    FAB_MEDIA,
    FAB_USR,
    FAB_DIAGSW,
    FAB_DIAGBL,
    FAB_CHASSIS,
    FAB_DIAGPIC,
    FAB_ROUTE,
    FAB_GE_USR,
    FAB_LIF,
    FAB_TYPE_MAX = FAB_LIF,
    FAB_OBJ5, /* Spare used in fabobj_set_my_reference */
    FAB_OBJ6, /* Spare used in fabobj_set_my_reference */
    FAB_OBJ7  /* Spare used in fabobj_set_my_reference */
}fabType;

typedef struct _ioc {
    int cmd;
    int arg1;
    int arg2;
    int arg3;
    int arg4;
} ioc_t;


/* From /usr/include/asm/ioctl.h */
#define _IOC_NRBITS 8
#define _IOC_TYPEBITS   8
#define _IOC_SIZEBITS   14
#define _IOC_DIRBITS    2

#define _IOC_NRMASK ((1 << _IOC_NRBITS)-1)
#define _IOC_TYPEMASK   ((1 << _IOC_TYPEBITS)-1)
#define _IOC_SIZEMASK   ((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK    ((1 << _IOC_DIRBITS)-1)

#define _IOC_NRSHIFT    0
#define _IOC_TYPESHIFT  (_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT  (_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT   (_IOC_SIZESHIFT+_IOC_SIZEBITS)

#define _IOC_NONE   0U
#define _IOC_WRITE  1U
#define _IOC_READ   2U

#define _IOC(dir,type,nr,size) \
	    (((dir)  << _IOC_DIRSHIFT) | \
		      ((type) << _IOC_TYPESHIFT) | \
		      ((nr)   << _IOC_NRSHIFT) | \
		      ((size) << _IOC_SIZESHIFT))

/* used to create numbers */
#define _IO(type,nr)        _IOC(_IOC_NONE,(type),(nr),0)

/* IOC_PT_ */
#define IOC_PT_PTIO         _IO(IOC_PT_MAGIC, 2)    /* port based ioctls */

#define PTIO_RTE_VIF_DELETE _ASIC_IOC(ASIC_RTE, 0x30)


/*
 * High 16 bits in seg_reason will be used
 * for detailed reason for RFE 2183
 */

 /*
  * SEG_PUT_DET_REASON assumes that high 16 bits
  * are CLEAR(set to 0).
  * SEG_CLEAR_DET_REASON should be called
  * before calling SEG_PUT_DET_REASON.
  */
#define SEG_CLEAR_DET_REASON(r)  ((r) &= (0x0000ffff))
#define SEG_PUT_DET_REASON(r, det_reason) ((r) |= \
					(u_int)(((det_reason) & 0xffff) << 16))

#define DEB_TIME		((150 * HZ)/1000) /* 150ms */
#define DEB_CUTOFF_TIME		(51) /* ~ 500 ms - should be odd!!! */
#define DEB_RSTRTS		20

#define	PT_DIAG_TESTING(pt)	((pt)->p_diag_cflags & DIAG_TESTING)
#define	PT_SET_DIAG_TESTING(pt)	((pt)->p_diag_cflags |= DIAG_TESTING)
#define	PT_CLR_DIAG_TESTING(pt)	((pt)->p_diag_cflags &= ~DIAG_TESTING)

#define PT_INVALID_CONTROL(pt, cmd, rval)	\
	VVERIFY3(0, (pt)->p_number, (cmd), (rval));

#define INVALID_LG_PORT_NUM 0

//SFP Read failure threshold limit
#define SFP_READ_THRESHOLD	3

//static int switch_port_present(opaque_t swhan, int port);


/*
 * Switch port state definitions:
 */
typedef enum _swPState {
	PORT_DOWN,	/* port is currently offline */
	PORT_UP,	/* port is currently online */
	PORT_DISC,	/* port is under E_Port discovery */
	PORT_LOGIN,	/* port is processing FLOGI from Nx_Port */
	PORT_ISL,	/* port has been determined as an E_Port */
	PORT_ACTIVE	/* port is an active Fx_Port in operations */
} swPState_e;

typedef struct _phdev {
	u_int	p_pid:24;	/* S_ID of phantom device */
	u_char	p_alpa;		/* AL_PA assigned for the phantom device */
	u_int	p_age;		/* Age of the entry (0 - active) */
	struct _phdev *p_next;	/* Link list pointer */
} phdev_t;

typedef struct _xels {
	u_int	p_els_sid:24;	/* S_ID of the ELS frame (pre-translated) */
	u_int	p_els_did:24;	/* D_ID of the ELS frame (pre_translated) */
	u_short	p_els_xid;	/* Exchange ID of the ELS frame (ox_id) */
	u_int	p_els_code;	/* ELS code (first word of the payload) */
	u_char	p_els_age;	/* Age of this ELS entry */
} xels_t;
/*
 * FSPF information
 */
typedef struct _fspf_info {
    u_int   rem_dom;      /* remote domain */
    u_int   rem_pt;       /* remote port */
    u_int   rem_state;    /* fspf state */
    u_int   link_cost;    /* link cost */
} fspf_info_t;

/*
 * TBD: Resolve which bits belong in p_type vs. p_flags
 * p_type was added during the merge from v4.1_main to main since
 * both branches had used up the last free bits in p_flags.
 * In case, any of these flags that are not required to be
 * synced to the standby, they may be moved to p_local_flags
 * in sys/dev/switch/port.h with proper change in verion number.
 */
/*
 * p_type - private port type values - used by port drivers
 */
#define P_EPORT     0x00000001  /* port is an E_Port */
#define P_FPORT     0x00000002  /* port is an F_Port */
#define P_GPORT     0x00000004  /* port is an G_Port */
#define P_LPORT     0x00000008  /* port is an L_Port */
#define P_UPORT     0x00000010  /* port is a universal port */
#define P_SEGMENTED 0x00000020  /* port segmented (incompatible) */
#define P_TPORT     0x00000040  /* Trunk port type */
#define P_TMASTER   0x00000080  /* Trunk master port type */
#define P_NPV       0x00000100  /* port is N_Port Virtualization F_Port */
#define P_EXPORT    0x00000200  /* port is an EX_Port */
#define P_LEPORT    0x00000400  /* port is an Logical E-port */
#define P_TFPORT    0x00000800  /* FPort trunk port type */
#define P_TFMASTER  0x00001000  /* FPort trunk master port type */
#define P_LGPORT    0x00002000  /*  port is an Logical port(LISL/VSAN/TAport)
                                    g_lg_port_bm can identify the logical port
                                    sub type (below) */
#define P_PHY_EXPORT    0x00004000      /* Formed EX_port after ELP success */
#define P_TRUE_EPORT    0x00008000      /* E-Port after EFP success */
#define P_ENCRYPT   0x00010000  /* Port is encrypted port after ELP */
#define P_COMPRESS  0x00020000  /* Port is compressed port after ELP */
#define P_DPORT     0x00040000  /* D-Port type */

#define MASTER_FPORT    (P_FPORT | P_TFPORT | P_TFMASTER)
#define TRUNK_MASK      (P_FPORT | P_TFPORT | P_TFMASTER)

/* The following macro definitions are from  sys/include/fc/fcsw.h */
enum {
    FLOW_CTL_INVALID,
    FLOW_CTL_VC_MODE,
    FLOW_CTL_NORMAL_MODE,
    FLOW_CTL_DUAL_CR_MODE,
    FLOW_CTL_EXT_VC_MODE
};


/*
 * Definitions for the loopback modes used by PTIO_ENABLE.
 */
#define P_NORMAL    0x00000000  /* normal non-loopback mode */
#define P_INT_LB    0x00000001  /* internal loop back */
#define P_SLW_LB    0x00000002  /* serial link wrapback (serdes) */
#define P_EXT_LB    0x00000004  /* external loop back (serdes) */
#define P_CBL_LB    0x00000008  /* cabled loop back */
#define P_SSC_LB    0x00000010  /* silk screen loop back */
#define P_FL_INT_LB 0x00000020  /* FL_Port internal loopback */
#define P_BYP_LB    0x00000040  /* external bypass */
#define P_PLW_LB    0x00000080  /* parallel link wrapback (MAC/PCS) */

#define P_HARD_LB   (P_INT_LB | P_SLW_LB | P_EXT_LB | P_SSC_LB | P_PLW_LB)

/*
 * WARNING: used as value of fields that are synched for HA.
 * Therfore any additions MUST be done at the end of the enum.
 *
 * asic_phys - physical state values
 *
 */
typedef enum {
    NO_CARD = 1,    /* no option card installed */
    NO_MODULE,  /* no GBIC module installed */
    LASER_FLT,  /* laser fault */
    NO_LIGHT,   /* no light being received */
    NO_SYNC,    /* out of sync */
    IN_SYNC,    /* in sync */
    PORT_FLT,   /* port fault */
    DIAG_FLT,   /* diagnostic fault */
    LOCK_REF,   /* receiver locking reference clock */
    MOD_VAL,    /* module validation in process */
    MOD_INV,    /* invalid module */
    REMOTE_FLT, /* remote fault signal received */
    LOCAL_FLT,  /* local fault signal received */
    SIG_DET,    /* No HSS Signal */
    NOT_USED = 255  /* unused state: used for VE ports */
} phys_states;

/*
 * LED control values
 */
typedef enum {
    LED_MANUAL, /* manual control by application */
    LED_AUTO,   /* automatic control by port driver */
    LED_HW,     /* the hardware driver controls the LED */
    LED_BLACK,
    LED_YELLOW,
    LED_GREEN
} led_control_t;


//static int g_lg_port_bm[SMI_MAX_PORTS];
#define SWC_SYNC_BEGIN 
#define SWC_SYNC_END 
/*
 * g_lg_port_bm: bit map to identify logical port (sub) type
 */
#define P_LISLPORT  0x00000001  /* port is an Logical ISL port */
#define P_TAPORT    0x00000002  /* port is an Logical trunk area Fport */
#define P_VSANPORT  0x00000004  /* port is an Logical VSAN Fport */

/*
 * p_flags - private port status values - used by port drivers
 */
#define P_PRESENT   0x00000001  /* port present (card plugged in) */
#define P_ACTIVE    0x00000002  /* port is in the active state */
#define P_UPSTREAM  0x00000004
#define P_DNSTREAM  0x00000008
#define P_NOLGI     0x00000010  /* port disabled to disallow FLOGI */
#define P_DISABLED  0x00000020  /* port disabled */
#define P_NOSYNC    0x00000040  /* port is no sync */
#define P_NOLIGHT   0x00000080  /* port has no light */
#define P_LOGIN     0x00000100  /* port is logged in (FLOGI or ELP) */
#define P_NOELP     0x00000200  /* ELP failed don't try again */
#define P_FAULT     0x00000400  /* port is faulty */
#define P_LGCL_ONLINE   0X00000800  /* port is logical online (used by HA) */
#define P_RRDY_MODE 0x00001000  /* port has R_RDY mode */
#define P_DEB       0x00002000  /* Debounce period started */
#define P_LED       0x00004000  /* enable automatic LED control */
#define P_NSREG     0x00008000  /* port has devices to be registered */
#define P_PROBE     0x00010000  /* port has devices to be probed */
#define P_ACCEPT    0x00020000  /* FLOGI ACC can be sent if set */
#define P_BYPASSED  0x00040000  /* bypassed */
#define P_FAULT_RETRY   0x00080000  /* faulty port is being reinited */
#define P_LOOPDIAG  0x00100000  /* up but in loop diag mode */
#define P_TRUNKABLE 0x00200000  /* Whether the port is trunkable or not */
#define P_DISABLE_EPORT 0x00400000  /* E_PORT disabled */
#define P_TNEGOTIATING  0x00800000  /* port is negotiating T port */
#define P_CLASS2    0x01000000  /* port has class 2 traffic */
#define P_DEGRADED  0x02000000  /* port is in degraded mode */
#define P_SEC_START 0x04000000  /* Security start on port */
#define P_TRK_START 0x08000000  /* F-port trunking has started on port */
#define P_AG_ACPORT 0x10000000  /* AC_PORT sent to AG */
#define P_SET_SFID_SH  0x20000000   /* set sfid is done for shared ports */
#define P_VC_START  0x40000000  /* In Ext VC mode */
#define P_BEACON  0x80000000    /* port is beaconing */
/*
 * Definitions for port polling types
 */
#define PT_POLL_TICK    0   /* bloom port per-tick-polling */
#define PT_POLL_SEC 1   /* bloom port per-second-polling */
#define PT_POLL_X_SEC   5   /* bloom port per-5second-polling */

typedef int status_e;
typedef struct _port {
	SWC_SYNC_BEGIN		/* object id for sync update */

	int	p_cflags;	/* control flags */
	int	p_cbl_lb;	/* cabled loopback port */
	u_char	p_ie_fctl_mode;	/* inter-element flow control mode type */

	u_int	p_newbmp[4];	/* Up-to-date (all) device bitmap */
	u_int	p_nocbmp[4];	/* Unchanged AL_PA device bitmap */
	u_int	p_prbbmp[4];	/* Probed device bitmap */

	u_int	p_npv_devbmp[NPV_NWORDS]; /* NPV bitmap previously returned */
					  /* to application (ie. NS)	    */
	u_int	p_npv_newbmp[NPV_NWORDS]; /* NPV current bitmap */
	wwn_t	node_name;	/* node_name */

	u_int	seg_reason;
	u_short	p_rtstate;	/* routing setup state */

	u_int	p_login[4];	/* Login bitmap */
//	flogi_t p_flogi;	/* FLOGI parameters (linked list for loop) */
	u_int	area;
	int		le_domain;	/* this EX-port's LE domain or 0 if none */

	int    	ti_info;	 /* TI group number of the traffic isolation group to
							which this port belongs. */
	int	full_fmt_area;	/* for standby to save the 10bit area for this port */
	int     reserved[1];  /* Cassini: Changed from 3 to 2 to account for the
							 new ti_group_num field. */

	SWC_SYNC_END		/* end of sync delimiter */

	u_int	p_updbmp[4];	/* Updated device bitmap */
	int	p_diag_cflags;	/* diagnostics control flags */
	status_e p_status;	/* status of the switch port */
	opaque_t p_handle;	/* object descriptor */
	fab_tnode_t * tnodep;	/* point into private obj tree */
	opaque_t p_switch;	/* pointer to switch */
	int	p_number;	/* port number */
	int	beacon;		/* beacon is ON/OFF */

//	struct work_struct p_task; /* transmit task of the mini-switch */
//	kmq_t	p_msgq;		/* arrary of port tx queues */

	struct proc_dir_entry *proc; /* proc-fs directory for this port */

	mmap_port_info_t *p_info; /* The MMAPed data */
	mmap_port_info_t *p_info_logical; /* The MMAPed logical data */

	u_char	p_scn;		/* last SCN delivered */
	u_char	p_phscn;	/* last physical SCN sent(OFFLINE/ONLINE) */
	u_char	p_tick;		/* counter for SCN delivery */


	u_char	p_deb_tick;	/* counter for SCN debounce timer */
	u_char	p_cutoff_tick;	/* counter for SCN debounce cutoff timer */
	u_char	p_deb_restarts; /* counter for debounce period restarts */


	int	p_hard_lb;	/* hard loopbcak mode */
	u_char	p_led;		/* last LED color set */
	u_char	p_led_mode;	/* LED mode, i.e., LED_HW/LED_MANUAL/LED_AUTO */

	wwn_t	p_wwn;		/* port Worldwide Name */

	u_int	p_devbmp[4];	/* Device bitmap */
	u_int	p_fcpbmp[4];	/* SCSI-FCP device bitmap */
	u_char	p_prbtc;	/* Probing trigger tick counter */
	u_char	p_poll_state;	/* maintains the status of ASIC polls */

	u_int	p_phantom[4];	/* Login bitmap */
	phdev_t *p_phntm;	/* Phantom device linked list */
	xels_t	*p_x_els;	/* Header Ptr to ACC pending exchange array */
	u_int	p_x_num;	/* Number of entries in the p_x_els array */
	u_int	p_x_free;	/* Number of slots that are free */
	u_int	p_x_used;	/* How many array elements have been used */

	fcFxPortC1AcctType	p_c1ac;	/* Class 1 accounting */
	fcFxPortC2AcctType	p_c2ac;	/* Class 2 accounting */
	fcFxPortC3AcctType	p_c3ac;	/* Class 3 accounting */

	/*
	 * NOTE: the stats counters are modified in the switch_poll_ports
	 * timer function, and so access during ioctls must be protected
	 * by spin_lock_bh() to prevent data corruption.
	 */
	port32_t counter32Prev;	/* 32-bit previous stats */
	port64_t counter64Port;	/* 64-bit stats */

	u_int	p_wdtx;		/* words transmitted */
	u_int	p_wdrx;		/* words received */
	u_int	p_wtot;		/* total words tx/rx */
	int	p_perf_ticks;   /* Last time perf was calculated in ticks */


	u_int	p_frmtx;		/* frames transmitted */
	u_int	p_frmrx;		/* frames received */
	u_int	p_frmtot;		/* total frames tx/rx */

	u_int	p_txrateacc;	/* transmitted rate accumulator */
	u_int	p_rxrateacc;	/* received rate accumulator */

	u_int	p_frmtxrateacc;	/* transmitted frame rate accumulator */
	u_int	p_frmrxrateacc;	/* received frame rate accumulator */
	int	p_acc_ticks;    /* Last time before 64 seconds peroid */
	u_int   p_oldvctxcrdz[MAX_VC_CNTR];    /* time BB credit = 0 one second 
		                                    * ago per VC 
		                                    */
	struct timeval p_oldvctxcrdz_time[MAX_VC_CNTR];
	u_int	p_oldtxcrdz;	/* time BB credit = 0 one second ago */
	u_int	p_oldtxcrdzli;	/* time BB credit = 0 64 seconds ago */

	/*
	 * FLOGI/PLOGI timer, to force response even if routing/zoning
	 * hasn't finished.  Don't access directly; rather use
	 * port_add_logi_timer and port_del_logi_timer, to protect
	 * against adding twice. [dbusing 4/15/2003]
	 */
	struct timer_list	logi_timer;

	u_int	link_downs;	/* number of link downs in 5 mins */
	u_long	last_time;	/* last time count started */
	int	logi_retry_cnt;	/* number of retries */
	int	port_offline_reason;
	if_id_t	prev_if_id;	/* previous interface id */
	u_int	p_local_flags;	/* Per port local flags, not to be synced */

	u_short	p_login_count;	/* number of FDISC(0) logins active on this port */
	fcrExPortCfg_t	ex_portcfg; /* EX_Port config from fcrd */

    /* AG related port structures */
    u_int port_id;
    wwn_t a_pwwn;   /* attached nport wwn */
    wwn_t a_nwwn;
    u_int port_type;
    wwn_t a_lgc_pwwn; /* ag logical port wwn */
	fspf_info_t fspf_info;	/* fspf info */
	int read_th;	/* Threshold counter for SFP read failure */
	int port_op_hist_bmp;	/* bitmap for port operation history */
} port_t;



/*
 * definition on port operation that may cause mutliple port state changes
 * and bump up p_transc. This will cause application such as port fencing
 * unable to maintain its threshold. These operations will be recorded in
 * port_op_hist_bmp and checked in switch_scn. p_transc increase will be
 * skipped when these bits are set
 */
#define	PORT_THROTTLE_ON_F_PORT		      	0x0001
#define	PORT_REINIT_ON_F_PORT_TRUNKING		0x0002


/*
 * Update the memory map functions to the logical port
 * if applicable.
 */
enum {
    MMAP_MEMCPY,	/* copy port to logical if port is master */
    MMAP_CLR_LGC_PT	/* clear logical port since no trunks are up */
};

/*
 * Logical port wwn data
 */
typedef struct lg_wwn_data {
	wwn_t pwwn;	/* port wwn */
	int pnum;	/* port number */
} lg_wwn_data_t;

/* p_local_flags: port status values local to the switch driver */
#define PL_FLOGI	0x0001		/* One or more FLOGIs accepted */
#define PL_PROBING	0x0002		/* FCP probing in progress. */
				/* Used in FDISC(0) processing on NPIV ports. */
#define PL_WAS_MULTI_LOGIN 0x0004	/* Was a port capable of multiple logins before disable (could be loop or NPIV port). */

#define	P_AREART	0x1    /* Area routing complete */
#define	P_DOMAINRT	0x2    /* FSPF declares domain routing complete */
#define P_FLOGIHELD	0x4    /* FLOGI rcvd before switchenable, being held */
#define P_PLOGIHELD	0x8    /* Same, for PLOGI */
#define P_HOLDPLOGI	0x10
/*
 * defect 26286 - removed this unused definition to avoid HA data mismatched.
 * re-use or uncomment it if this is needed.
 * #define P_HOLDNSQUERY	0x20
*/
#define P_FLOGIDELAY	0x40   /* Forces wait until timer expires            */
                               /* used for device-specific workaround delays */
#define P_PHANTOM	0x100
#define P_ZONE_DONE	0x200  /* Zoning has prepared this port */
#define P_LOGI_TIMER_ON 0x400  /* FLOGI/PLOGI timer is ticking,              */
                               /* used to avoid redundant timer adds         */
#define P_MULTIDONE	 0x800 /* Multicast routing completed  */

#define AG_NPORT_TYPE   01
#define AG_FPORT_TYPE   02

#define FLOGI_TOV	(25 * HZ/100)
#define FLOGI_DELAY_TOV	(850 * HZ/100)
#define NSPLOGI_TOV	(75 * HZ/100)
#define RUN_AT(s)	(jiffies + (s))

#define port_set_scn_type(pt, scn) \
		if (portEXport(pt)) { \
			scn.type = EX_PORT_SCN; \
		} else { \
			scn.type = PORT_SCN; \
		}

#define ENOENT 2
#define PT_IOC_MAGIC        (('P' + 'O' + 'R' + 'T') & _IOC_TYPEMASK)
#define    IOC_P_SET_CFG_BM _IO(PT_IOC_MAGIC, 87)

char *mmap_info;
#define MAX_FCSW_PARTITIONS 16
#define IOC_P_CLR_STATS _IO(ST_IOC_MAGIC, 21)
#define    IOC_P_GET_SGMT_RSN   _IO(PT_IOC_MAGIC, 23)
#define FKIO &MODNAME(FKIO_LIB)
#define    IOC_P_PHY_NO_CARD    _IOR(PT_IOC_MAGIC, 14, int)
#define    IOC_P_PHY_NO_MOD _IOR(PT_IOC_MAGIC, 15, int)
#define    IOC_P_PHY_LAS_FLT    _IOR(PT_IOC_MAGIC, 16, int)
#define    IOC_P_PHY_NO_LYT _IOR(PT_IOC_MAGIC, 17, int)
#define    IOC_P_PHY_NO_SYNC    _IOR(PT_IOC_MAGIC, 18, int)
#define    IOC_P_PHY_IN_SYNC    _IOR(PT_IOC_MAGIC, 19, int)
#define    IOC_P_PHY_PORT_FLT   _IOR(PT_IOC_MAGIC, 20, int)
#define    IOC_P_PHY_DIAG_FLT   _IOR(PT_IOC_MAGIC, 21, int)
#define    IOC_P_PHY_LOCK_REF   _IOR(PT_IOC_MAGIC, 22, int)
#define    IOC_P_PHY_MOD_INV    _IOR(PT_IOC_MAGIC, 23, int)

#define    IOC_P_T_PORT     _IOR(PT_IOC_MAGIC, 24, int)
#define    IOC_P_T_MASTER_PORT  _IOR(PT_IOC_MAGIC, 25, int)

#define    IOC_P_GET_BUF_ALLOC  _IO(PT_IOC_MAGIC, 95)
#define    IOC_P_GET_LX_DIST    _IO(PT_IOC_MAGIC, 96)
#define    IOC_P_DISABLE_WITH_REASON    _IO(PT_IOC_MAGIC, 7)
#define    IOC_P_GET_DISABLE_REASON     _IO(PT_IOC_MAGIC, 8)
#define    IOC_P_DSBL       _IO(PT_IOC_MAGIC, 21) /* disabled */

#define portDiag(port) fportDiag(FabOS_SW_FD, port)
/*
 *  * port status is unknown.
 *   * Never sent or registered as a subtype hence can be 0
 *    */
#define UNKNOWN     0x00000000
    /* port is online (in active state) */
#define ONLINE      0x00000001
    /* port is offline */
#define OFFLINE     0x00000002
    /* port is in use by diagnostics */
#define TESTING     0x00000004
    /* port is marked faulty */
#define FAULTY      0x00000008
    /* port is an E_Port */
#define E_PORT      0x00000010
    /* port is an F_port */
#define F_PORT      0x00000020
    /* port is segmented */
#define SEGMENTED   0x00000040
    /* port is a trunking port, not trunk master */
#define T_PORT      0x00000080
    /* Loop initialization occurred */
#define LIP_ONLINE  0x00000100
    /* Force OFFLINE a port that is already OFFLINE */
#define FORCE_OFFLINE   0x00000200
    /* became online by acquiring free buffers */
#define BUF_ONLINE  0x00000400
    /* became offline due to lack of buffers */
#define BUF_OFFLINE 0x00000800
    /* debounced and still offline with new reason */
#define OFFLINE_NEW_REASON  0x00001000
    /* became degraded due to lack of buffers */
#define BUF_DEGRADED        0x00002000
    /* degraded bitmap needs to be cleared and updated */
#define BUF_UPDATE          0x00004000
    /* change slave trunk port to new master */
#define EPORT_CHANGE        0x00008000
    /* Switch has received a PLOGI */
#define PLOGI_RCVD  0x00010000
    /* Switch has received a LOGO */
#define LOGO_RCVD   0x00020000
    /* SCN when an exchange is aborted. */
#define EXCHANGE_ABORT_NOTIFY   0x00040000
    /* port is an virtual FL port */
#define VF_PORT     0x00080000
    /* Area route and multicast route setup completed */
#define AREART_MULTI_DONE   0x00100000
    /* Switch has received an explicit FLOGO */
#define EFLOGO_RCVD 0x00200000
    /* F_port change */


#define ST_IOC_MAGIC        (('S' + 'T' + 'A' + 'T' + 'I') & _IOC_TYPEMASK)

extern unsigned int __invalid_size_argument_for_IOC;
#define _IOC_TYPECHECK(t) (sizeof(t))

/* used to create numbers */
#define _IO(type,nr)        _IOC(_IOC_NONE,(type),(nr),0)
#define _IOR(type,nr,size)  _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOW(type,nr,size)  _IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOWR(type,nr,size) _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOR_BAD(type,nr,size)  _IOC(_IOC_READ,(type),(nr),sizeof(size))
#define _IOW_BAD(type,nr,size)  _IOC(_IOC_WRITE,(type),(nr),sizeof(size))
#define _IOWR_BAD(type,nr,size) _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),sizeof(size))

/* used to decode ioctl numbers.. */
#define _IOC_DIR(nr)        (((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr)       (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr)     (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr)       (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)

/* ...and for the drivers/sound files... */

#define IOC_IN      (_IOC_WRITE << _IOC_DIRSHIFT)
#define IOC_OUT     (_IOC_READ << _IOC_DIRSHIFT)
#define IOC_INOUT   ((_IOC_WRITE|_IOC_READ) << _IOC_DIRSHIFT)
#define IOCSIZE_MASK    (_IOC_SIZEMASK << _IOC_SIZESHIFT)
#define IOCSIZE_SHIFT   (_IOC_SIZESHIFT)
/* ePortInfo_t flag entries */
#define PORT_INFO_CREDIT_SHARING        0x0001      /* Credit sharing */
#define PORT_INFO_FPORT_TRUNKING_CAP    0x0002      /* F_PORT trunking capable */
#define PORT_USE_RRDY_CREDIT            0x0004      /* Use RRDY credit */
#define PORT_INFO_VC_ARB_MODE       0x0008
#define PORT_CONDOR2_IN_VCRDY_MODE  0x0010      /* To let fabric know it has
                             * potential issue supporting
                             * this mode */
#define PORT_BB_SC_N_VALID              0x0020      /* BB_SC_N Valid */
#define PORT_INFO_FPORT                 0x0040      /* F port Trunk port
                                                     * Dont move to TRUE_EPORT if set */
#define PORT_REMOTE_ASIC_CONDOR_3       0x0080      /* Credit Rec supported on all VCs */

/* Max supported bb_sc_n */
#define PORT_BB_SC_N_MAX_VALUE          0xF         /* Max  BB_SC_N Valid */
#define PORT_BB_SC_N_MASK               0xF         /* Mask  BB_SC_N Valid */

/*
 * Maximum number of int elements to represent user port bitmap
 */
#define MAX_PORT_BM     58  /* 58 * 32 = 1856 (1800 ports - includes both phy and lgc) */

/*
 * Port configuration return code for valid/invalid configuration
 */
#define VALID_CONFIG    1
#define INVALID_CONFIG  0

#define portOnline(p)       (portState(p) == ONLINE)
#define portOffline(p)      (portState(p) == OFFLINE)
#define portTesting(p)      portDiag(p)
#define portFaulty(p)       (portState(p) == FAULTY)
#define portType(p)     (fportType(FabOS_SW_FD, p))

#define NAA_UNDEF       0   /* Ignored */
#define NAA_IEEE48      1   /* IEEE 48 bit 802.1A ULA */
#define NAA_IEEE48X     2   /* IEEE extended 48 bit 802.1A ULA */
#define NAA_LOCAL       3   /* Locally assigned */
#define NAA_IP          4   /* Internet address */
#define NAA_VENDOR      5   /* Vendor specific address */
#define NAA_CCITT_IND   12  /* CCITT individual address */


#define portDisabled(port) fportDisabled(FabOS_SW_FD, port)
#define portStateName(port) fportStateName(FabOS_SW_FD, port)
#define portGetDisableReasonStr(port, reason) \
	            fportGetDisableReasonStr(FabOS_SW_FD, port, reason)


#define    IOC_P_TRANS_COUNT    _IOR(PT_IOC_MAGIC, 32, int)
#define    IOC_P_GET_ASIC_ID    _IOR(PT_IOC_MAGIC, 33, int)
#define    IOC_P_GET_AREA   _IOR(PT_IOC_MAGIC, 34, int)
#define    IOC_P_GET_SFP_DIRECT     _IOR(PT_IOC_MAGIC, 35, int)

#define    IOC_P_GET_ENC_COMP_CONFIG    _IOR(PT_IOC_MAGIC, 36, int)
#define    IOC_P_GET_ECB_INFO        _IOR(PT_IOC_MAGIC, 37, int)
#define    IOC_P_IS_ENCRYPT_PORT     _IOR(PT_IOC_MAGIC, 38, int)
#define    IOC_P_IS_COMPRESS_PORT    _IOR(PT_IOC_MAGIC, 39, int)
#define    IOC_P_GET_ENC_KEY_SIZE    _IOR(PT_IOC_MAGIC, 40, int)
#define    IOC_P_GET_ENC_MODE        _IOR(PT_IOC_MAGIC, 41, int)
#define    IOC_P_GET_ENC_COMP_STATUS _IOR(PT_IOC_MAGIC, 42, int)
#define    IOC_P_PHY_MOD_VAL         _IOR(PT_IOC_MAGIC, 43, int)
#define    IOC_P_PHY_NO_SIG_DET      _IOR(PT_IOC_MAGIC, 44, int)
#define    IOC_P_GET_HSS_INFO        _IOR(PT_IOC_MAGIC, 45, int)
#define    IOC_P_SET_HSS_INFO        _IOR(PT_IOC_MAGIC, 46, int)
#define    IOC_P_PORT_STATE _IO(PT_IOC_MAGIC, 65)
#define    IOC_P_DIAG           _IO(PT_IOC_MAGIC, 103)

static int ioctl (int __fd, unsigned long int __request, ...)
{
	return (0);
}

static inline int
wwnzero(const wwn_t *wwn)
{
    /*
     * It is sufficient to check the first byte of the
     * WWN, because the WWN is undefined when it is zero.
     */
    return (wwn->ieee.ww_naaNnib == NAA_UNDEF);
}
#define portGetCfgBm(port, pcfg_bm) fportGetCfgBm(FabOS_SW_FD, port, pcfg_bm)
#define    IOC_P_PERF       _IO(PT_IOC_MAGIC, 67)
#define    IOC_P_RX_PERF    _IO(PT_IOC_MAGIC, 68)
#define    IOC_P_TX_PERF    _IO(PT_IOC_MAGIC, 69)

#define    IOC_P_SET_CFG_BM _IO(PT_IOC_MAGIC, 87)
#define    IOC_P_GET_CFG_BM _IO(PT_IOC_MAGIC, 88)
#define    IOC_P_CHECK_PORT_CFG     _IO(PT_IOC_MAGIC, 107)



#define PORT_DISTANCE_NORMAL        0       /* no license required */
#define PORT_DISTANCE_LONG      1   /* > 10km <= 50km */
#define PORT_DISTANCE_SUPERLONG     2   /* > 50km <= 100km */
#define PORT_DISTANCE_EXT       3   /* <= 10km, no lic required */
#define PORT_DISTANCE_OLD_TOTAL     4   /* total levels */

#define PORT_DISTANCE_MEDIUM        4   /* > 10km <= 25km */
#define PORT_DISTANCE_AUTO      5   /*  LD dynamic */
#define PORT_DISTANCE_STATIC        6   /* LS static distance mode. */
                        /* It replaces L1, L2 etc. */

/* Used to disable fportbuffer configured port */
#define PORT_FPORTBUFFER_NORMAL     0


typedef enum {
    PORTCFG_NPIV_SUCCESS                    = 6, /* if the Qos cfg change
                                                  * does not change the
                                                  * real QOS state
                                                  */
    PORTCFG_SUCCESS_NO_BOUNCE               = 5, /* one or more E-ports may
                                                  * become degraded
                                                  */
    PORTCFG_BC_E_DEGR                       = 4, /* port maybe degraded if the
                                                  * actual dist is longer than
                                                  * estimated
                                                  */
    PORTCFG_LD_WARN                         = 3, /* port maybe reserved more
                                                  * credits, depending on the
                                                  * port speed
                                                  */
    PORTCFG_AN_WARN                         = 2,
    PORTCFG_NOCHNG                          = 1,
    PORTCFG_SUCCESS                         = 0,
    PORTCFG_FAILED                          = -1,
    PORTCFG_BADPORT                         = -2,
    PORTCFG_BADARG                          = -3,
    PORTCFG_BADNUMARG                       = -4,
    PORTCFG_CFGABT                          = -5,
    PORTCFG_NOLICENSE                       = -6,
    PORTCFG_BADSWTYPE                       = -7,
    PORTCFG_ISQLPORT                        = -8,
    PORTCFG_ISLPORT                         = -9,
    PORTCFG_ISGPORT                         = -10,
    PORTCFG_MCASTLB_LBEXIST                 = -11,  /* mcast loop back port exist */
    PORTCFG_LONGDIST_MCASTON                = -12,
    PORTCFG_LONGDIST_NOLDFAB                = -13,
    PORTCFG_BADPTTYPE                       = -14,
    PORTCFG_TESTING                         = -15,
    PORTCFG_NOSUPPORT                       = -16,
    PORTCFG_NOTALLOW_ISLLD                  = -17,
    PORTCFG_ISMPORT                         = -18,
/* newly defined entries */
    PORTCFG_BADARG_SPEEDLVL                 = -20,
    PORTCFG_BADARG_LXLVL                    = -21,
    PORTCFG_BADARG_TPORT                    = -22,
    PORTCFG_BADARG_GPORT                    = -23,
    PORTCFG_BADARG_LPORT                    = -24,
    PORTCFG_BADARG_LINIT                    = -25,
    PORTCFG_BADARG_ESTD_DIST                = -26, /* Bad argument for
                                                    * estimated distance,
                                                    * required for long
                                                    * distance level LD only.
                                                    */
    PORTCFG_BADARG_FILL_WORD                = -27,

    PORTCFG_NOLICENSE_EF                    = -30,
    PORTCFG_NOLICENSE_TRUNKING              = -31,
    PORTCFG_NOSUPPORT_TRUNKING              = -32,
    PORTCFG_NOSUPPORT_EF                    = -33,
    PORTCFG_NOSUPPORT_TRUNKING_LX           = -34,
    PORTCFG_NOSUPPORT_LX_TRUNKON            = -35,
    PORTCFG_BADSTRING                       = -36,
    PORTCFG_NOCAPABILITY_TRUNKING           = -37,
    PORTCFG_NOCAPABILITY_LONGDIST           = -38,
    PORTCFG_NOSUPPORT_TRUNKING_EX           = -39,
    PORTCFG_FILL_WORD_SUPPORT_8G_ONLY       = -40,
    PORTCFG_BADSTRING_DUP                   = -41,
    PORTCFG_BADSTRING_SWITCHNAME            = -42,

/* refer to the states in portcfg.c - xxx routine */
    PORTCFG_FIXED_BC_INVALID                = -50,
    PORTCFG_FIXED_BC_S0                     = -51,
    PORTCFG_FIXED_BC_S1                     = -52,
    PORTCFG_FIXED_BC_S2                     = -53,
    PORTCFG_FIXED_BC_S3                     = -54,
    PORTCFG_LD_S5                           = -55,
    PORTCFG_LD_S3                           = -56,
    PORTCFG_LD_S3_1                         = -57,
    PORTCFG_LD_S1                           = -58,
    PORTCFG_LD_S4                           = -59,
    PORTCFG_AN_S2                           = -60,
    PORTCFG_AN_S10                          = -61,
    PORTCFG_AN_S8                           = -62,
/* User has configured a configuration that exceeds what is supported */
    PORTCFG_BC_OVFLOW                       = -63,
    PORTCFG_BC_OVFLOW_1G                    = -64,
    PORTCFG_BC_OVFLOW_2G                    = -65,
    PORTCFG_BC_OVFLOW_4G                    = -66,
    PORTCFG_BC_OVFLOW_10G                   = -67,
/* port doesn't support credit sharing */
    PORTCFG_NO_CREDITSHARE                  = -68,
    PORTCFG_BC_OVFLOW_8G                    = -69,


/* return values from ficud serialization calls in lib/ficu/ficu_api.c */
    PORTCFG_SWITCHBUSY                      = -70,
    PORTCFG_FICU_EIPC                       = -71,
    PORTCFG_FICU_NOT_ASM                    = -72, /* Not (active==saved)
                                                            mode. */
    PORTCFG_FICU_EOTHER                     = -73,
    PORTCFG_NOCAPABILITY                    = -74,
    PORTCFG_NOCAPABILITY_PHANTOM            = -75,
    PORTCFG_LOGICAL                         = -76, /* Logical port, op not applicable */
    PORTCFG_NOTALLOW_LDISL                  = -77,
    PORTCFG_EXPORT                          = -78, /* EX port, op not applicable */
    PORTCFG_NOT_SUPPORT_LPORT               = -79, /* L-port Not supported */
    PORTCFG_PLAT_NOCAPABILITY               = -80, /* Platform not capable supporting feature */
    PORTCFG_NOSUPPORT_EMBEDDED_PORT         = -81,
    PORTCFG_LONG_DST                        = -82, /* Port is configured as long distance */
    PORTCFG_NOTALLOW_ISL_QOS                = -83, /* Port is configured as QOS port */
    PORTCFG_NOTALLOW_QOS_ISL                = -84, /* Port is configured as R_RDY port */
    PORTCFG_ICL                             = -85, /* ICL port, op not applicable */
    PORTCFG_TRUNK_AREA_SHARED_AREA          = -86, /* TA is enabled on shared area */
    PORTCFG_TRUNK_AREA_PID_FORMAT           = -87, /* TA is enabled when switch not in CORE PID */
    PORTCFG_FICU_EOORANGE                   = -88,


/* FC Acceleration return values. */
    PORTCFG_ISMPORT_FCACC                   = -90,
    PORTCFG_FCACC_ISMPORT                   = -91,
    PORTCFG_FCACC_NOTALLOWED                = -92,
    PORTCFG_FCACC_EXPORT                    = -93,

    PORTCFG_TRUNK_AREA_FCACC                = -94, /* FC Acceleration not allowed on TA ports */
    PORTCFG_TRUNK_AREA_MIRROR               = -95, /* Mirror port not allowed on TA ports */
    PORTCFG_TA_LONG_DISTANCE                = -96, /* Long Distance not allowed on TA ports */
    PORTCFG_TA_EXPORT                       = -97, /* EX_port not allowed on TA ports */
    PORTCFG_CREDIT_RECOV_EXPORT             = -98, /* EX_port not allowed on CR Recov ports */
    PORTCFG_LGPORT                          = -99, /* Logical Port */
    PORTCFG_LPORT                           = -100, /* Not allowed on Lport */
    PORTCFG_FCACC                           = -101, /* Not allowed on fast write port */
    PORTCFG_QOS                             = -102, /* Not allowed on QOS port */
    PORTCFG_TRUNK_AREA                      = -103, /* Not allowed on TA */
    PORTCFG_FPORT_BUFFER                    = -104, /* Not allowed on Fport Buffer */
    PORTCFG_FPORT_BUFFER_INV_DISABLE_USAGE  = -105, /* Invalid Fport Buffer disable usage */
    PORTCFG_FPORT_BUFFER_INV_ENABLE_USAGE   = -106, /* Invalid Fport buffer enable usage */
    PORTCFG_FPORT_BUFFER_INV_MODE           = -107, /* Invalid mode for Fport Buffer */
    PORTCFG_FPORT_BUFFER_HELP               = -108, /* Help mode */
    PORTCFG_NOTALLOW_VCINIT_QOS             = -109, /* when vcinit== 0, QOS enable is not allowed */
    PORTCFG_PORT_HAS_NO_AREA_DISABLED       = -110,
    PORTCFG_NO_QOS_INTEROP                  = -111, /* QOS configuration is not available in interop mode, IM2/IM3 */
    PORTCFG_BADARG_FPORT_BBCREDIT           = -112, /*when F Port BB credit value becomes less than 8 */
    PORTCFG_EPORT                           = -113, /* When Port is locked as E-port */
    PORTCFG_BC_OVFLOW_16G                   = -114,
    PORTCFG_SPEED_COMBO_INVALID             = -115, /* Invalid Octet Speed Combo */
    PORTCFG_REQ_10G_BUT_INV_COMBO1          = -116, /* 10G requested but octet speed combo is invalid */
    PORTCFG_REQ_16G_BUT_INV_COMBO2          = -117, /* 16G requested but octet speed combo is invalid */
    PORTCFG_NPIV_DISABLE_PORT               = -118,
    PORTCFG_NPIV_INVALID_LIMIT              = -119,
    PORTCFG_NPIV_NO_SUPPORT                 = -120,
    PORTCFG_NPIV_FAIL                       = -121,
    PORTCFG_REQ_2G_4G_8G_BUT_INV_COMBO3     = -122,
    PORTCFG_DPORT                           = -123,
    PORTCFG_EPORT_NOT_SUPP_IN_DEFSW         = -124, /* This port doesnt support E port in DS */
    PORTCFG_FAULT_DELAY_INVALID             = -125, /* Invalid fault delay interval */
    PORTCFG_FICU_MIXPORTS                   = -126,
    PORTCFG_COMPRESSION_NO_SUPPORT          = -127, /* Port type does not support Compression */
    PORTCFG_ENCRYPTION_NO_SUPPORT           = -128, /* Port type does not support Encryption */
    PORTCFG_ENC_COMP_INVALID                = -129, /* Encryption and compression cannot be enabled on non-E ports */
    PORTCFG_DPORT_DISABLE_PORT              = -130, /* Port needs to in disabled state for D-Port mode change */
    PORTCFG_DPORT_RRDY_PORT                 = -131, /* D-Port is not allowed on RRDY mode */
    PORTCFG_RRDY_DPORT                      = -132, /* RRDY mode is not allowed on D-Port*/
    PORTCFG_ISLRRDY_ON_BASE_SW_NOTSUPPORT   = -133, /* ISL R_RDY mode not supported on BS ports */
    PORTCFG_NOTALLOWED                     = -134, /* Maximum allowed ports configured */
                                        /* for Encryption/Compression */
    PORTCFG_FICU_SWAP_RESERVED              = -135, /* NO swap with CUP reserved ports */
    PORTCFG_OCTET_COMBO_UNSUP_SPEED         = -136, /* speed unsupporte in cbo*/
    PORTCFG_OCTET_COMBO_2_3_PORT_ONLINE     = -137, /* port online when
switching combo betwen 2 & 3 */
    PORTCFG_OCTET_COMBO_INVALID_PORT        = -138, /* command allowed on first
8 ports */
    PORTCFG_ENC_COMP_DISABLE_PORT           = -139, /* Port needs to in disabled state for Encryption/Compression change */
    PORTCFG_10GSPEED_NOTALLOWED     = -140, /* Port can not be operated in 10G speed when slot port is > 8 */
    PORTCFG_PORTNAME_NULL                   = -141, /* Portname is NULL */
    PORTCFG_DELIMITER                       = 0x0000ffff



} portCfgCode_t;





typedef struct port_cfg_arg {
    int cmd;                    /* configure speed or long distance command */
    int level;                  /* arguments to the command */
    int distance;               /* link distance */
    int port_bm[MAX_PORT_BM];   /* bitmap of user ports that are degraded */
} port_cfg_arg_t;






#define PORTCFG_RETURN_STR_MAXLEN               256

typedef enum {
        PORTCFG_CMD_SPEED = 0,
        PORTCFG_CMD_LONGDISTANCE,
        PORTCFG_CMD_QOS,
        PORTCFG_CMD_CFGDEFAULT,
        PORTCFG_CMD_FPORT_BUFFER
} portCfgCmdType_t;

typedef struct _portCfgRet_ {
        portCfgCode_t   type;
        char                    entry[PORTCFG_RETURN_STR_MAXLEN];
} portCfgRet_t;


//static portCfgRet_t cfgRet;
/*
 * Define new distance levels here.
 * For each new entry define a string
 * in port_distance array in portcmds.c
 */
#define PORT_DISTANCE_TOTAL     7   /* total levels */


/* Used in static port_distance array */
#define INVLD_DIST_STRING   "??"
#define INVLD_DIST_DESCRN   "unknown"

#define PORT_ENOUGH_BUFFER      0
#define PORT_INSUFFICIENT_BUFFER    1


static inline  int
fportValidateConfig(int sw_handle, int port, port_cfg_arg_t *port_arg,
        portCfgCode_t *errCode)
{
        portIoc_t arg;

        memset(&arg, 0, sizeof (portIoc_t));

        arg.port = port;
        arg.result = 0;
       //arg.arg1 = (int)(port_arg;
       //arg.arg2 = (int)errCode;

        if (ioctl(sw_handle, IOC_P_CHECK_PORT_CFG, &arg) == -1) {
                return (-1);
        }
        return (arg.result);
}

static  inline int
portValidateConfig(int port, port_cfg_arg_t *port_arg, portCfgCode_t *rcode)
{
    return (fportValidateConfig(FabOS_SW_FD, port, port_arg, rcode));
}



static inline int
fportTxPerf(int port, uint32_t *perf)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(FabOS_SW_FD, IOC_P_TX_PERF, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}


static inline int portTxPerf(int port, uint32_t *perf)
{
    return (fportTxPerf(port,perf));
}


static port_dist_t port_distance[PORT_DISTANCE_SIZE] = {
    {"L0",          "L0",           "normal"},
    {"L1",          "L1",           "long <= 50km"},
    {"L2",          "L2",           "super long <= 100km"},
    {"LE",          "LE",           "standard <= 10km"},
    {"L0.5",        "LM",           "medium long <= 25km"},
    {"LD",          "LD",           "auto"},
    {"LS",          "LS",           "static"},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN},
    {INVLD_DIST_STRING, INVLD_DIST_STRING,  INVLD_DIST_DESCRN}
};

static inline const char* portDistance2chCode(int level)
{
    if (level >= PORT_DISTANCE_NORMAL && level < PORT_DISTANCE_SIZE)
            return (port_distance[level].dist_level_2ch_code);
    else
        return (INVLD_DIST_STRING);
}

static inline int
fportGetCfgBm(int sw_handle, int port, pcfg_bm_t *pcfg_bm)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;
    //arg.arg1 = (int)pcfg_bm;
        if (ioctl(sw_handle, IOC_P_GET_CFG_BM, &arg) == -1) {
                return (-1);
        }

        return (arg.result);

}

static inline int
fportState(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PORT_STATE, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportDiag(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_DIAG, &arg) == -1) {
        return (0);
    }
    return (arg.result);
}
typedef struct buf_usage {
    int num_ports_per_grp;      /* for bloom, this will be 4; for condor, 32 */
    int total_bufs_per_grp;     /* for bloom this will be 108; condor, 1024 */
    int alloc_bufs;             /* number of buffers allocated for this port */
    int unused_bufs;            /* number of buffers left unused for this grp */
    int desired_bufs;           /* number of buffers desired for the port */
    int user_est_bufs;          /* number of buffs needed for user est link */
    int num_user_ports;         /* number of user ports in this group       */
    int internal_usage;         /* buffer used by internal ports            */
    unsigned long port_bm[MAX_PORT_BM]; /* bitmap of user ports in group    */
} buf_usage_t;                  /*    distance                              */


#define MODNAME(name) mod_##name	
#define fcport_defined(port)     1


static inline int
fportBufferAlloc(int sw_handle, int port, void *buffer)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    //arg.arg1 = (int)buffer;
    arg.arg2 = arg.arg3 = 0;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_GET_BUF_ALLOC, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}


static inline int
fportLxDistance(int sw_handle, int port, int *actual, int *estimated)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    //arg.arg1 = (int)actual;
    //arg.arg2 = (int)estimated;
    arg.arg3 = 0;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_GET_LX_DIST, &arg) == -1) {
        return (0);
    }
    return (arg.result);
}


static inline mmap_port_info_t *portGetMemMap(int port)
{
    mmap_port_info_t    *pt_info;
    mmap_switch_info_t  *sw_info = MMAP_SWITCH_INFO(mmap_info);

    if (sw_info == NULL) {
        return (NULL);
    }

    if ((port >= LGC_PT_MAX(sw_info)) || (port < 0)) {
        return (NULL);
    }

    pt_info = MMAP_PORT_INFO(mmap_info, port);
    /*
     *This check is necessary because we use single piece of memory for
     *a port irrespective of the switch it is attached to. So if called
     *from an unrelated switch, this function must return NULL so that
     *a logical switch will not use this port unless it is actually meant
     *for it.
     */
    if (!fcport_defined(port)) {
        return (NULL);
    }

    /* if port is absent, return NULL */
    if (!(pt_info->p_flags & P_PRESENT)) {
        return (NULL);
    }

    /* Internal ports are ONLY manageable and local to the Switch driver */
    if (pt_info->p_is_port_internal == TRUE) {
        return (NULL);
    }

    return (pt_info);
}

static inline int portId(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (-ENOENT);
    }
    return (pt_info->p_id);
}



/*
 * function:
 *  portLxDistance
 *
 * description:
 *  Retrieves the connecting distance of the port.
 *
 * inputs:
 *  port    - port number in the switch
 *
 * outputs:
 *  actual    - actual distance in km.
 *  estimated - estimated distance in km.
 *
 * returns:
 *  0 on success; -1 on failure
 */
static inline int portLxDistance(int port, int *actual, int *estimated)
{
    return (fportLxDistance(FabOS_SW_FD, port, actual, estimated));
}


/*
 * function:
 *  portBufferAlloc
 *
 * description:
 *  Retrieves the port buffer allocations within the port group.
 *
 * inputs:
 *  port    - port number in the switch.
 *
 * outputs:
 *  buffer  - structure to return buffer allocation values
 *
 * returns:
 *  0 on success; -1 on failure
 */
static inline int portBufferAlloc(int port, buf_usage_t *buffer)
{
    return (fportBufferAlloc(FabOS_SW_FD, port, buffer));
}

static inline int
fportClrStats(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_CLR_STATS, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}
static inline int portActive(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_flags & P_ACTIVE);
}

static inline int portGetActualLinkSpeed(int port)
{
	return (0);
}

static inline int
fportGetSegmentReason(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_GET_SGMT_RSN, &arg) == -1) {
        return (0);
    }
    return (arg.result);
}

/*
 * function:
 *  portGetSegmentReason
 *
 * description:
 *  Retrieves the segmented reason for a port. It is caller's
 * responsibility to check the port is a segmented eport. If the port is not
 * an e-port or is not segmented, this API will simply return 0 without
 * reporting any error
 *
 * inputs:
 *  port    - port number in the switch
 *
 * outputs:
 *  None
 *
 * returns:
 *   -1 on failure, otherwise, reason code
 */
static inline int portGetSegmentReason(int port)
{
    return (fportGetSegmentReason(FabOS_SW_FD, port));
}



static inline int portTmasterport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_TMASTER);
}

static inline int portTport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_TPORT);
}


static inline int portSegmented(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_SEGMENTED);
}

static inline int
fportEportDisabled(int sw_handle, int port)
{
    pcfg_bm_t pcfg_bm;

    if (fportGetCfgBm(sw_handle, port, &pcfg_bm) == -1) {
        return (-1);
    }
    return (PORTCFG_EPORTDISABLE(pcfg_bm));
}

static inline int portTFport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_TFPORT);
}


static inline int
fportPhysNoCard(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_NO_CARD, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysNoModule(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_NO_MOD, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysLaserFalt(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_LAS_FLT, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}


static inline int
fportPhysNoLight(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_NO_LYT, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysNoSync(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_NO_SYNC, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysInSync(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_IN_SYNC, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysPortFalt(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_PORT_FLT, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysDiagFalt(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_DIAG_FLT, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysLockRef(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_LOCK_REF, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}


static inline int
fportPhysModInv(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_MOD_INV, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysModVal(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_MOD_VAL, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}

static inline int
fportPhysNoSigDet(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_PHY_NO_SIG_DET, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}


static inline const char *
fportPhysName(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;

    if (fportPhysNoCard(sw_handle, port)) {
        return ("No_Port  ");
    }
    if (fportPhysNoModule(sw_handle, port)) {
        return ("No_Module");
    }
    if (fportPhysLaserFalt(sw_handle, port)) {
        return ("Laser_Flt");
    }
    if (fportPhysNoLight(sw_handle, port)) {
        return ("No_Light ");
    }
    if (fportPhysNoSync(sw_handle, port)) {
        return ("No_Sync  ");
    }
    if (fportPhysInSync(sw_handle, port)) {
        return ("In_Sync  ");
    }
    if (fportPhysPortFalt(sw_handle, port)) {
        return ("Port_Flt ");
    }
    if (fportPhysDiagFalt(sw_handle, port)) {
        return ("Diag_Flt ");
    }
    if (fportPhysLockRef(sw_handle, port)) {
        return ("Lock_Ref ");
    }
    if (fportPhysModInv(sw_handle, port)) {
        return ("Mod_Inv");
    }
    if (fportPhysModVal(sw_handle, port)) {
        return ("Mod_Val");
    }
    if (fportPhysNoSigDet(sw_handle, port)) {
        return ("No_SigDet");
    }
    return ("UNKNOWN  ");
}


static inline int
fportGetDisableReasonStr(int sw_handle, int port, char *reason)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    //arg.arg1 = (int)reason;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_GET_DISABLE_REASON, &arg) == -1) {
        
        return (-1);
    }

    return (arg.result);
}


static inline const char *
fportStateName(int sw_handle, int port)
{
    switch (fportState(sw_handle, port)) {
        case UNKNOWN:   return ("         ");
        case ONLINE:    return ("Online   ");
        case OFFLINE:   return ("Offline  ");
        case TESTING:   return ("Testing  ");
        case FAULTY:    return ("Faulty   ");
        case E_PORT:    return ("E_Port   ");
        case F_PORT:    return ("F_Port   ");
        case SEGMENTED: return ("Segmented");
    }
    return ("Unknown  ");
}

static inline int
fportDisabled(int sw_handle, int port)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(sw_handle, IOC_P_DSBL, &arg) == -1) {
        
        return (0);
    }
    return (arg.result);
}


static inline reason_t  portGetDisableReason(int port)
{
    mmap_port_info_t    *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (R_INVALID_REASON);
    }
    return (pt_info->p_disable_reason);
}

static inline int portPhyEport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_EPORT);
}

static inline int portEport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }

    if ((pt_info->p_flags & P_TNEGOTIATING) ||
            (pt_info->p_type & P_LEPORT)) {
        return (0);
    }

    if (pt_info->p_type & P_TPORT) {
        return ((pt_info->p_type & P_EPORT) &&
            (pt_info->p_type & P_TMASTER));
    } else {
        return (pt_info->p_type & P_EPORT);
    }
}

static inline int portUport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_UPORT);
}

static inline int portPhysFport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_FPORT);
}

static inline int portFport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }

    if (pt_info->p_flags & P_TNEGOTIATING) {
        return (0);
    }

    if (pt_info->p_type & P_TFPORT) {
        return ((pt_info->p_type & P_FPORT) &&
            (pt_info->p_type & P_TFMASTER));
    } else {
        return (pt_info->p_type & P_FPORT);
    }
}

static inline int portGport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_GPORT);
}

static inline int portLport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_LPORT);
}

static inline int portTFmasterport(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (0);
    }
    return (pt_info->p_type & P_TFMASTER);
}
static inline int portState(int port)
{
    mmap_port_info_t *pt_info = portGetMemMap(port);

    if (pt_info == NULL) {
        return (-ENOENT);
    }
    return (pt_info->p_state);
}


static inline
wwn_t *getNodeName(opaque_t handle)
{
	static wwn_t nnwwn;
	nnwwn.wwn[3]=(0xff & handle);
	nnwwn.wwn[6]=(0xff & (handle>>8));
	nnwwn.wwn[5]=(0xff & (handle>>16));
	nnwwn.wwn[0]=0x27;
    return &nnwwn;

}

static inline 
wwn_t *getPortName( int port, wwn_t *wwn)
{
	uint32_t handle=0x12131415;
	static wwn_t pnwwn;
	pnwwn.wwn[3]=(0xff & handle);
	pnwwn.wwn[6]=(0xff & (handle>>8));
	pnwwn.wwn[5]=(0xff & (handle>>16));
	pnwwn.wwn[1]=(0xff & port);
	pnwwn.wwn[4]=(0xff & (port>>8));
	pnwwn.wwn[0]=0x2b;
	pnwwn.wwn[2]=0x5;
	pnwwn.wwn[7]=0x21;
	pnwwn.wwn[5]=0x23;

    return &pnwwn;
}


static inline 
wwn_t *getNpNodeName(int port,  wwn_t *wwn)
{
	uint32_t sw=0x1a2a3a3b;
	static wwn_t npnwwn;
	npnwwn.wwn[3]=(0xff & sw);
	npnwwn.wwn[6]=(0xff & (sw>>8));
	npnwwn.wwn[5]=(0xff & (sw>>16));
	npnwwn.wwn[1]=(0xff & port);
	npnwwn.wwn[4]=(0xff & (port>>8));
	npnwwn.wwn[0]=0x2b;
	npnwwn.wwn[2]=0x5;
	npnwwn.wwn[7]=0x2c;
    return &npnwwn;
}

static inline
wwn_t *getNpPortName(int port,  wwn_t *wwn)
{
	uint32_t sw=0x29286a3b;
	static wwn_t npnwwn;
	npnwwn.wwn[3]=(0xff & sw);
	npnwwn.wwn[6]=(0xff & (sw>>8));
	npnwwn.wwn[5]=(0xff & (sw>>16));
	npnwwn.wwn[1]=(0xff & port);
	npnwwn.wwn[4]=(0xff & (port>>8));
	npnwwn.wwn[0]=0x2b;
	npnwwn.wwn[2]=0x5;
	npnwwn.wwn[7]=0x2c;
    return &npnwwn;
}

static inline 
int fwPortStatesGet(portStateArray_t *callerArry)
{
	memset(callerArry, 0, sizeof(portStateArray_t));
	return (0);
}

#define    IOC_P_RX_PERF    _IO(PT_IOC_MAGIC, 68)

static inline int
fportRxPerf(int port, u_int*  perf)
{
    portIoc_t arg;

    memset(&arg, 0, sizeof (portIoc_t));

    arg.port = port;
    arg.result = 0;

    if (ioctl(FabOS_SW_FD, IOC_P_RX_PERF, &arg) == -1) {
        return (-1);
    }
    return (arg.result);
}


static inline int portRxPerf(int port, u_int*  perf)
{
    return (fportRxPerf(port, perf));
}

#endif /* __PORT_H__ */
