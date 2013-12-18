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
 *
 *    Copyright (c) 1996-2004 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *      Port layer adminstration and management interface
 *
 */

#ifndef __SYS_ADMIN_H__
#define	__SYS_ADMIN_H__
#if 0
#include <sys/fabos/cdefs.h>
#include <sys/fc/wwn.h>
#include <ipsport/ips_port_trace.h>

#if defined(__KERNEL__) && !defined(RTE_UNIT_TEST)
#include <linux/time.h>
#else
#include <sys/time.h>
#include <inttypes.h>
#endif

OS_CDECLS_BEGIN
#endif
/*
 * Adminstration command codes:
 */
#define PADM_CLR_STATS		0x50	/* Clear port statistics history */
#define PADM_GET_CFG		0x51	/* Get fcFxPortCfgType entry */
#define PADM_GET_CAP		0x52	/* Get fcFxPortCapType entry */
#define PADM_GET_FLOGI		0x53	/* Get fcNxPortFlogiType entry */
#define PADM_GET_C3ACC		0x54	/* Get fcFxPortC3AcctType entry */
#define PADM_GET_C2ACC		0x55	/* Get fcFxPortC2AcctType entry */
#define PADM_GET_C1ACC		0x56	/* Get fcFxPortC1AcctType entry */
#define PADM_GET_ERRS		0x57	/* Get fcFxPortErrorType entry */
#define PADM_GET_OPER		0x58	/* Get fcFxPortOperType entry */
#define PADM_GET_PHYS		0x59	/* Get fcFxPortPhysType entry */
#define PADM_GET_FXSTS		0x5a	/* Get fcFxPortStatsType entry */
#define PADM_GET_DEVCNT		0x5c	/* Get device counts (loop only) */
#define PADM_GET_FXINTRS	0x5d	/* Get swFxPortIntrsType entry */
#define PADM_GET_FDET		0x5e	/* Get failure detection entry */
#define PADM_GET_HWERRS		0x5f	/* Get fcFxPortHwErrorType entry */

#define	FEM_COSCAP_CLASS_F	0x01
#define	FEM_COSCAP_CLASS_1	0x02
#define	FEM_COSCAP_CLASS_2	0x04
#define	FEM_COSCAP_CLASS_3	0x08
#define	FEM_COSCAP_CLASS_4	0x10
#define	FEM_COSCAP_CLASS_5	0x20
#define	FEM_COSCAP_CLASS_6	0x40
#define	FEM_COSCAP_RESERVED	0x80

#define	FEM_BAUDCAP_OTHER	0x01
#define	FEM_BAUDCAP_EIGHTH	0x02
#define	FEM_BAUDCAP_QUARTER	0x04
#define	FEM_BAUDCAP_HALF	0x08
#define	FEM_BAUDCAP_FULL	0x10
#define	FEM_BAUDCAP_DOUBLE	0x20
#define	FEM_BAUDCAP_TRIPLE	0x30
#define	FEM_BAUDCAP_QUAD	0x40
#define	FEM_BAUDCAP_NEGOTIATE	0x80
#define	FEM_BAUDCAP_NEG_FULL	0x100	/* negotiated 1Gbps */
#define	FEM_BAUDCAP_NEG_DOUBLE	0x200	/* negotiated 2Gbps */
#define	FEM_BAUDCAP_NEG_QUAD	0x400	/* negotiated 4Gbps */
#define	FEM_BAUDCAP_XG		0x800	/* 10Gbps */
#define	FEM_BAUDCAP_8G		0x1000	/* 8Gbps */
#define	FEM_BAUDCAP_NEG_8G	0x2000	/* negotiated 8Gbps */
#define FEM_BAUDCAP_NA		0x8000  /* not applicable */
#define FEM_BAUDCAP_NEG_XG	0x10000	/* negotiated 10Gbps */

#define	FEM_MEDIUMCAP_UNKNOW	0x0001
#define	FEM_MEDIUMCAP_SM	0x0002
#define	FEM_MEDIUMCAP_M5	0x0004
#define	FEM_MEDIUMCAP_M6	0x0008
#define	FEM_MEDIUMCAP_TV	0x0010
#define	FEM_MEDIUMCAP_MI	0x0020
#define	FEM_MEDIUMCAP_STP	0x0040
#define	FEM_MEDIUMCAP_TW	0x0080
#define	FEM_MEDIUMCAP_LV	0x0100

#define	FEM_TOPOCAP_NONE	0
#define	FEM_TOPOCAP_P2P		0x01
#define	FEM_TOPOCAP_LOOP	0x02
#define	FEM_TOPOCAP_UNIVERSAL	0x03

#define	FEM_STACKEDMODE_NONE	1
#define	FEM_STACKEDMODE_XPARENT	2
#define	FEM_STACKEDMODE_LOCKED	3

#define	FEM_TXTYPE_UNKNOW	1
#define	FEM_TXTYPE_LL		2
#define	FEM_TXTYPE_SL		3
#define	FEM_TXTYPE_LE		4
#define	FEM_TXTYPE_EL		5
#define	FEM_TXTYPE_SN		6

#define	FEM_DISTANCE_UNKNOWN	1
#define	FEM_DISTANCE_LONG	2
#define	FEM_DISTANCE_MEDIUM	3
#define	FEM_DISTANCE_SHORT	4

#define	FEM_PORTMOD_AUTO	1
#define	FEM_PORTMOD_FPORT	2
#define	FEM_PORTMOD_FLPORT	3

#define	FEM_BBCMOD_REG		1
#define	FEM_BBCMOD_ALT		2
#define TRACE_NUM_MOD 20

typedef unsigned long long u_int64;

typedef struct {
	wwn_t	fcFxPortName;
	long	fcFxPortFcphVersionHigh;
	long	fcFxPortFcphVersionLow;
	long	fcFxPortBbCredit;
	long	fcFxPortRxBufSize;
	long	fcFxPortRatov;
	long	fcFxPortEdtov;
	long	fcFxPortCosSupported;
	long	fcFxPortIntermixSupported;
	long	fcFxPortStackedConnMode;
	long	fcFxPortClass2SeqDeliv;
	long	fcFxPortClass3SeqDeliv;
	long	fcFxPortHoldTime;
	long	fcFxPortBaudRate;
	long	fcFxPortMedium;
	long	fcFxPortTxType;
	long	fcFxPortDistance;
} fcFxPortCfgType;	/* GET_CFG */

typedef struct {
	long	fcFxPortCapFcphVersionHigh;
	long	fcFxPortCapFcphVersionLow;
	long	fcFxPortCapBbCreditMax;
	long	fcFxPortCapBbCreditMin;
	long	fcFxPortCapRxDataFieldSizeMax;
	long	fcFxPortCapRxDataFieldSizeMin;
	long	fcFxPortCapCos;
	long	fcFxPortCapIntermix;
	long	fcFxPortCapStackedConnMode;
	long	fcFxPortCapClass2SeqDeliv;
	long	fcFxPortCapClass3SeqDeliv;
	long	fcFxPortCapHoldTimeMax;
	long	fcFxPortCapHoldTimeMin;
	long	fcFxPortCapBaudRates;
	long	fcFxPortCapMedia;
	long	fcFxPortCapTopologies;
} fcFxPortCapType;		/* GET_CAP */

typedef struct {
	long	fcFlogiNxPortIndex;		/* Use AL_PA index */
	long	fcFlogiNxPortFcphVersionAgreed;
	long	fcFlogiNxPortBbCredit;
	long	fcFlogiNxPortRxDataFieldSize;
	long	fcFlogiNxPortCosSuppAgreed;
	long	fcFlogiNxPortIntermixSuppAgreed;
	long	fcFlogiNxPortStackedConnModeAgreed;
	long	fcFlogiNxPortClass2SeqDelivAgreed;
	long	fcFlogiNxPortClass3SeqDelivAgreed;
	wwn_t	fcFlogiNxPortPortName;
	long	fcFlogiNxPortConnected;
	long	fcFlogiNxPortBbCreditModel;
} fcFlogiNxPortType;		/* GET_FLOGI */

typedef struct {
	long	fcFxPortC1InConnections;
	long	fcFxPortC1OutConnections;
	long	fcFxPortC1FbsyFrames;
	long	fcFxPortC1FrjtFrames;
	long	fcFxPortC1ConnTime;
	long	fcFxPortC1InFrames;
	long	fcFxPortC1OutFrames;
	long	fcFxPortC1InOctets;
	long	fcFxPortC1OutOctets;
	long	fcFxPortC1Discards;
} fcFxPortC1AcctType;		/* GET_C1ACC */

typedef struct {
	long	fcFxPortC2InFrames;
	long	fcFxPortC2OutFrames;
	long	fcFxPortC2InOctets;
	long	fcFxPortC2OutOctets;
	long	fcFxPortC2Discards;
	long	fcFxPortC2FbsyFrames;
	long	fcFxPortC2FrjtFrames;
} fcFxPortC2AcctType;		/* GET_C2ACC */

typedef struct {
	long	fcFxPortC3InFrames;
	long	fcFxPortC3OutFrames;
	long	fcFxPortC3InOctets;
	long	fcFxPortC3OutOctets;
	long	fcFxPortC3Discards;
} fcFxPortC3AcctType;		/* GET_C3ACC */

typedef struct {
	long	fcPortListAhits;
	long	fcPortListAdiscards;
	long	fcPortListBhits;
	long	fcPortListBdiscards;
	long	fcPortListDhits;
	long	fcPortListDdiscards;
	long	fcPortListOtherhits;
	long	fcPortListOtherdiscards;
} fcPortFltAcctType;

typedef struct {
	long	fcFxPortLinkFailures;
	long	fcFxPortSyncLosses;
	long	fcFxPortSigLosses;
	long	fcFxPortPrimSeqProtoErrors;
	long	fcFxPortInvalidTxWords;
	long	fcFxPortInvalidCrcs;
	long	fcFxPortDelimiterErrors;
	long	fcFxPortAddressIdErrors;
	long	fcFxPortLinkResetIns;
	long	fcFxPortLinkResetOuts;
	long	fcFxPortOlsIns;
	long	fcFxPortOlsOuts;
} fcFxPortErrsType;		/* GET_ERRS */

/*
 * Note: fcFxPortHwErrsType should match ss_grp_errsts_t in the ASIC driver.
 */
typedef struct {
	long	fcFxPortHwEncInErrors;
	long	fcFxPortHwCrcErrors;
	long	fcFxPortHwTruncErrors;
	long	fcFxPortHwTooLongErrors;
	long	fcFxPortHwBadEofErrors;
	long	fcFxPortHwEncOutErrors;
	long	fcFxPortHwBadOsErrors;
	long	fcFxPortHwDiscC3Errors;
	long	fcFxPortHwTxC3ToErrors;
	long	fcFxPortHwRxC3ToErrors;
	long	fcFxPortHwRxUnrchErrors;
} fcFxPortHwErrsType;		/* GET_HWERRS */

typedef struct {
	long	fcFLPortLipIns;
	long	fcFLPortLipOuts;

	struct timespec	fcFLPortLastSyncLoss;
	long	fcFLPortSyncLossFreqCnt;

	struct timespec	fcFLPortLastLip;
	long	fcFLPortLipFreqCnt;
	u_short	fcFLPortLastLipAlpa;
} fcFLPortErrsType;		/* ADMIN_GET_FLERR */

typedef struct {
	long	fcFxPortID;
	wwn_t	fcFPortAttachedPortName;	/* Class 1 only */
	long	fcFPortConnectedPort;		/* Class 1 only */
	long	fcFxPortBbCreditAvailable;
	long	fcFxPortOperMode;
	long	fcFxPortAdminMode;
} fcFxPortOperType;		/* GET_OPER */

typedef struct {
	long	fcFxPortPhysAdminStatus;
	long	fcFxPortPhysOperStatus;
	long	fcFxPortPhysLastChange;
	long	fcFxPortPhysRttov;
} fcFxPortPhysType;		/* GET_PHYS */

typedef struct {
	u_int	swFxPortTxWords;
	u_int   swFxPortRxWords;
	u_int	swFxPortTxFrames;
	u_int	swFxPortRxFrames;
	u_int	swFxPortRxC2Frames;
	u_int	swFxPortRxC3Frames;
	u_int	swFxPortRxLCs;
	u_int	swFxPortRxMcasts;
	u_int	swFxPortTooManyRdys;
	u_int	swFxPortNoTxCredits;
	u_int	swFxPortRxEncInFrs;
	u_int	swFxPortRxCrcs;
	u_int	swFxPortRxTruncs;
	u_int	swFxPortRxTooLongs;
	u_int	swFxPortRxBadEofs;
	u_int	swFxPortRxEncOutFrs;
	u_int	swFxPortRxBadOs;
	u_int	swFxPortC3Discards;
	u_int	swFxPortMcastTimedOuts;
	u_int	swFxPortTxMcasts;
	u_int	swFxPortRxCrcsBadEof;

	/* statistics for LOOM based ASICs */
	u_int	swFlPortOpen;
	u_int	swFlPortXfer;
	u_int	swFlPortOpened;
	u_int	swFlPortOneTenancy;
	u_int	swFlPortLessTenancy;
	u_int	swFlPortMoreTenancy;
	u_int	swFlPortZeroTenancy;
	u_int	swFxPortStarveStop;
	u_int	swFlPortFlTenancy;
	u_int	swFlPortNlTenancy;
	u_int	swFxPortFrameNoZone;
	u_int	swFxPortBcrcGeof;
	u_int	swFxPortReserved1;

	/* statistics for Loop Initialization */
	u_int	swFxPortLipIns;
	u_int	swFxPortLipOuts;
	u_int	swFxPortLipLastAlpa;
} swFxPortStatsType;		/* GET_FXSTTS */

/*
 * The change in the structure member needs also to
 * update the string table in the portcmds.c for
 * printing the strings accordingly.
 */
struct ipStatsType_t {
	u_int	ipaddr;					/* IP address */
	u_int64   ip_out_comp_octets;		/* num of comp octets xmitted */
	u_int64   ip_out_uncomp_octets;	/* num of uncomp octets xmitted */
	u_int64   ip_out_pkts;			/* num of pkts xmitted */
	u_int64   ip_out_ucast_pkts;		/* num of unicast pkts xmitted */
	u_int64   ip_out_bcast_pkts;		/* num of broadcast pkts xmitted */
	u_int64   ip_out_mcast_pkts;	/* num of multicast pkts xmitted */
	u_int64   ip_in_comp_octets;	/* num of comp octets rcvd */
	u_int64   ip_in_uncomp_octets;	/* num of uncomp octets rcvd */
	u_int64   ip_in_pkts;				/* num of pkts rcvd */
	u_int64   ip_in_ucast_pkts;		/* num of unicast pkts rcvd */
	u_int64   ip_in_bcast_pkts;		/* num of broadcast pkts rcvd */
	u_int64   ip_in_mcast_pkts;		/* num of multicast pkts rcvd */
	u_int     ip6addr[4];        /* IPv6 address */
	char      flags;
#define STATS_IPV4 0x80
#define STATS_IPV6 0x40
	char      reserved[3];
} __attribute__ ((__packed__));

typedef struct ipStatsType_t ipStatsType;

/*
 * The change in the structure member needs also to
 * update the string table in the portcmds.c for
 * printing the strings accordingly.
 */
typedef struct {
	ipStatsType ip_stats[8];
	u_int   ip_err_crc;				/* num of CRC errors */
	u_int   ip_err_hdr_cksum;		/* num of chksum errors */
	u_int   ip_err_tcp_data_chksum;	/* num of TCP data chksum errors */
} gePortIpStatsType; 	/* IOC_P_GET_IP_STAT */

/*
 * The change in the structure member needs also to
 * update the string table in the portcmds.c for
 * printing the strings accordingly.
 */
struct gePortFcipStats_t {
	u_int		tunnel_id;				/* Tunnel ID */
	u_int64	fcip_ip2fc_bytes;	/* uncompressed bytes	*/
	u_int64	fcip_ip2fc_pkts;	/* pkt received by fcip entity from ip */
	u_int		fcip_ip2fc_wantov_drop;	/* pkt dropped due to wantov */
	u_int64	fcip_fc2ip_pkts;		/* ve to fcip_entity pkts */
	u_int64	fcip_fc2ip_bytes;		/* ve to fcip_entity bytes */
	u_int		fcip_io_accel;			/* num of io accelerated    */
} __attribute__ ((__packed__));

typedef struct gePortFcipStats_t gePortFcipStats;

typedef struct {
	gePortFcipStats	fcip_stats[8];
} gePortFcipStatsType;	/* IOC_P_GET_FCIP_STAT */

struct gePortStatsType_t {
	u_int64   ge_stat_tx_frms;			/* num of tx frames */
	u_int64   ge_stat_tx_octets;		/* num of tx octets */
	u_int64   ge_stat_tx_ucast_frms; /* num of tx unicast frames */
	u_int64   ge_stat_tx_mcast_frms; /* num of tx multicast frames */
	u_int64   ge_stat_tx_bcast_frms; /* num of tx broadcast frames */
	u_int64   ge_stat_tx_vlan_frms;		/* num of tx VLAN frames */
	u_int   ge_stat_tx_pause_frms;		/* num of tx pause frames */
	u_int64   ge_stat_rx_frms;			/* num of rx frames */
	u_int64   ge_stat_rx_octets;			/* num of rx octets */
	u_int64   ge_stat_rx_ucast_frms; /* num of rx unicast frames */
	u_int64   ge_stat_rx_mcast_frms; /* num of rx multicast frames */
    u_int64   ge_stat_rx_bcast_frms; /* num of rx broadcast frames */
	u_int64   ge_stat_rx_vlan_frms;		/* num of rx VLAN frames */
	u_int   ge_stat_rx_pause_frms;		/* num of rx pause frames */
	u_int   ge_err_carrier;				/* num of carrier loss */
	u_int   ge_err_collision;			/* num of collision */
	u_int   ge_err_length;				/* num of length error pkts */
	u_int   ge_err_crc;					/* num of crc errors */
	u_int   ge_err_dribbling;			/* num of dribbling bit errors */
	u_int   ge_err_runt;				/* num of runt pkts */
	u_int   ge_err_jabber;				/* num of jabber */
	u_int   ge_err_abort;				/* num of abort pkts */
	u_int   ge_err_overrun;				/* num of overrun */
	u_int   ge_err_fifo_ovf;			/* num of fifo overflow */
} __attribute__ ((__packed__));

typedef struct gePortStatsType_t gePortStatsType;		/* IOC_P_GET_GE_STAT */

typedef struct {
	u_int		sid;		/* sequence no		*/
	char		tdata[128];	/* string message	*/
} tentries;					/* 1250 Trace Buffer Entries*/

typedef struct {
	u_int		b_entr;		/* begining entry in pci buffer*/
	u_int		e_entr;		/* ending entry in pci buffer*/
	tentries	tentr[128];	/* local buffer for retrieving data */
} gePortTBufferType;		/*  IOC_P_GET_TRACE_BUF */

/*
 * size of trace buffer entries for ironman should be less
 * than IBD_MAX_IOCTLDATA_SIZE (ibd_intl_cmd.h)
 */
#define ISCSI_PORT_TRACE_ENTRIES	22
typedef struct {
	u_int		b_entr;		/* begining entry in pci buffer*/
	u_int		e_entr;		/* ending entry in pci buffer*/
	tentries	tentr[ISCSI_PORT_TRACE_ENTRIES];	/* local buffer for
													 * retrieving data */
} iscsiGePortTBufferType;		/*  IOC_P_GET_TRACE_BUF */



typedef struct {
	u_int		buffer;		/* buffer flag		*/
	u_int		console;	/* console flag		*/
	u_int		flag[TRACE_NUM_MOD];	/* 1250 module flag	*/
	u_int		cur_entr;	/* current entry pointer */
	u_int		seq_no;		/* seqence no		*/
	u_int		roll_over;	/* no of roll over	*/
} gePortTBufferStatType;	/* IOC_P_GET_TRACE_STAT	*/


/* failure detection status */
typedef struct _fdet_status {
	u_int	ministate_stat;	/* minibuffer state checking status */
	u_short	faildet_pstat;	/* fail detect ram parity status */
	u_short	frmtrk_prtstat;	/* Frame tracking port status */
	u_int	less_n;		/* Indicates buffer tags status */
	u_int	frmtrk_failinfo[2]; /* frame tracking fail info */
} fdet_status_t;

/* Flow control counters */
#define MAX_QOS_PRI_LEVELS		8
typedef struct _if_flowctrl_ctrs {
    uint64_t rx_pause_time_priority[MAX_QOS_PRI_LEVELS];
    uint64_t tx_pause_frames;
} if_flowctrl_ctrs_t;

/* CoS counters */
typedef struct _if_cos_ctrs {
	uint64_t rx_priority_frames[MAX_QOS_PRI_LEVELS];
	uint64_t rx_priority_dw[MAX_QOS_PRI_LEVELS];
	uint64_t tx_priority_frames[MAX_QOS_PRI_LEVELS];
	uint64_t tx_priority_dw[MAX_QOS_PRI_LEVELS];
	/*	for eAvnil only	*/
	uint64_t rx_untagged_frames;
	uint64_t rx_untagged_dw;
	uint64_t tx_untagged_frames;
	uint64_t tx_untagged_dw;
} if_cos_ctrs_t;

/* RFC 2819 RMON mib counters */
typedef struct _if_rmon_ctrs {
	uint64_t	rx_total_pkts;    	/* good + bad packets */
	uint64_t	tx_total_pkts;    	/* good + bad packets */
	uint64_t	rx_total_octets;
    uint64_t	tx_total_octets;
	uint64_t	good_pkts_rcv;
	uint64_t	mc_pkts_rcv;		/* good packets only */
	uint64_t	brdc_pkts_rcv;  	/* good packets only */
	uint64_t	uc_pkts_rcv;  		/* good packets only */
    uint64_t	bad_crc;
    uint64_t	undersize_pkts;
    uint64_t	oversize_pkts;
    uint64_t	fragments_pkts;
    uint64_t	jabber_pkts;
    uint64_t	rx_pkts_64_octets;
    uint64_t	rx_pkts_65_127_octets;
    uint64_t	rx_pkts_128_255_octets;
    uint64_t	rx_pkts_256_511_octets;
    uint64_t	rx_pkts_512_1023_octets;
    uint64_t	rx_pkts_1024_1518_octets;
    uint64_t	rx_pkts_gt_1518_octets; /* not in RMON */
    uint64_t	rx_symbol_errs;         /* not in ipi */
    uint64_t	rx_ifg_vol;				/* not in ipi */
	uint64_t	rx_pause_time;          /* not in ipi */
	uint64_t	rx_base_shim_errs;      /* not in ipi */
	uint64_t	rx_intra_frame_idles;   /* not in ipi */
    uint64_t	drop_events;
	uint64_t	rx_frlen_mismatch;      /* not in ipi */
    uint64_t	good_pkts_sent;
    uint64_t	mc_pkts_sent;			/* good pkts only */
    uint64_t	brdc_pkts_sent;			/* good pkts only */
    uint64_t	uc_pkts_sent;			/* good pkts only */
	uint64_t	bad_pkts_sent;			/* not in ipi */
	uint64_t	pause_pkts_sent;        /* not in ipi */
	uint64_t	oversize_xmit_drop;     /* oversized frame xmitted or discard */
	uint64_t	tx_pkts_64_octets;
	uint64_t	tx_pkts_65_127_octets;
	uint64_t	tx_pkts_128_255_octets;
	uint64_t	tx_pkts_256_511_octets;
	uint64_t	tx_pkts_512_1023_octets;
	uint64_t	tx_pkts_1024_1518_octets;
	uint64_t	tx_pkts_gt_1518_octets; /* not in RMON */
	uint64_t	tx_idles_underrun;
	uint64_t	tx_vlan_discard;
	uint64_t	rx_error_pkts;
	uint64_t	tx_error_pkts;
	uint64_t	rx_discard_pkts;
	uint64_t	tx_discard_pkts;
} if_rmon_ctrs_t;

typedef union if_mac_ctrs_ {
	/* RFC 2819 RMON mib counters */
	if_rmon_ctrs_t		rmon;
	/* Flow control counters */
	if_flowctrl_ctrs_t	flowctrl;
	/* COS counters */
	if_cos_ctrs_t		cos;
} if_mac_ctrs_t;

/* proc args structure for passing the arguments of proc to asic */
typedef struct _proc_args {
	char *buf;
	char **start;
	off_t pos;
	int count;
	int *eof;
} proc_args_t;

/*
 * statistics print out format string
 */
#define	STATS_FMT	"%-12s\t%-12u%s\n"


#endif /* __SYS_ADMIN_H__ */
