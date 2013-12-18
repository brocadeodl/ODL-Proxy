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
 *    Copyright (c) 1996-2011 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *      PTIO_* command codes header file
 *
 *		The PTIO_* command codes in this file used to be in this file:
 *			fabos/src/sys/include/asic/asic.h
 *		but they need to be extracted out to allow the 1250 microcode SW
 *		to share the same code. This file will still be included in asic.h.
 */

#ifndef __PTIO_CODES_H__
#define	__PTIO_CODES_H__

#include <sys/asic/asic_blk.h>

/* *********************************************************************
 * ioctl command codes
 *
 * The format of the IOCTL is as following:
 * Bits 31-24 -- ASIC functional block defined in asic_blks.h
 * Bits 23-21 -- unused
 * Bit  20    -- Flag for async ioctl (set dynamically)
 * Bits 19-16 -- Event type (1 - ioctl, 2 - intr, 3 - input, 4 - timer
 * Bits 15- 0 -- code based on the event type
 *
 ***********************************************************************/

/* ***** UPSM ***** */
#define	PTIO_SET_FLOWCTL	_ASIC_IOC(UPSM, 0x1)	/* E_Port flow ctrl mode */
#define PTIO_SFLAG_DIAG		_ASIC_IOC(UPSM, 0x2 | ASIC_SW_ACCESS)	/* Set diag flag */
#define PTIO_CFLAG_DIAG		_ASIC_IOC(UPSM, 0x3 | ASIC_SW_ACCESS)	/* Clear diag flag */
#define	DIAG_FLAG_RXEN		0x1		/* Enable/Disable diag RX mode */
#define DIAG_FLAG_CHIP_RESET    0x2		/* ASIC Driver must autonomously perform reset/initialization
						 * Value of 2 denotes that reset will be done. 
						 */

#define	PTIO_GET_VCCREDITS	_ASIC_IOC(UPSM, 0x4 | ASIC_SW_ACCESS)	/* Get E_Port VCs credits */
#define	PTIO_SET_VCCREDITS	_ASIC_IOC(UPSM, 0x5)	/* set E_Port VCs credits */
#define	PTIO_SET_LINE_LB	_ASIC_IOC(UPSM, 0x6)	/* Set line loopback port */
#define PTIO_GET_TOPO		_ASIC_IOC(UPSM, 0x7 | ASIC_SW_ACCESS)	/* Get port topology */
#define PTIO_SET_TOPO		_ASIC_IOC(UPSM, 0x8)	/* Set port topology */
#define	PTIO_LED			_ASIC_IOC(UPSM, 0x9)	/* LED control */
#define	PTIO_EPORT			_ASIC_IOC(UPSM, 0xa)	/* Port is an E_Port */
#define	PTIO_FPORT			_ASIC_IOC(UPSM, 0xb)	/* Port is an F_Port */
#define	PTIO_SEGMENT		_ASIC_IOC(UPSM, 0xc)	/* Port is segmented */
#define	PTIO_DISABLE		_ASIC_IOC(UPSM, 0xd)	/* Take port offline */
#define	PTIO_LINK_RESET		_ASIC_IOC(UPSM, 0xe)	/* Take port into LR*/
#define	PTIO_INIT			_ASIC_IOC(UPSM, 0xf)	/* Take port down then up */
#define PTIO_DISABLED		_ASIC_IOC(UPSM, 0x10)	/* Is a port disabled? */
#define PTIO_GET_PTYPE		_ASIC_IOC(UPSM, 0x11 | ASIC_SW_ACCESS)	/* Get port type */
#define	PTIO_MODE			_ASIC_IOC(UPSM, 0x12 | ASIC_SW_ACCESS)	/* Get mode of operation */
#define PTIO_RNID			_ASIC_IOC(UPSM, 0x13)	/* Set RNID related info */
/*
 * defect 52479 - Bloom has VC Compatibility issue on backend port with
 *	different chip type like Condor. Loading hardware VC values after
 * 	both Chips are up to solve this incompatibility issue and timing gap.
 */
#define	PTIO_RELOAD_BX_VCCREDITS	_ASIC_IOC(UPSM, 0x14)

/*
 * defect 49804:
 * Remove IOC_SM_SET_MODE (PTIO_SET_MODE).
 * This ioctl is used to update the STANDBY CP lb_mode.
 * There are three cases need to be considerated:
 * 1. diag case - diag will rerun after failover.
 * 2. Cable loop back/segmented - rerun after failover
 * 3. Normal situation - no need to update STANDBY site.
 */
#define	PTIO_TRUE_EPORT		_ASIC_IOC(UPSM, 0x15)	/* Port is true EPORT? */
/* XXX Need to revist the number */
#define PTIO_LOAD_CREDIT	_ASIC_IOC(UPSM, 0x16)	/* load be port credit */
#define PTIO_ACTIVATE_PORT	_ASIC_IOC(UPSM, 0x17)	/* activate the port */
#define PTIO_LED_MGMT		_ASIC_IOC(UPSM, 0x18)  /* update led mgmt ctrl */
	#define LED_ENABLE_MGMT		0x0
	#define LED_DISABLE_MGMT	0x1
	#define LED_CLI_DISABLE_MGMT	0x2
#define PTIO_RECOV_ONLINE	_ASIC_IOC(UPSM, 0x19)  /* HA recovery check */
#define PTIO_PORT_FAULT		_ASIC_IOC(UPSM, 0x1a)	/* Upper layer faulted port */
#define	PTIO_SET_IFG		_ASIC_IOC(UPSM, 0x1b)	/* set inter-frame gap */
#define	PTIO_GET_IFG		_ASIC_IOC(UPSM, 0x1c)	/* get inter-frame gap */
#define	PTIO_UNSET_LINE_LB	_ASIC_IOC(UPSM, 0x1d)	/* Unset line loopback port */
#define	PTIO_SET_FRAME_LB	_ASIC_IOC(UPSM, 0x1e)	/* Unset frame loopback port */
#define	PTIO_CHK_CONSIST	_ASIC_IOC(UPSM, 0x1f)	/* Check consistency bet */
													/* Condor-Egret ports */
#define	PTIO_GET_CR_RECOV	_ASIC_IOC(UPSM, 0x20 | ASIC_SW_ACCESS)	/* Get E_Port credits recov */
#define	PTIO_ENABLE_PRIM	_ASIC_IOC(UPSM, 0x21)	/* Enable primitives (Brocade-mode) */

/*
 * internal flex port related
 */
#define PTIO_CLEAR_TOPO		_ASIC_IOC(UPSM, 0x22)
#define	PTIO_SERDES_DOWN    _ASIC_IOC(UPSM, 0x23)	/* Power down serdes */
/* DCE Neighbor probe link reset */
#define	PTIO_DCE_LINK_RESET			_ASIC_IOC(UPSM, 0x24)	/* Link Reset for DCE */
#define	PTIO_ENABLE_PRIM_ONLINE		_ASIC_IOC(UPSM, 0x25)	/* Enable primitive when online */
#define	PTIO_DISABLE_PRIM_ONLINE	_ASIC_IOC(UPSM, 0x26)	/* Disable prim when online */

/* ***** PHYSM ***** */
#define	PTIO_GET_PHYSTATE	_ASIC_IOC(PHYSM, 0x1)	/* Get physical state */
#define	PTIO_SET_PHYSTATE	_ASIC_IOC(PHYSM, 0x2)	/* Set physical state */
#define	PTIO_GET_MODTYPE	_ASIC_IOC(PHYSM, 0x3)	/* Get GBIC type */
#define PTIO_MEDIA_STATE	_ASIC_IOC(PHYSM, 0x4)	/* Module val result */
#define PTIO_SET_PORT_BYPASS_MODE	_ASIC_IOC(PHYSM, 0x5)	/* Set or clear bypass mode */

#define PM_VALIDATION_SUCCESS 1		/* Module validation success */
#define PM_VALIDATION_FAILURE 2		/* Module validation failure */

/* ***** SNSM ***** */
#define PTIO_SPEED_SET		_ASIC_IOC(SNSM, 0x5)	/* set auto sn mode to arg1 value */
#define PTIO_SPEED_GET		_ASIC_IOC(SNSM, 0x6)	/* get pt spd ala admin.h defines */
#define PTIO_SPEED_CAP_GET	_ASIC_IOC(SNSM, 0x7)	/* get pt spd cap ala admin.h */
#define PTIO_SPEED_LED_GET	_ASIC_IOC(SNSM, 0x8)	/* get pt spd led color */
#define PTIO_SPEED_LED_SET	_ASIC_IOC(SNSM, 0x9)	/* set pt spd led color */

/* ***** LPSM ***** */
#define	PTIO_GET_LOOPBMP	_ASIC_IOC(LPSM, 0x8)
#define PTIO_GET_BLM_MY_ALPA	_ASIC_IOC(LPSM, 0x9)	/* get blm_my_alpa table */
#define PTIO_GET_BLM_PLT_CAM	_ASIC_IOC(LPSM, 0xa)	/* get blm_plt_cam table */
#define PTIO_GET_BLM_PLT_ALPA	_ASIC_IOC(LPSM, 0xb)	/* get blm_plt_alpa table */
#define	PTIO_LOOP_LPB		_ASIC_IOC(LPSM, 0xc)	/* Loop port (or looplet) bypass */
#define	PTIO_FLA_LINIT		_ASIC_IOC(LPSM, 0xd)	/* FLA Loop INITializing */
#define	PTIO_FLA_LPC		_ASIC_IOC(LPSM, 0xe)	/* FLA Loop Port Control */
#define	PTIO_FLA_LSTS		_ASIC_IOC(LPSM, 0xf)	/* FLA Loop STatuS */
#define	PTIO_LPORT_ALPA		_ASIC_IOC(LPSM, 0x10)	/* LPORT ALPA bitmap */
#define PTIO_TX_ONOFF		_ASIC_IOC(LPSM, 0x11)	/* LIP the loop, TX_UNAVAIL on/off */
#define PTIO_LOOP_MRK		_ASIC_IOC(LPSM, 0x12)	/* send MRK primitive signal */
#define	PTIO_FPORT_DEL		_ASIC_IOC(LPSM, 0x13)	/* Undo the FPORT ioctl */
#define PTIO_SET_ALPA_AVAIL	_ASIC_IOC(LPSM, 0x14)	/* Set alpa in blm_alpa_avail[] */
#define PTIO_CLR_ALPA_AVAIL	_ASIC_IOC(LPSM, 0x15)	/* Clr alpa in blm_alpa_avail[] */
/* 
 * defect 210908 - Allow multiple FL port login within one
 * fl_port_stage_interval by checking the number of devices
 * on the current port, when (number of devices on the current port + number of
 * devices login within the current fl_port_stage_interval < fl_port_max_enable)
 */
#define	PTIO_GET_LISABMP			_ASIC_IOC(LPSM, 0x16)


/* ***** LKSM ***** */
/* XXX Revist the category and number */
#define PTIO_LR_CONT		_ASIC_IOC(LKSM, 0x1) /* continue link init/reset */

/* ***** TRKSM ***** */
#define PTIO_SEND_MARK		_ASIC_IOC(TRKSM, 0x2)	/* Send MARK prim onto wire */
#define PTIO_READ_MARK		_ASIC_IOC(TRKSM, 0x3)	/* Get MARK timestamps */
#define PTIO_SET_TPORT		_ASIC_IOC(TRKSM, 0x4)	/* Add port to the trunk */
#define PTIO_GET_MASTERS	_ASIC_IOC(TRKSM, 0x5)	/* Get all trunk masters in Quad */
#define	PTIO_UPDATE_MARK	_ASIC_IOC(TRKSM, 0x6)	/* Update MARK timestamp with RMT */
#define PTIO_TRUNKABLE		_ASIC_IOC(TRKSM, 0x7)	/* Check whether port is trunkable */
#define	PTIO_DO_TRUNK		_ASIC_IOC(TRKSM, 0x8)	/* Enable trunking if possible */
#define PTIO_GET_TINFO		_ASIC_IOC(TRKSM, 0x9)	/* Get trunking group info */
#define PTIO_FIND_MASTER	_ASIC_IOC(TRKSM, 0xa)	/* Find the master port */
#define PTIO_GET_MPORT		_ASIC_IOC(TRKSM, 0xb)	/* Get the master port */
#define PTIO_GET_QPORTS		_ASIC_IOC(TRKSM, 0xc)	/* Get ports within a quad */
#define PTIO_CHECK_MASTER	_ASIC_IOC(TRKSM, 0xd)	/* Check whether port has a master */
#define PTIO_GET_ISLBW		_ASIC_IOC(TRKSM, 0xe)	/* Get the bandwidth of an ISL */
#define PTIO_GET_TMASTER_MSIDX	_ASIC_IOC(TRKSM, 0Xf)	/* trunk master mini-sw idx */

/* ***** FLTSM ***** */
#define PTIO_PROCESS_FLT	_ASIC_IOC(FLTSM, 0x1)	/* filter processing stages */
#define PTIO_PROCESS_FLT_STAGE2 _ASIC_IOC(FLTSM, 0x2)
#define PTIO_SW_FILTERING	_ASIC_IOC(FLTSM, 0x3)	/* s/w frame filtering */
#define	PTIO_ZONE_ENABLE	_ASIC_IOC(FLTSM, 0x4)	/* enable hardware zoning */
#define	PTIO_ZONE_DISABLE	_ASIC_IOC(FLTSM, 0x5)	/* disable hardware zoning */
#define	PTIO_ZONE_ADD		_ASIC_IOC(FLTSM, 0x6)	/* add members to port's zone */
#define	PTIO_ZONE_DEL		_ASIC_IOC(FLTSM, 0x7)	/* del member from port's zone */
#define	PTIO_ZONE_ADD_TYPE	_ASIC_IOC(FLTSM, 0x8)	/* add a zone type (new) */
#define PTIO_ZONE_ADD_GROUP	_ASIC_IOC(FLTSM, 0x9)	/* add zone group (new) */
#define	PTIO_ZONE_ENABLE_PORT	_ASIC_IOC(FLTSM, 0xa)	/* enable all pt zoning (new) */
#define PTIO_ZONE_RESET_PORT	_ASIC_IOC(FLTSM, 0xb)	/* reset all pt zoning (new) */
#define	PTIO_ZONE_DISABLE_PORT	_ASIC_IOC(FLTSM, 0xc)	/* disable all pt zoning (new) */
#define PTIO_ZONE_TOKEN_FREE	_ASIC_IOC(FLTSM, 0xd)	/* free zoning token (new) */
#define PTIO_TRAP_PLOGI_CMD	_ASIC_IOC(FLTSM, 0xe)	/* setup PLOGI cmd trap (new) */
#define PTIO_TRAP_REPORT_LUN_CMD _ASIC_IOC(FLTSM, 0xf)	/* setup report lun trap (new) */
#define PTIO_ZONE_CLEAR_PORT	_ASIC_IOC(FLTSM, 0x10)	/* clear port zoning (new) */
#define PTIO_GET_IDS_LUN	_ASIC_IOC(FLTSM, 0x11)	/* get rcv/orig id (new) */
#define PTIO_APPLY_LUN_INFO	_ASIC_IOC(FLTSM, 0x12)	/* apply lun info (new) */
#define PTIO_ZONE_EXCLUDE	_ASIC_IOC(FLTSM, 0x13)	/* exclude pt from zoning (new) */
#define PTIO_ZONE_SOFT		_ASIC_IOC(FLTSM, 0x14)	/* reset zone for port */
#define PTIO_ZONING_RSCN	_ASIC_IOC(FLTSM, 0x15)	/* propogate rscn info to flt */
#define PTIO_SET_FLT_FEATURES	_ASIC_IOC(FLTSM, 0x16)	/* set filtering features */
#define PTIO_GET_FLT_FEATURES	_ASIC_IOC(FLTSM, 0x17)	/* get filtering features */
#define PTIO_ZONE_RESET_ALL	_ASIC_IOC(FLTSM, 0x18)	/* reset all port zoning (new) */
#define		DIAG_FLAG_RXEN	0x1	/* Enable/ Disable diag RX mode */
/* v4.1 support added */
#define PTIO_ZONE_GET_TYPE	_ASIC_IOC(FLTSM, 0x19)	/* get zone type data */
#define PTIO_ZONE_GET_GROUP	_ASIC_IOC(FLTSM, 0x1a)	/* get zone group data */
#define PTIO_ZONE_CHECK_GROUP	_ASIC_IOC(FLTSM, 0x1b)	/* check zone group data */
/* v4.1 support end */

#define PTIO_DIAG_LOAD_SIDCAM   _ASIC_IOC(FLTSM, 0x1c)   /* load sidcam (diag) */
#define PTIO_DIAG_LOAD_DIDCAM   _ASIC_IOC(FLTSM, 0x1d)   /* load didcam (diag) */
#define PTIO_DIAG_LOAD_LUN_OFFSET _ASIC_IOC(FLTSM, 0x1e) /* load lun offset (diag) */
#define PTIO_DIAG_LOAD_ZONE_GROUP _ASIC_IOC(FLTSM, 0x1f) /* load zone grp ram (diag) */
#define PTIO_DIAG_LOAD_HORIZONTAL _ASIC_IOC(FLTSM, 0x20) /* ld zone horizontally diag) */
#define PTIO_DIAG_LOAD_FILTER   _ASIC_IOC(FLTSM, 0x21)	/* load flt sel (diag) */
#define PTIO_DIAG_LOAD_FIELD    _ASIC_IOC(FLTSM, 0x22)   /* load field def (diag) */
#define PTIO_DIAG_LOAD_ACTION   _ASIC_IOC(FLTSM, 0x23)   /* load action regs (diag) */
#define PTIO_DIAG_GET_COUNTER   _ASIC_IOC(FLTSM, 0x24)   /* get flt stats (diag) */
#define PTIO_DIAG_CLEAR_ALL 	_ASIC_IOC(FLTSM, 0x25)   /* clr all filtering hw (diag) */
#define PTIO_DIAG_FLT_ENABLE    _ASIC_IOC(FLTSM, 0x26)   /* enable frame flt (diag) */
#define PTIO_DIAG_FLT_DISABLE   _ASIC_IOC(FLTSM, 0x27)   /* disable frame flt (diag) */
#define PTIO_REG_FLT_FROZEN 	_ASIC_IOC(FLTSM, 0x28)   /* Reg frozen process hndl */
#define PTIO_DIAG_LOAD_GRPCTL	_ASIC_IOC(FLTSM, 0x29)		/* Load Group control */
#define PTIO_GET_FLT_INFO	_ASIC_IOC(FLTSM, 0x2a)	/* portCamShow support and more */
#define PTIO_PORT_CAM_SHOW	_ASIC_IOC(FLTSM, 0x2b)	/* portCamShow */
#define PTIO_ADD_FLTCNT		_ASIC_IOC(FLTSM, 0x2c)	/* Add a filter counter */
#define PTIO_DEL_FLTCNT		_ASIC_IOC(FLTSM, 0x2d)	/* Delete a filter counter */
#define PTIO_GET_FLTCNT		_ASIC_IOC(FLTSM, 0x2e)	/* number of filter hit count */
#define PTIO_GET_FLTREF		_ASIC_IOC(FLTSM, 0x2f)	/* ADD get perf filter references */
#define PTIO_CLR_ALL_FLT	_ASIC_IOC(FLTSM, 0x30)	/* clear all filter monitors */
#define PTIO_CLR_FLT_STATS	_ASIC_IOC(FLTSM, 0x31)	/* clear all filter status */
#define PTIO_GET_FLTDESC	_ASIC_IOC(FLTSM, 0x32)	/* get perf filter descriptions */
#define PTIO_GET_FLTR_PERF_DATA	_ASIC_IOC(FLTSM, 0x33)	/* Get all Fltr data */

/* v4.3 support added */
#define PTIO_ADD_FLT_FICON_CUP	_ASIC_IOC(FLTSM, 0x34) /* Add a filter for FICON/CUP */
#define PTIO_DEL_FLT_FICON_CUP	_ASIC_IOC(FLTSM, 0x35) /* Delete a filter for FICON/CUP */
/* v4.3 support end */

/* support for Marathon */
#define PTIO_CHECK_ZONE_MATCH	_ASIC_IOC(FLTSM, 0x36) /* Check whether an SID/DID pair are zoned together */

/* support for Quantum */
#define PTIO_ADD_FLT_PORT_MIRROR	_ASIC_IOC(FLTSM, 0x37) /* Add a filter for port mirror */
#define PTIO_DEL_FLT_PORT_MIRROR	_ASIC_IOC(FLTSM, 0x38) /* Delete a filter for port mirror */
#define PTIO_GET_PORT_MIRROR_FLTDESC	_ASIC_IOC(FLTSM, 0x39) /* Get fltr desc for port mirror */
#define PTIO_VERIFY_PORT_MIRROR_FLTDESC	_ASIC_IOC(FLTSM, 0x3a) /* Verify fltr desc for port mirror */
#define PTIO_FLTR_CHECK_PORT_MIRROR_RESOURCE _ASIC_IOC(FLTSM, 0x3b) /* Verify fltr desc port mirror */
#define PTIO_FLT_MIRROR_DISCARD		_ASIC_IOC(FLTSM, 0x3c) /* Add/delete discard fltr */
#define PTIO_FLT_MIRROR_PROCESS_OFFLINE	_ASIC_IOC(FLTSM, 0x3d) /* Process offline port */
#define PTIO_ZONE_DEL_GROUP	_ASIC_IOC(FLTSM, 0x3e)	/* del zone group */

#define PTIO_FLT_FCACC_INGRESS_CFG	_ASIC_IOC(FLTSM, 0x3f) /* FCACC ingress filter config */
#define PTIO_FLT_FCACC_EGRESS_CFG	_ASIC_IOC(FLTSM, 0x40) /* FCACC egress filter config */
#define PTIO_ZONE_FCACC_ADD_FLOW	_ASIC_IOC(FLTSM, 0x41)
#define PTIO_ZONE_FCACC_REMOVE_FLOW	_ASIC_IOC(FLTSM, 0x42)
#define PTIO_ZONE_FCACC_REMOVE_PID	_ASIC_IOC(FLTSM, 0x43)
#define PTIO_ZONE_FCACC_SOFT		_ASIC_IOC(FLTSM, 0x44)
#define PTIO_FLT_FCACC_CLEANUP_CFG  _ASIC_IOC(FLTSM, 0x45) /* FCACC cleanup */
#define PTIO_FLT_FCACC_HA_EGRESS_CFG _ASIC_IOC(FLTSM, 0x46)

#define PTIO_ENABLE_FISOFT_ZONING _ASIC_IOC(FLTSM, 0x47)

/* support for Cassini/Galileo */
#define PTIO_ZONING_RSCN_NEW		_ASIC_IOC(FLTSM, 0x48)
#define PTIO_ZONE_ENABLE_ALL		_ASIC_IOC(FLTSM, 0x49)
#define PTIO_ZONE_DISABLE_ALL		_ASIC_IOC(FLTSM, 0x4a)
#define PTIO_ZONE_SOFT_ALL			_ASIC_IOC(FLTSM, 0x4b)

#define PTIO_FLT_ADD_REDIRECT_FILTER 	_ASIC_IOC(FLTSM, 0x4c) /* add a redirect filter */
#define PTIO_FLT_DEL_REDIRECT_FILTER 	_ASIC_IOC(FLTSM, 0x4d) /* remove a redirect filter */
#define PTIO_ZONE_QOS_SET_FLOW		_ASIC_IOC(FLTSM, 0x4e) /* set qos flow for AG */
#define PTIO_AG_REMOVE_PLOGI_TRAP_CMD	_ASIC_IOC(FLTSM, 0x4f) /* remove PLOGI trap for AG */
#define PTIO_ADD_FLT_PORT_MIRROR_ROUTE	_ASIC_IOC(FLTSM, 0x50) /* Add a route for port mirror */
#define PTIO_DEL_FLT_PORT_MIRROR_ROUTE	_ASIC_IOC(FLTSM, 0x51) /* Delete a route for port mirror */
#define PTIO_FCOE_IFID_LIST		_ASIC_IOC(FLTSM, 0x52) /* List of FCoE FE port IF_id */
/* v4.3 support end */
/* ***** ASIC_RTE ***** */
#define	PTIO_UNI_ADD		_ASIC_IOC(ASIC_RTE, 0x1)	/* add unicast route */
#define	PTIO_UNI_DEL		_ASIC_IOC(ASIC_RTE, 0x2)	/* delete unicast route */
#define	PTIO_MULTI_ADD		_ASIC_IOC(ASIC_RTE, 0x3)	/* add multicast route */
#define	PTIO_MULTI_DEL		_ASIC_IOC(ASIC_RTE, 0x4)	/* delete multicast route */
#define	PTIO_UNI_DONE		_ASIC_IOC(ASIC_RTE, 0x5)	/* unicast routing table done */
#define PTIO_AREART_ADD		_ASIC_IOC(ASIC_RTE, 0x6)	/* add area route */
#define PTIO_AREART_DEL		_ASIC_IOC(ASIC_RTE, 0x7)	/* del area route */
#define PTIO_GET_DOMAIN_PORT	_ASIC_IOC(ASIC_RTE, 0x8)	/* get domain dest port (diag) */
#define PTIO_GET_PORT_RT_TABLE	_ASIC_IOC(ASIC_RTE, 0X9)	/* get rt table from sw */

/* These are new ioctl used for condor */
#define	PTIO_RTE_ADD_NEXTHOP	_ASIC_IOC(ASIC_RTE, 0xa)	/* add next hop */
#define	PTIO_RTE_ADD_ROUTE_LIST	_ASIC_IOC(ASIC_RTE, 0xb)	/* add multiple routes */
#define	PTIO_RTE_ADD_PATH	_ASIC_IOC(ASIC_RTE, 0xc)	/* add path */
#define	PTIO_RTE_DEL_PATH	_ASIC_IOC(ASIC_RTE, 0xd)	/* delete path */
#define	PTIO_RTE_ADD_ROUTE	_ASIC_IOC(ASIC_RTE, 0xe)	/* add route */
#define	PTIO_RTE_DEL_ROUTE	_ASIC_IOC(ASIC_RTE, 0xf)	/* delete route */
#define	PTIO_RTE_CLEAR_ROUTE	_ASIC_IOC(ASIC_RTE, 0x10)	/* clear route */
#define	PTIO_RTE_CONFIG_VIF	_ASIC_IOC(ASIC_RTE, 0x11)	/* config virtual link */
#define	PTIO_RTE_CONFIG_POLICY	_ASIC_IOC(ASIC_RTE, 0x12)	/* config. route policy */
#define	PTIO_RTE_CONFIG_VCMODE	_ASIC_IOC(ASIC_RTE, 0x13)	/* config vc mode */
#define	PTIO_RTE_CONFIG_VC	_ASIC_IOC(ASIC_RTE, 0x14)	/* config vc setting */
#define	PTIO_RTE_UPDATE_CONFIG	_ASIC_IOC(ASIC_RTE, 0x15)	/* config chip tables */
#define	PTIO_RTE_UPDATE_PORT_CONFIG _ASIC_IOC(ASIC_RTE, 0x16)	/* config port tables */
#define	PTIO_RTE_VERIFY_ADD_ROUTES _ASIC_IOC(ASIC_RTE, 0x17)	/* verify add route */
#define	PTIO_RTE_VERIFY_DEL_ROUTES _ASIC_IOC(ASIC_RTE, 0x18)	/* verify delete routes */
#define	PTIO_RTE_CONFIG_IF _ASIC_IOC(ASIC_RTE, 0x19)	/* configure if */
#define	PTIO_RTE_VERIFY_NEXTHOP _ASIC_IOC(ASIC_RTE, 0x1a)	/* verify nexthop */
#define	PTIO_RTE_VERIFY_CONFIG _ASIC_IOC(ASIC_RTE, 0x1b)	/* verify routing config */
#define	PTIO_RTE_ADD_LPBK_ROUTE _ASIC_IOC(ASIC_RTE, 0x1c)	/* add loopback area rt */
#define PTIO_RTE_SET_STAG _ASIC_IOC(ASIC_RTE, 0x1d)			/* configure STAG */
#define PTIO_RTE_PAUSE _ASIC_IOC(ASIC_RTE, 0x1e)			/* Pause traffic    */
#define PTIO_RTE_RESUME _ASIC_IOC(ASIC_RTE, 0x1f)			/* Resume traffic  */
	
#define	PTIO_RTE_DEL_NEXTHOP	_ASIC_IOC(ASIC_RTE, 0x20)	/* del next hop */
/* New IOCTL's for Condor, used exclusively by CER. */

#define PTIO_CER_ADD_FRAGMENT _ASIC_IOC(ASIC_RTE, 0x21)	/* add portion of route */
#define PTIO_CER_DEL_FRAGMENT _ASIC_IOC(ASIC_RTE, 0x22)	/* remove portion of route */
/* read routing table, assuming port-based routing */
#define PTIO_ROUTE_GET_PORT_TABLE _ASIC_IOC(ASIC_RTE, 0x23)
#define PTIO_SET_PID2VC		_ASIC_IOC(ASIC_RTE, 0x24)	/* set blm_port2vc value */

/* New RTE IOCTLs for FC Fast write */
/* FCIP ASIC driver to setup the egid to vc and reverse map and Condor drv */
#define PTIO_FCACC_CFGSETUP    _ASIC_IOC(ASIC_RTE, 0x25)
#define PTIO_FCACC_CFGCLEANUP  _ASIC_IOC(ASIC_RTE, 0x26)
#define PTIO_RTE_ADD_EGIDS  	_ASIC_IOC(ASIC_RTE, 0x27)
#define PTIO_RTE_DEL_EGIDS		_ASIC_IOC(ASIC_RTE, 0x28)
/* Spike/Striker support */
#define PTIO_RTE_CONFIG_TUNNEL_QOS_VC	_ASIC_IOC(ASIC_RTE, 0x29)	/*configure multiple vcs for tunnel qos*/
#define PTIO_RTE_CONFIG_SPRAY			_ASIC_IOC(ASIC_RTE, 0x2a)	/*configure spray table*/
/* DCE platform core-edge init by blade driver */
#define PTIO_DCE_CORE_EDGE_SETUP		_ASIC_IOC(ASIC_RTE, 0x2b)	/* dce blade initiaite asic core-edge*/


/* ***** Pinball FPGA ioctls ***** */

/*  FCR related ioctls */

#define PTIO_FCR_SET_EX_EGID		_ASIC_IOC(ASIC_FCR, 0x0)
#define PTIO_FCR_CLR_EX_EGID		_ASIC_IOC(ASIC_FCR, 0x1)
#define PTIO_FCR_SET_SFID			_ASIC_IOC(ASIC_FCR, 0x2)
#define PTIO_FCR_CLR_SFID			_ASIC_IOC(ASIC_FCR, 0x3)
#define PTIO_FCR_CREATE_BB_XD		_ASIC_IOC(ASIC_FCR, 0x4)
#define PTIO_FCR_ADD_EDGE_DOM		_ASIC_IOC(ASIC_FCR, 0x5)
#define PTIO_FCR_DEL_EDGE_DOM		_ASIC_IOC(ASIC_FCR, 0x6)
#define PTIO_FCR_PURGE_EDGE_DOM		_ASIC_IOC(ASIC_FCR, 0x7)
#define PTIO_FCR_ADD_FAB_ROUTE		_ASIC_IOC(ASIC_FCR, 0x8)
#define PTIO_FCR_DEL_FAB_ROUTE		_ASIC_IOC(ASIC_FCR, 0x9)
#define PTIO_FCR_PURGE_FAB_ROUTE	_ASIC_IOC(ASIC_FCR, 0xa)
#define PTIO_FCR_ADD_PROXY			_ASIC_IOC(ASIC_FCR, 0xb)
#define PTIO_FCR_DEL_PROXY			_ASIC_IOC(ASIC_FCR, 0xc)
#define PTIO_FCR_PURGE_PROXY		_ASIC_IOC(ASIC_FCR, 0xd)
#define PTIO_FCR_CFG_ELS_TRAP		_ASIC_IOC(ASIC_FCR, 0xe)
#define PTIO_FCR_CONFIG_FCR_MODE	_ASIC_IOC(ASIC_FCR, 0xf)
#define PTIO_FCR_CONFIG_ROUTE_POLICY _ASIC_IOC(ASIC_FCR, 0x10)
#define PTIO_FCR_SET_BB_FAB_ID 		_ASIC_IOC(ASIC_FCR, 0x11)
#define PTIO_FCR_GET_DFAB_EGIDS		_ASIC_IOC(ASIC_FCR, 0x12)
#define PTIO_FCR_GET_BB_FAB_ID 		_ASIC_IOC(ASIC_FCR, 0x13)
#define PTIO_FCR_GET_FABRIC_INFO	_ASIC_IOC(ASIC_FCR, 0x14)
#define PTIO_FCR_GET_FABRT_INFO		_ASIC_IOC(ASIC_FCR, 0x15)
#define PTIO_FCR_GET_PROXY_INFO		_ASIC_IOC(ASIC_FCR, 0x16)
#define PTIO_FCR_SET_LOCAL_EGID		_ASIC_IOC(ASIC_FCR, 0x17)
#define PTIO_FCR_LE_XD_REACHABLE	_ASIC_IOC(ASIC_FCR, 0x18)
#define PTIO_FCR_LE_XD_UNREACHABLE	_ASIC_IOC(ASIC_FCR, 0x19)
#define PTIO_FCR_ADD_EDGE_ROUTE		_ASIC_IOC(ASIC_FCR, 0x1a)
#define PTIO_FCR_DEL_EDGE_ROUTE		_ASIC_IOC(ASIC_FCR, 0x1b)
/*PTIO redefines for striker/spike vex intercept */
#define PTIO_FCR_SET_SFID_INT		_ASIC_IOC(ASIC_FCR, 0x1c)
#define PTIO_FCR_CLR_SFID_INT		_ASIC_IOC(ASIC_FCR, 0x1d)
#define PTIO_FCR_ADD_PROXY_INT		_ASIC_IOC(ASIC_FCR, 0x1e)
#define PTIO_FCR_DEL_PROXY_INT		_ASIC_IOC(ASIC_FCR, 0x1f)
#define PTIO_FCR_PURGE_PROXY_INT	_ASIC_IOC(ASIC_FCR, 0x20)
#define PTIO_FCR_LE_XD_REACHABLE_INT	_ASIC_IOC(ASIC_FCR, 0x21)
#define PTIO_FCR_LE_XD_UNREACHABLE_INT	_ASIC_IOC(ASIC_FCR, 0x22)

/* VF related ioctls */
#define PTIO_VF_SET_SFID			_ASIC_IOC(ASIC_VF, 0x0)
#define PTIO_VF_FAB_DOM				_ASIC_IOC(ASIC_VF, 0x1)
#define PTIO_VF_ADD_EGRESS_DST		_ASIC_IOC(ASIC_VF, 0x2)
#define PTIO_VF_DEL_EGRESS_DST		_ASIC_IOC(ASIC_VF, 0x3)

/* ***** BUF ***** */
#define	PTIO_GET_BUF		_ASIC_IOC(BUF, 0x1)	/* get buffer and port */
#define	PTIO_SET_BUF		_ASIC_IOC(BUF, 0x2)	/* set avail buf intr */
#define	PTIO_RET_BUF		_ASIC_IOC(BUF, 0x3)	/* return buffer */
#define	PTIO_RET_TX_BUF		_ASIC_IOC(BUF, 0x4)	/* return Tx buffer */
#define	PTIO_GET_ABBC		_ASIC_IOC(BUF, 0x5)	/* get available BB_Credit */
#define	PTIO_GET_BBC		_ASIC_IOC(BUF, 0x6)	/* get bb credit for the Fx_PORT */
#define	PTIO_DIAG_RET_BUF	_ASIC_IOC(BUF, 0x7)	/* diag return buffer */
#define PTIO_GET_LX_DISTANCE	_ASIC_IOC(BUF, 0x8)
#define PTIO_ENABLE_MARK_RETX	_ASIC_IOC(BUF, 0x9)	/* Enable MARK retransmission */
#define PTIO_DISABLE_MARK_RETX	_ASIC_IOC(BUF, 0xa)	/* Disable MARK retransmission */
	/* Send MARK primitive with LRTT (link round trip timer) enabled */
#define PTIO_SEND_MARK_LRTT	_ASIC_IOC(BUF, 0xb)
	/* Save link round trip timer from ASIC */
#define PTIO_SAVE_LINK_RT_TIMER	_ASIC_IOC(BUF, 0xc)
	/* Save the credits from link round trip timer from other switch */
#define PTIO_SAVE_LINK_RT_CREDITS	_ASIC_IOC(BUF, 0xd)
/* Check if port config should be allowed */
#define PTIO_CHK_PORT_CFG	_ASIC_IOC(BUF, 0xe)
/* get port buffer allocations */
#define PTIO_GET_BUFFER_ALLOC	_ASIC_IOC(BUF, 0xf)
/* get long distance calculations */
#define PTIO_APPLY_RLIMIT	_ASIC_IOC(BUF, 0x10) /* apply rate limit */
#define PTIO_EARLY_BUF_RET	_ASIC_IOC(BUF, 0x11) /* return an RRDY before the poll */
#define PTIO_SET_EDGE_HOLD_TIME	_ASIC_IOC(BUF, 0x12) /* Set edge_hold_time */

/* ***** INF ***** */
#define	PTIO_GET_FXERRSTS	_ASIC_IOC(INF, 0x1)
#define	PTIO_GET_FLERRSTS	_ASIC_IOC(INF, 0x2)
#define	PTIO_PORT_ADMIN		_ASIC_IOC(INF, 0x3)	/* port admin stuff */
#define	PTIO_CLR_STATS		_ASIC_IOC(INF, 0x4)	/* clr hw stat SRAM */
#define	PTIO_GET_GSTATS		_ASIC_IOC(INF, 0x5)	/* get common hw stats */
#define	PTIO_GET_LSTATS		_ASIC_IOC(INF, 0x6)	/* get loop hw stats */
#define	PTIO_GET_FRMSTS		_ASIC_IOC(INF, 0x7)	/* get hw frame stats */
#define	PTIO_GET_ERRSTS		_ASIC_IOC(INF, 0x8)	/* get hw error stats */
#define	PTIO_GET_INTSTS		_ASIC_IOC(INF, 0x9)	/* get intr stats */
#define	PTIO_GET_TXRXWORDS	_ASIC_IOC(INF, 0xa)	/* pt perf calculation */
#define	PTIO_GET_TXRXFRMS	_ASIC_IOC(INF, 0xb)	/* port perf calculation */

#define PTIO_ADD_EECNT		_ASIC_IOC(INF, 0xc)	/* Add a SID-DID pair */
#define PTIO_DEL_EECNT		_ASIC_IOC(INF, 0xd)	/* Del a SID-DID pair */
#define PTIO_GET_EEREF		_ASIC_IOC(INF, 0xe)	/* Get list of EE keys */
#define PTIO_GET_EEMSK		_ASIC_IOC(INF, 0xf)	/* Get current EE mask */
#define PTIO_SET_EEMSK		_ASIC_IOC(INF, 0x10)	/* Set the SID-DID pair */
#define PTIO_CLR_ALPACNT	_ASIC_IOC(INF, 0x11)	/* Clr CRC cnt for ALPA */
#define PTIO_GET_ALPACNT	_ASIC_IOC(INF, 0x12)	/* Get CRC cnt for ALPA */
#define PTIO_GET_EESNT_CNT	_ASIC_IOC(INF, 0x13)	/* Snt wd cnt SID-DID */
#define PTIO_GET_EERCV_CNT	_ASIC_IOC(INF, 0x14)	/* Rcv wd cnt SID-DID */
#define PTIO_GET_EECRC_CNT	_ASIC_IOC(INF, 0x15)	/* CRC err cnt SID-DID */

/* v4.1 support added for Zoning/PS */
#define PTIO_CLR_ALL_EE		_ASIC_IOC(INF, 0x16)	/* clear all EE monitors */
#define PTIO_CLR_EE_STATS	_ASIC_IOC(INF, 0x17)	/* clear all EE status */
#define	PTIO_GET_BBZEROC 	_ASIC_IOC(INF, 0x18)   /* get ticks when BB credit == 0 */
#define	PTIO_GET_STATSET	_ASIC_IOC(INF, 0x19)	/* get STATSET */
#define	PTIO_CLR_GSTATS		_ASIC_IOC(INF, 0x1a)	/* clr trunk group stat SRAM */

/* v4.2 support added */
#define PTIO_REPLACE_ALL_EE	_ASIC_IOC(INF, 0x20)	/* Replace all SID-DID pairs */
#define PTIO_CLR_MULTI_ALPACNT	_ASIC_IOC(INF, 0x21)	/* Clear CRC cntrs for multi ALPA's */
#define PTIO_GET_MULTI_ALPACNT	_ASIC_IOC(INF, 0x22)	/* Get CRC cntrs for multiple ALPA's */
#define	PTIO_CLR_TXRXWORDS	_ASIC_IOC(INF, 0x23)	/* Clear the TX and/or Rx word counts */
/* v4.2 support end */

#define PTIO_GET_EE_PERF_DATA	_ASIC_IOC(INF, 0x24)	/* Get all EE data */
#define PTIO_PERF_POLL		_ASIC_IOC(INF, 0x25)	/* Trigger the Perf Poll */
#define PTIO_GET_CHIP_STAT	_ASIC_IOC(INF, 0x26)	/* get chip stat */
#define PTIO_CLR_CHIP_STAT _ASIC_IOC(INF, 0x27) /* clear chip stats */
#define PTIO_IS_ANY_VPORTS_ONLINE	_ASIC_IOC(INF, 0x28) /* is any virtual ports online */

/* FC Acceleration information, might need to relocate */
#define PTIO_FCACC_ALLOWED	_ASIC_IOC(INF, 0x29) /* FC Acceleration allowed */
#define PTIO_FCACC_GE_ASSOC_PORTS _ASIC_IOC(INF, 0x2a) /* FC Accel ports */
#define PTIO_FCACC_GE_SET_CAP_BM _ASIC_IOC(INF, 0x2b) /* Switch port cap */
#define PTIO_1250_FCACC_ADD_FLOW    _ASIC_IOC(INF, 0x2c)
#define PTIO_1250_FCACC_REMOVE_FLOW _ASIC_IOC(INF, 0x2d)
#define PTIO_1250_FCACC_REMOVE_PID  _ASIC_IOC(INF, 0x2e)
#define PTIO_1250_FCACC_PORT_OFFLINE  _ASIC_IOC(INF, 0x2f)


/* Generic Performance Monitor IOCTLS		*/
#define PTIO_ADD_PMON			_ASIC_IOC(INF, 0x30) /* add sid-did PM monitor		*/
#define PTIO_DEL_PMON			_ASIC_IOC(INF, 0x31) /* delete sid-did PM monitor	*/
#define PTIO_START_PMON			_ASIC_IOC(INF, 0x32) /* start PM monitor polling	*/
#define PTIO_STOP_PMON			_ASIC_IOC(INF, 0x33) /* stop PM monitor polling		*/
#define PTIO_GET_PMSTATS		_ASIC_IOC(INF, 0x34) /* get sid-did PM statistics	*/
#define PTIO_CLR_PMSTATS		_ASIC_IOC(INF, 0x35) /* clear sid-did PM statistics */
#define PTIO_GET_ALL_PMSTATS	_ASIC_IOC(INF, 0x36) /* get all PM statistics		*/
#define PTIO_CLR_ALL_PMSTATS	_ASIC_IOC(INF, 0x37) /* clear all PM statistics		*/
#define PTIO_GET_PMREFS			_ASIC_IOC(INF, 0x38) /* get PM indeces/references	*/
#define PTIO_GET_PM_LTSIZE		_ASIC_IOC(INF, 0x39) /* get link table size			*/
#define PTIO_ALLOC_PM_LTENTRY	_ASIC_IOC(INF, 0x3a) /* allocate link table entry	*/
#define PTIO_FREE_PM_LTENTRY	_ASIC_IOC(INF, 0x3b) /* free link table entry		*/
#define PTIO_REPLACE_PMON		_ASIC_IOC(INF, 0x3c) /* replace sid-did PM monitors	*/
#define PTIO_READ_REGISTER		_ASIC_IOC(INF, 0x3d) /* Read an ASIC register*/

#define PTIO_GET_FIXED_MEDIA_DATA	_ASIC_IOC(INF, 0x3e) /* Read broadcom GBIC Data*/

/* ***** TXRX ***** */
#define PTIO_CLEANUP_UPSTREAM_DMA _ASIC_IOC(TXRX, 0x1) /* cleanup dma */
#define PTIO_SEND_MSG			  _ASIC_IOC(TXRX, 0x2) /* send msg */
#define PTIO_CONTROL_FRAME_RECV   _ASIC_IOC(TXRX, 0x3) /* control frame reception */

/* ***** ASIC_INIT ***** */
#define	PTIO_ENABLE			_ASIC_IOC(ASIC_INIT, 0x1)	/* bring port online */
#define PTIO_ASIC_RESET		_ASIC_IOC(ASIC_INIT, 0x2) 	/* Reset ASIC happened */
#define PTIO_CHIP_INIT		_ASIC_IOC(ASIC_INIT, 0x3)	/* Initialize the chip */
#define	PTIO_MARK_INIT		_ASIC_IOC(ASIC_INIT, 0x4 | ASIC_SW_ACCESS)   /* Mark chip and ports initialized */
#define PTIO_PORT_INT_FAULT	_ASIC_IOC(ASIC_INIT, 0x5)	/* fault port due to excessive int. */
#define PTIO_CHIP_HT_INIT	_ASIC_IOC(ASIC_INIT, 0x6)	/* init hold time registers for every chip in the group */
#define PTIO_XCP_CONNECT	_ASIC_IOC(ASIC_INIT, 0x7)	/* initialize XCP conne ction */
#define PTIO_XCP_DISCONNECT	_ASIC_IOC(ASIC_INIT, 0x8)	/* tear down XCP conne ction */
#define PTIO_INIT_PCI_CFG	_ASIC_IOC(ASIC_INIT, 0x9)	/* initialize PCI config params for chip */
#define PTIO_FWDL_ISCSI		_ASIC_IOC(ASIC_INIT, 0xa)	/* fwdl 2 iscsi chip */
#define PTIO_CHIP_WSTART	_ASIC_IOC(ASIC_INIT, 0xb)	/* BP warm start */
#define PTIO_EMBD_INT_REENABLE 		_ASIC_IOC(ASIC_INIT, 0xc)	/* Poll in case of excessive chip interrupts */
#define PTIO_ASIC_POLL_START		_ASIC_IOC(ASIC_INIT, 0xd)	/* Start the polling between cpsas and 1250 */

/* **** KAP Driver Specific ASIC_INITs ***** */
#define	PTIO_KAP_INFO				_ASIC_IOC(ASIC_INIT, 0xe)

/* ***** ASIC_INTR ***** */
#define	PTIO_PORT_INT_GET	_ASIC_IOC(ASIC_INTR, 0x1)	/* No longer used */
#define	PTIO_PORT_INT_ENABLE	_ASIC_IOC(ASIC_INTR, 0x2)	/* enable port intr */
#define	  PORT_CHIP_INT		0x0	/* enable chip level port intr */
#define	  PORT_ABUF_INT		0x20	/* enable free buffer intr */
#define	PTIO_PORT_INT_DISABLE	_ASIC_IOC(ASIC_INTR, 0x3)	/* disable pt intr */
#define	PTIO_PORT_INT_PEND	_ASIC_IOC(ASIC_INTR, 0x4)	/* check if pt intr pending */
#define	PTIO_CHIP_INT_ENABLE	_ASIC_IOC(ASIC_INTR, 0x5)	/* enable chip intr */
#define	PTIO_CHIP_INT_DISABLE	_ASIC_IOC(ASIC_INTR, 0x6)	/* disable chip intr */
#define	PTIO_CHIP_INT_PEND	_ASIC_IOC(ASIC_INTR, 0x7)	/* check if chip intr pending */
#define	PTIO_CHIP_HALT	_ASIC_IOC(ASIC_INTR, 0x8)	/* halt chip called by blade fault */


/* ***** ASIC_HA ***** */
#define PTIO_SET_ASIC_ONLINE	_ASIC_IOC(ASIC_HA, 0x1)	/* clr ASIC offline bit */
#define PTIO_IS_BLADE_READY		_ASIC_IOC(ASIC_HA, 0x2)	/* chk ASIC blade rdy bit */
#define PTIO_SET_BLADE_READY	_ASIC_IOC(ASIC_HA, 0x3)	/* set ASIC blade rdy bit */
#define PTIO_CLR_BLADE_READY	_ASIC_IOC(ASIC_HA, 0x4)	/* clr ASIC blade rdy bit */
#define PTIO_IS_SPAD_SET		_ASIC_IOC(ASIC_HA, 0x5)	/* check ASIC PIC-ready bit */
#define PTIO_SET_SPAD			_ASIC_IOC(ASIC_HA, 0x6)	/* set ASIC PIC-ready bit */
#define PTIO_CLR_SPAD			_ASIC_IOC(ASIC_HA, 0x7)	/* clear ASIC PIC-ready bit */

#define PTIO_IS_PIC_READY		PTIO_IS_SPAD_SET
#define PTIO_SET_PIC_READY		PTIO_SET_SPAD
#define PTIO_CLR_PIC_READY		PTIO_CLR_SPAD

/* *****ASIC_ERR ***** */

/* ***** ASIC_MISC ***** */
	/* hot plug */
#define PTIO_SET_STATUS		_ASIC_IOC(ASIC_MISC, 0x1 | ASIC_SW_ACCESS)	/* set the bloom status */
	/* "direct" register access */
#define PTIO_GBIC_CMD		_ASIC_IOC(ASIC_MISC, 0x2)   /* Execute I2C GBIC cmd */
#define	PTIO_DISABLE_SID_DID_CHK _ASIC_IOC(ASIC_MISC, 0x3)	/* disable SID/DID chk */
#define PTIO_ASIC_ACCESSIBLE	_ASIC_IOC(ASIC_MISC, 0x4)   /* Verify ASIC can be accessed */
	/* debug/test */
#define PTIO_PROC_ENTRY		_ASIC_IOC(ASIC_MISC, 0x5 | ASIC_SW_ACCESS)	/* modify  proc entries for asic */
	/* Others */
#define PTIO_GET_REG_MAP	_ASIC_IOC(ASIC_MISC, 0x6)
#define PTIO_GET_CHIP_REG	_ASIC_IOC(ASIC_MISC, 0x7)
#define	PTIO_DOMAIN			_ASIC_IOC(ASIC_MISC, 0x8)	/* domain name is known */
#define	PTIO_MULTI_DONE		_ASIC_IOC(ASIC_MISC, 0x9)	/* mcast routing table done */
#define PTIO_SET_PROPERTY	_ASIC_IOC(ASIC_MISC, 0xa | ASIC_SW_ACCESS)	/* update property of the port */
#define PTIO_ADD_ASICMSG	_ASIC_IOC(ASIC_MISC, 0xb)	/* add the ASIC driver msg to appropriate queue */
#define PTIO_SET_AREA		_ASIC_IOC(ASIC_MISC, 0xc)	/* set area ASIC PORT ID ADDR Reg */
#define PTIO_LOG_LLI		_ASIC_IOC(ASIC_MISC, 0xd)	/* log LLI and swc data */
#define PTIO_SET_AGMODE     _ASIC_IOC(ASIC_MISC, 0xe | ASIC_SW_ACCESS) /* agmode */

/*
 * FC acceleration PTIO to set condor to fc fastwrite mode:
 * used for non-shared area routing. 
*/
#define PTIO_SET_FCACC_CONDOR_CFG  _ASIC_IOC(ASIC_MISC, 0xf)
#define PTIO_PROC_LINK_ENTRY _ASIC_IOC(ASIC_MISC, 0x10)	/* Additional proc entry link */
#define PTIO_SET_AG_ROUTE_LOGICAL_ONLINE    _ASIC_IOC(ASIC_MISC, 0x11) /* For AG trunked ports, set route to logically online */
#define PTIO_PROC_OPCODE	_ASIC_IOC(ASIC_MISC, 0x12)	/* Process based on opcode */
#define PTIO_SET_SERDES_CFG _ASIC_IOC(ASIC_MISC, 0x13)  /* Set SerDes configuration */
#define PTIO_SET_BP_CFG		_ASIC_IOC(ASIC_MISC, 0x14)	/* Pass the BP config */
#define PTIO_CHIP_RSRC_SET	_ASIC_IOC(ASIC_MISC, 0x15)	/* Chip Shared Memory attach */

/* DCE suuport */
#define	PTIO_VLAN_CFG			_ASIC_IOC(ASIC_DCE, 0x1)	/* VLAN based configs */
#define PTIO_SET_VID_EVID_REMAP	_ASIC_IOC(ASIC_DCE, 0x2)	/* set vid evid mapping */
#define PTIO_GET_VID_EVID_REMAP	_ASIC_IOC(ASIC_DCE, 0x3)	/* get vid evid mapping */
#define PTIO_SET_PRI_VLAN_IDX	_ASIC_IOC(ASIC_DCE, 0x4)	/* set private vlan index */
#define PTIO_GET_PRI_VLAN_IDX	_ASIC_IOC(ASIC_DCE, 0x5)	/* get private vlan  */
#define PTIO_SET_REMAP_VLAN		_ASIC_IOC(ASIC_DCE, 0x6)	/* set remap vlan id */
#define PTIO_GET_REMAP_VLAN		_ASIC_IOC(ASIC_DCE, 0x7)	/* get remap vlan id */

#ifdef CONFIG_FLRN
#define PTIO_CFG_FAKE_LEARN     _ASIC_IOC(ASIC_DCE, 0x8)    /* Enable fake learning on port */
#endif

/* ***** ASIC_BLOOM specific ***** */
	/* FDET related */
#define	PTIO_GET_FDET_STATS	_ASIC_IOC(ASIC_BLM, 0x1)	/* get fail detect lgc stat */
					/* see fdet_stats_t struct */
#define	PTIO_SET_FDET_CTL	_ASIC_IOC(ASIC_BLM, 0x2)	/* set fail detect ctrl bit */
#define	PTIO_UNSET_FDET_CTL	_ASIC_IOC(ASIC_BLM, 0x3)	/* clr fail detect ctrl bit */
#define	PTIO_SET_R2TQ_PCTL	_ASIC_IOC(ASIC_BLM, 0x4)	/* set Rx-to-Tx parity ctrl */
#define	PTIO_GET_R2TQ_PSTAT	_ASIC_IOC(ASIC_BLM, 0x5)	/* get Rx-to-Tx parity err status */
#define	PTIO_ENABLE_FDET_INT	_ASIC_IOC(ASIC_BLM, 0x6)	/* enable fail detect intr */
#define	PTIO_DISABLE_FDET_INT	_ASIC_IOC(ASIC_BLM, 0x7)	/* disable fail detection interrupt */
#define	PTIO_GET_FDET_INT	_ASIC_IOC(ASIC_BLM, 0x8)	/* check for fail detection interrupt */
#define PTIO_BZIO_POLL		_ASIC_IOC(ASIC_BLM, 0x9)	/* doing BZIO polling */
	/* No longer used */
#define	PTIO_GET_DEV_INFO	_ASIC_IOC(ASIC_BLM, 0xa)
#define	PTIO_LOOPDIAG_LIP	_ASIC_IOC(ASIC_BLM, 0xb)	/* set up pt loop diag mode */
#define	PTIO_LOOPDIAG_LPB	_ASIC_IOC(ASIC_BLM, 0xc)	/* loop port bypass the ALPD */
#define	PTIO_LOOPDIAG_LPE	_ASIC_IOC(ASIC_BLM, 0xd)	/* loop port enable the ALPD */
#define	PTIO_CMEM_DIAG		_ASIC_IOC(ASIC_BLM, 0xe)	/* diag message output */
#define	PTIO_LOOP_DIAG		_ASIC_IOC(ASIC_BLM, 0xf)	/* Chk if FL_Port diag hooked-up */
#define	PTIO_FLBS_DIAG		_ASIC_IOC(ASIC_BLM, 0x10)	/* chk if FL_Port a lb slave */
#define	PTIO_ALPA_DIAG		_ASIC_IOC(ASIC_BLM, 0x11)	/* set AL_PA for FL lb test */
#define	PTIO_TXDS_DIAG		_ASIC_IOC(ASIC_BLM, 0x12)	/* Disable FC-AL tx front-end */
#define	PTIO_TXEN_DIAG		_ASIC_IOC(ASIC_BLM, 0x13)	/* Enable FC-AL tx front-end */
#define	PTIO_SET_FCTL_MODE	_ASIC_IOC(ASIC_BLM, 0x14)
#define PTIO_PORT_DBG_SHOW	_ASIC_IOC(ASIC_BLM, 0x15)	/* ptXxxShow */
/* Phantom/QL/FA */
#define	PTIO_PHAN_LOGO		_ASIC_IOC(ASIC_BLM, 0x16)	/* rm all phantom nodes pt */
#define	PTIO_PHAN_REG		_ASIC_IOC(ASIC_BLM, 0x17)	/* add phantom dev (loop only) */
#define	PTIO_PHAN_XLATE		_ASIC_IOC(ASIC_BLM, 0x18)	/* xlate phantom sid and did */
#define	PTIO_PHAN_PADD		_ASIC_IOC(ASIC_BLM, 0x19)	/* add phantom node for remote did */
#define	PTIO_PHAN_TEST		_ASIC_IOC(ASIC_BLM, 0x1a)	/* test phantom for (S_ID, D_ID) */
#define	PTIO_PHAN_ADD		_ASIC_IOC(ASIC_BLM, 0x1b)	/* add a phantom device (loop only) */
#define	PTIO_PHAN_DEL		_ASIC_IOC(ASIC_BLM, 0x1c)	/* remove a phantom device */
#define	PTIO_PHAN_ALPA		_ASIC_IOC(ASIC_BLM, 0x1d)	/* get phantom AL_PA by address ID */
#define	PTIO_PHAN_DEVID		_ASIC_IOC(ASIC_BLM, 0x1e)	/* get address ID by phantom AL_PA */
#define	PTIO_LOOP_LIP		_ASIC_IOC(ASIC_BLM, 0x1f)	/* Looplet init (send LIPs) */
#define	PTIO_LOOP_INIT		_ASIC_IOC(ASIC_BLM, 0x20)	/* Looplet init sequence */
#define	PTIO_LOOP_BMP		_ASIC_IOC(ASIC_BLM, 0x21)	/* Looplet init AL_PA bitmaps */
#define	PTIO_LOOP_RTR		_ASIC_IOC(ASIC_BLM, 0x22)	/* Looplet unicast routes */
#define	PTIO_GET_DEVCNT		_ASIC_IOC(ASIC_BLM, 0x23)	/* get pub/pri/phantom cnts */
/* Fport disable port tx before ACC to avoid leaking frames */
#define PTIO_PORT_DISABLE_TX	_ASIC_IOC(ASIC_BLM, 0x24)
/* swc_test debug */
#define	PTIO_GET_DEBUG_DATA	_ASIC_IOC(ASIC_DEBUG, 0x800	 | ASIC_SW_ACCESS) /* get debug data */
#define PTIO_DO_UNIT_TEST	_ASIC_IOC(ASIC_DEBUG, 0x801)	/* asic swc unit tests, not used yet */
#define	PTIO_TEST_RECOV		_ASIC_IOC(ASIC_DEBUG, 0x900)	/* test recovery */

/*
 * Marathon FCIP ioctls.
 * =====================
 *
 * Marathon. Command/Response used from ipsd to FCIP driver. Used to provide a
 * blocking request/response semantics to ipsd daemon.
 *
 *
 * APP_CMD will be packaged as an FC frame and sent down to the FC driver.
 * 	FC header info:
 * 		s_id/d_id 	= 0xFFFFFD
 * 		Type 		= TYPE_IPSD_FKIO
 *
 * FC driver will fill in the ox_id and other info in the FC header.
 *
 * FCIP driver will intercept this VU type frame and package it as a ipmh_t
 * msg to 1250
 *
 * When the response for APP_CMD is received, the fcip driver will repackage it
 * as a FC frame with the Vendor unique TYPE_IPSD_FKIO and send it up to ipsd
 * (thru FC driver)
 *
 */
#define	PTIO_IPSD_APP_CMD			_ASIC_IOC(ASIC_FCIP, 0x00)	/* IPSd ioctl */
/*
 * Other non IPSd ioctls:
 */
#define	PTIO_FCIP_FRM_TX			_ASIC_IOC(ASIC_FCIP, 0x01)	/* Normal FC Frames TX to 1250 */
#define	PTIO_FCIP_DUMP_BI_REGS		_ASIC_IOC(ASIC_FCIP, 0x02)	/* dump BI regs per ptRegShow <BI> */
#define	PTIO_FCIP_DUMP_BI_STATS		_ASIC_IOC(ASIC_FCIP, 0x03)	/* dump BI regs per ptRegShow <BI> */
#define	PTIO_FCIP_DUMP_ERR_STATS	_ASIC_IOC(ASIC_FCIP, 0x04)	/* Trigger 1250 to dump error stats */
#define	PTIO_FCIP_EGID_ASSIGNED		_ASIC_IOC(ASIC_FCIP, 0x05)	/* destined for BI per VE LINK_UP */
#define	PTIO_FCIP_TUNNEL_UPORT		_ASIC_IOC(ASIC_FCIP, 0x06)	/* destined for GigE port for getting uport associated with tunnel in GigE */
#define	PTIO_FCIP_DUMP_GE_REG		_ASIC_IOC(ASIC_FCIP, 0x07)	/* dump GE stats */
#define	PTIO_FCIP_DUMP_GE_STATS		_ASIC_IOC(ASIC_FCIP, 0x08)	/* dump GE stats */
#define	PTIO_FCIP_DUMP_GE_STATS_NOSYNC		_ASIC_IOC(ASIC_FCIP, 0x09)	/* dump GE stats and CP wont block*/
#define	PTIO_FCIP_IP_MGMT_PACKET		_ASIC_IOC(ASIC_FCIP, 0x0a)	/* inband mgmt packet recieved */
#define	PTIO_FCIP_IP_MGMT_MAC_INIT		_ASIC_IOC(ASIC_FCIP, 0x0b)	/* inband mgmt mac init frame recieved */
#define	PTIO_FCIP_OLV_UPDATE		_ASIC_IOC(ASIC_FCIP, 0x0c)	/* olv usage: offset/len/value */
#define PTIO_FCIP_PORT_MAP			_ASIC_IOC(ASIC_FCIP, 0x0d) /* portmap */
#define PTIO_FCIP_BOARD_TYPE		_ASIC_IOC(ASIC_FCIP, 0x0e) /* board type */
#define PTIO_FCIP_DUMP_BI_INFO		_ASIC_IOC(ASIC_FCIP, 0x0f) /* BI-port info */
#define PTIO_FCIP_DUMP_VE_INFO		_ASIC_IOC(ASIC_FCIP, 0x10) /* VE-port info */
#define PTIO_FCIP_DUMP_GE_INFO		_ASIC_IOC(ASIC_FCIP, 0x11) /* GE-port info */
#define PTIO_FCIP_GET_PORT_STATE	_ASIC_IOC(ASIC_FCIP, 0x12) /* port state */
#define PTIO_FCIP_HELLO				_ASIC_IOC(ASIC_FCIP, 0x13) /* DPF hello */

#define	PTIO_FCIP_HTBT_MSG			_ASIC_IOC(ASIC_FCIP, 0x99)	/* 1250 HeartBeat Message */

/* Scimitar CPSAS ioctls */
#define PTIO_DUMP_REG				_ASIC_IOC(ASIC_CPSAS, 0x1)	/* Copy regs to 1250 shared mem */
#define PTIO_DUMP_STATS				_ASIC_IOC(ASIC_CPSAS, 0x2)	/* Copy stats to 1250 shared mem */
#define PTIO_CLR_PORT_STATS			_ASIC_IOC(ASIC_CPSAS, 0x3)	/* Clear Anzio/Sflipper port stats */
#define PTIO_HTBT_MSG				_ASIC_IOC(ASIC_CPSAS, 0x4)	/* 1250 heartbeat message */
#define PTIO_SEND_SAS_MSG			_ASIC_IOC(ASIC_CPSAS, 0x05)	/* Send message to SAS Driver */
#define	PTIO_SEND_CDC_MSG			_ASIC_IOC(ASIC_CPSAS, 0x05)
/* reuse the CPSAS value, but follow CDC naming convention */
#define PTIO_GE_ENABLE				_ASIC_IOC(ASIC_CPSAS, 0x06)	/* Enable the GE port - only for Diag */
#define PTIO_GE_DISABLE				_ASIC_IOC(ASIC_CPSAS, 0x07)	/* Disable the GE port - only for Diag */
#define PTIO_RUN_BIST				_ASIC_IOC(ASIC_CPSAS, 0x09)	/*  run BIST test */
/* ***** ZEUS FPGA ioctls - FCoE Specific ioctls ***** */
#define PTIO_ZM_CONFIG_EPORT    _ASIC_IOC(ASIC_FCOE,  0x01)
#define PTIO_ZM_CONFIG_FPORT    _ASIC_IOC(ASIC_FCOE,  0x02)
#define PTIO_ZM_ERR_MAC         _ASIC_IOC(ASIC_FCOE,  0x03)
#define PTIO_ZM_ADD_LKUP        _ASIC_IOC(ASIC_FCOE,  0x04)
#define PTIO_ZM_DEL_LKUP        _ASIC_IOC(ASIC_FCOE,  0x05)
#define PTIO_ZM_HOP_CNT         _ASIC_IOC(ASIC_FCOE,  0x06)
#define PTIO_ZM_VLAN_MAP        _ASIC_IOC(ASIC_FCOE,  0x07)
#define PTIO_ZM_SP_FC_FRAME     _ASIC_IOC(ASIC_FCOE,  0x08)
#define PTIO_ZM_SET_TRUNK_STATE _ASIC_IOC(ASIC_FCOE,  0x09)
#define PTIO_ZM_SET_DEF_VLAN    _ASIC_IOC(ASIC_FCOE,  0x0a)
#define PTIO_ZM_SET_C2_VCS      _ASIC_IOC(ASIC_FCOE,  0x0b)
#define PTIO_ZM_GET_CTRVAL      _ASIC_IOC(ASIC_FCOE,  0x0c)
#define PTIO_ZM_SET_FLOW_MAP    _ASIC_IOC(ASIC_FCOE,  0x0d)
#define PTIO_ZM_GET_MAC         _ASIC_IOC(ASIC_FCOE,  0x0e)
#define PTIO_ZM_ACL_ADD         _ASIC_IOC(ASIC_FCOE,  0x0f)
#define PTIO_ZM_ACL_DEL         _ASIC_IOC(ASIC_FCOE,  0x10)

/* DCE FCOE/eAnvil*/
#define PTIO_FCOE_DFLT_VFID		_ASIC_IOC(ASIC_FCOE, 0x20)	/* default vfid */
#define PTIO_FCOE_QMG_VSAN_PMAP	_ASIC_IOC(ASIC_FCOE, 0x21)	/* q to vsan map */
#define PTIO_FCOE_ERR_CTRL		_ASIC_IOC(ASIC_FCOE, 0x22)	/* error traps */

/* ************************** END of IOCTL defines ******************** */


/***********************************************************************
 * IOCTL structures
 ***********************************************************************/

/************************* Start ASIC_MISC *************************/
/************************* End ASIC_MISC *************************/

/************************* Start ASIC_DCE *************************/
/*
 * PTIO_VLAN_CFG
 * This IOCTL configures VLAN related parameters on a per interface basis
 * When Audit (cfg_valid_flags.audit = 1):
 *	if Caller (IFM) set cfg_valid_flags.cfg_vlan_enable = 1;
 *		driver returns cfg_flags.cfg_vlan_enable.
 *	if IFM set cfg_valid_flags.rx_port_vlan = 1;
 *		driver returns port_vlan.
 *	if IFM set cfg_valid_flags.rx_port_vlan_prio = 1;
 *		driver returns port_vlan_prio
 *
 *	Caller (IFM) should always use bitmap here:
 *	if IFM set cfg_valid_flags.rx_fwd_allowed = 1;
 *		driver returns all VLAN with cfg_flags.rx_fwd_allowed in bitmap format.
 *	else if IFM set cfg_valid_flags.rx_lrn_allowed = 1;
 *		driver returns all VLAN with cfg_flags.rx_lrn_allowed in bitmap format.
 *	else if IFM set cfg_valid_flags.tx_allowed = 1;
 *		driver returns all VLAN with cfg_flags.tx_allowed in bitmap format.
 *	else if IFM set cfg_valid_flags.tx_tagged = 1;
 *		driver returns all VLAN with cfg_flags.tx_tagged in bitmap format.
 *
 *		IOCTL structure:	dce_ioctl_ops_t
 *		buf[0] cast to:		dce_vlan_params_t
 *		return data:		dce_stat_t
 */
typedef struct {
	uint32_t		num_vlans;	/* number of VLANs of interest here */
	struct {
		uint32_t	rx_is_hybrid:1;		/* accept both tagged & untagged pkt */
		uint32_t	audit:1;			/* If set, audit the driver data	*/
		uint32_t	audit_update:1;		/* when audit set, driver update too.*/

		uint32_t	rx_fwd_allowed:1;
		uint32_t	rx_lrn_allowed:1;	/* If set, rx_lrn_allowed is valid */
		uint32_t	cfg_vlan_enable:1;	/* Only if it is set,
										 * cfg_flags.cfg_vlan_enable is vld */
		uint32_t	tx_allowed:1;
		uint32_t	tx_tagged:1;		/* vlan tagging (vid != 0) */
		uint32_t	rx_port_vlan:1;
		uint32_t	rx_port_vlan_prio:1;
		uint32_t	tx_pri_tagged:1;	/* priority tagging (vid = 0) */
	} cfg_valid_flags;
	struct {
		uint32_t	rx_is_hybrid:1;	/* accept both tagged & untagged pkt */
		uint32_t	audit_fail:1;	/* when audit set, driver return result	*/

		uint32_t	rx_fwd_allowed:1;
		uint32_t	rx_lrn_allowed:1;
		uint32_t	cfg_vlan_enable:1;	/* if set, tx frm may be tagged per tbl
										 * tx_tagged; if clr, no vlan tag */
		uint32_t	tx_allowed:1;
		uint32_t	tx_tagged:1;	/* vlan tagging (vid != 0) */
		uint32_t	vlan_all:1;		/* Set if applies to all VLANs. vlan_bitmap,
									 * num_vlans and vlans[] are ignored */
		uint32_t	vlan_bitmap:1;	/* Set if vlans[] is actually VLAN bitmap
									 * Otherwise, vlans[] is an array of VLANs if
									 * num_vlans is neither 4096 nor 0 */
		uint32_t	tx_pri_tagged:1;	/* priority tagging (vid = 0) */
	} cfg_flags;
	uint16_t		port_vlan;
	uint16_t		port_vlan_prio;
#ifndef __DCM_COMPILATION__
	uint32_t		*vlans;
#endif
} dce_vlan_params_t;

/*
 * Private vlan support
 */
#define	DCE_PRI_VLAN_IOCTL_ENT_MAX	512

enum {
	DCE_STAT_PVLAN_NOT_ETH_PORT = 1,
	DCE_STAT_PVLAN_IOC_OVER_SIZE = 2,
	DCE_STAT_PVLAN_INVALID_VID = 3,
	DCE_STAT_PVLAN_INVALID_EVID = 4,
	DCE_STAT_PVLAN_TABLE_FULL = 5,
	DCE_STAT_PVLAN_MEM_FAIL = 6
};

typedef struct {
	uint16_t			vid;
	uint16_t			evid;
} dce_pvid_t;

typedef struct {
	uint16_t			num;
	uint16_t			vlan_all:1;		/* Set if applies to all VLANs. */
	uint16_t			no_map:1;		/* reset all when vlan_all is on */
	uint16_t			status:14;		/* detail status */
	uint16_t			evid;			/* used when vlan_all is set */
#ifndef __DCM_COMPILATION__
	dce_pvid_t			*vid_evid;
#endif
} dce_pvid_ioc_t;

/* PTIO_GET_PRI_VLAN_IDX and PTIO_SET_PRI_VLAN_IDX */
typedef struct {
	uint16_t			vid;
	uint16_t			idx;
} dce_pri_vlan_idx_t;

typedef struct {
	uint16_t			num_vlan;
	struct {
		uint16_t		vlan_all:1;		/* Set if applies to all VLANs. */
	} cfg_flags;
	uint16_t			idx;			/* used when vlan_all is set */
#ifndef __DCM_COMPILATION__
	dce_pri_vlan_idx_t	*map;
#endif
} dce_pri_vlan_idx_ioc_t;

/* PTIO_GET_REMAP_VLAN and PTIO_SET_REMAP_VLAN */
typedef struct {
	uint16_t			idx;
	uint16_t			vid;
} dce_remap_vid_t;

typedef struct {
	uint16_t			num_idx;
#ifndef __DCM_COMPILATION__
	dce_remap_vid_t		*vid_map;
#endif
} dce_remap_vid_ioc_t;

#ifdef CONFIG_FLRN
/* PTIO_CFG_FAKE_LEARN */
typedef struct {
#define     FLRN_ON         1
#define     FLRN_OFF        2
#define     FLRN_UPDT_RBID  4
    uint8_t         cmd; 
    uint16_t        burst_size;
    uint8_t         burst_cnt;   
    uint8_t         rbridge_id;
    uint64_t        tot_new_macs;
    uint64_t        tot_macs_lrnd;      /* Populated by the driver for FLRN_OFF */
} dce_fake_lrn_cfg_t;
#endif

/************************* End ASIC_DCE *************************/

typedef struct _ioc_ftl_fcacc_cfg {
    int proxy_port; /* it is loopback port index */
    int egid;       /* EGID of loopback port */
    int redirect_port_ifid; /* for maranthon fcacc it is flipper-condor port */ 
	int proxy_port_ifid;    /* loopback port ifid, it is for time being.  */
} ioc_flt_fcacc_cfg_t;

/************************* FCOE(eAnvil) *************************/

/*
 * PTIO_FCOE_DFLT_VFID
 *
 * RETURN:
 *	DCE_STAT_OK
 *	DCE_STAT_INVLD_PARAM
 */
typedef struct {
    struct {
        uint16_t op_rnw:1;        /* operation: 1 is read, 0 is write */
        uint16_t match_vid:1;     /* valid flag for match_vid */
        uint16_t dflt_vfid_typ:1; /* valid flag for dflt_vfid_typ */
        uint16_t dflt_vfid:1;     /* valid flag for dflt_vfid */
    } flags;
    uint16_t match_vid;     /* VID that can use default VFID */
    uint16_t dflt_vfid_typ; /* default VFID type - input to EXM */
    uint16_t dflt_vfid;     /* default vfid */
} dce_fcoe_dflt_vfid_t;

/*
 * PTIO_FCOE_QMG_VSAN_PMAP
 *
 * RETURN:
 *	DCE_STAT_OK
 *	DCE_STAT_INVLD_PARAM
 */
typedef struct {
    struct {
        uint16_t op_rnw:1; /* operation: 1 is read, 0 is write */
    } flags;
    uint8_t valid_mask;    /* which entries are valid - bit0 -> qmg_pri0 */
    uint8_t vsan_prio[8];  /* VSAN tag priorityl idx0 -> qmg_pri0 */
} dce_fcoe_q_vsan_pmap_t;

/*
 * PTIO_FCOE_ERR_CTRL
 *
 * RETURN:
 *	DCE_STAT_OK
 *	DCE_STAT_INVLD_PARAM
 */
typedef struct {
	struct {
		uint16_t op_rnw:1; /* operation: 1 is read, 0 is write */

		/* valid flags for the corresponding items below */
		uint16_t rte_trp_eof_err:1;
		uint16_t rte_chk_re_err:1;
		uint16_t rte_trp_re_err:1;
		uint16_t rte_chk_opt_err:1;
		uint16_t rte_trp_opt_err:1;
		uint16_t rte_chk_ttl_err:1;
		uint16_t rte_trp_ttl_err:1;
	} flags;

	uint8_t rte_trp_eof_err:1;		/* RTE trap frames with bad eof */
	uint8_t rte_chk_re_err:1;		/* RTE check for routing-enable errors */
	uint8_t rte_trp_re_err:1;		/* RTE trap routing-enable errors */
	uint8_t rte_chk_opt_err:1;		/* RTE check for FCoE HDR option errors */
	uint8_t rte_trp_opt_err:1;		/* RTE trap FCoE HDR option errors */
	uint8_t rte_chk_ttl_err:1;		/* RTE check for TTL errors */
	uint8_t rte_trp_ttl_err:1;		/* RTE trap TTL errors */
} dce_fcoe_err_ctl_t;

#endif /* __PTIO_CODES_H__ */
