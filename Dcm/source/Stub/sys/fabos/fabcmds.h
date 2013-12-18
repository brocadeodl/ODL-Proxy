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
 *    Copyright (c) 1996-2004 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Module name: fabcmds.h
 *
 *    Description:
 *    	This file contains the *control* commands supported by the various
 *	kernel modules.
 */

#ifndef __SYS_FABCMDS_H__
#define	__SYS_FABCMDS_H__

#if 0
#include <sys/fabos/cdefs.h>
#include <linux/ioctl.h>

OS_CDECLS_BEGIN
#endif
/*
 * Control command definitions for the FAB_PORT object
 */
#define IOC_PT_MAGIC	(('F' + 'A' + 'B' + 'P' + 'O' + 'R' + 'T') & _IOC_TYPEMASK)
/* IOC_PT_ */
#define IOC_PT_GETST		_IO(IOC_PT_MAGIC, 1)	/* get link state */
#define IOC_PT_PTIO			_IO(IOC_PT_MAGIC, 2)	/* port based ioctls */
#define IOC_PT_IFIO			_IO(IOC_PT_MAGIC, 3)	/* if_id based ioctls */

#define IOC_PT_GETBUF		_IO(IOC_PT_MAGIC, 4)	/* get any available buffer */
#define IOC_PT_NOABUF		_IO(IOC_PT_MAGIC, 5)	/* notify of available buffer */
#define IOC_PT_RETBUF		_IO(IOC_PT_MAGIC, 6)	/* return used buffer */
#define IOC_PT_RDBUF		_IO(IOC_PT_MAGIC, 7)	/* read receive buffer */
#define IOC_PT_WRBUF		_IO(IOC_PT_MAGIC, 8)	/* write transmit buffer */
#define IOC_PT_POLL			_IO(IOC_PT_MAGIC, 9)	/* port poll */
#define IOC_PT_PHANTOM		_IO(IOC_PT_MAGIC, 10)	/* phantom translation */
#define IOC_PT_TX_RETBUF 	_IO(IOC_PT_MAGIC, 11)	/* return buffer for Tx port */
#define IOC_PT_PROC_FLT 	_IO(IOC_PT_MAGIC, 12) 	/* first stage of frame filtering */
#define IOC_PT_PROC_FLT_STAGE2 \
							_IO(IOC_PT_MAGIC, 13) 	/* second stage of frame filtering */
#define IOC_PT_SW_FILTERING	_IO(IOC_PT_MAGIC, 14) 	/* software frame filtering */

#define IOC_PT_SET_TPORT 	_IO(IOC_PT_MAGIC, 15)	/* set port for trunking */
#define IOC_PT_TRUNKABLE 	_IO(IOC_PT_MAGIC, 16)	/* find whether port is trunkable */
#define IOC_PT_GET_VCMODE 	_IO(IOC_PT_MAGIC, 17)	/* get port VC mode */
#define IOC_PT_SET_VCMODE 	_IO(IOC_PT_MAGIC, 18)	/* set port VC mode */
#define IOC_PT_PROC_ENTRY	_IO(IOC_PT_MAGIC, 19) 	/* create proc entry */
#define IOC_PT_XSEC_POLL	_IO(IOC_PT_MAGIC, 20)	/* multiple second port poll */
#define IOC_PT_DIAG_RETBUF	_IO(IOC_PT_MAGIC, 21)	/* return used diag buffer */
#define IOC_PT_UPDATE_CFG	_IO(IOC_PT_MAGIC, 22)	/* the port's config has been updated */
#define IOC_PT_CHK_VALID_ERRORS \
							_IO(IOC_PT_MAGIC, 23)	/* check validity of error messages */
#define IOC_PT_FISOFT_ZONING \
							_IO(IOC_PT_MAGIC, 24)	/* check validity of error messages */
#define IOC_PT_GET_FI_LIC 	_IO(IOC_PT_MAGIC, 25)	/* get FI_Port lic info */

/*
 * Control command definitions for the FAB_CHIP object
 */
#define IOC_CH_MAGIC	(('F' + 'A' + 'B' + 'C' + 'H' + 'I' + 'P') & _IOC_TYPEMASK)
/* IOC_CH_ */
#define IOC_CH_CHIO		_IO(IOC_CH_MAGIC, 1)		/* chip based ioctls */
#

/*
 * Control command definitions for the FAB_SWITCH object
 */
#define IOC_SW_MAGIC	(('F' + 'A' + 'B' + 'S' + 'W' + 'C' + 'H') & _IOC_TYPEMASK)
/* IOC_SW_ */
#define IOC_SW_GET_SWCSTAGE		_IO(IOC_SW_MAGIC, 1)	/* get SWC FSS main state */
#define IOC_SW_GET_SWCSUBSTAGE	_IO(IOC_SW_MAGIC, 2)	/* get SWC sub state */
#define IOC_SW_GEN_NUM			_IO(IOC_SW_MAGIC, 3)	/* get gen number for user port */
#define	IOC_SW_GET_SLOT_FROM_DEFAULT_AREA	_IO(IOC_SW_MAGIC, 4)
#define IOC_SW_GET_RGRP_ID		_IO(IOC_SW_MAGIC, 5)	/* get switch rgrp id */
#define IOC_SW_GET_RGRP_ID_TRILL		_IO(IOC_SW_MAGIC, 6)	/* get trill rgrp id used in callisto/callisto-f platform */



/*
 * Control command definitions for the FAB_FC object
 */
/*
 * WARNING: IOC_FC_ and IOC_ELS_ cmds should NOT overlap since they are
 * both directed to FC.
 */

#define IOC_FC_MAGIC	(('F' + 'A' + 'B' + 'F' + 'C') & _IOC_TYPEMASK)

/* IOC_FC_ */
#define	IOC_FC_FLUSHX		_IO(IOC_FC_MAGIC, 1)	/* flush an exchange by x_id */
#define IOC_FC_FLUSHPTX		_IO(IOC_FC_MAGIC, 2)	/* flush all exchanges by port */
#define IOC_FC_FLOGIACC		_IO(IOC_FC_MAGIC, 3)	/* tx the flogi acc */
													/* held because of routing */
#define IOC_FC_PLOGIACC		_IO(IOC_FC_MAGIC, 4)	/* tx the plogi acc */
													/* held because of routing */
#define IOC_FC_LB_REQUEST	_IO(IOC_FC_MAGIC, 5)	/* send a loop back test frame */
#define IOC_FC_FLARESP		_IO(IOC_FC_MAGIC, 6)	/* send an FLA response frame */
#define IOC_FC_SCRACC		_IO(IOC_FC_MAGIC, 7)	/* tx the scr acc */
													/* held because of fss ack */
#define IOC_FC_LAST_FRAME_SENT \
							_IO(IOC_FC_MAGIC, 8)	/* Notify FCPH that last frame */
									 				/* of unsolicitied command was sent */

/* Trill ioclt definitions */
#define IOC_TRILL_MAGIC (('T' + 'R' + 'I' + 'L' + 'L') & _IOC_TYPEMASK)

#define IOC_TRILL_DOMAIN_REACHABLE		_IO(IOC_TRILL_MAGIC, 1) /* arg1 = domain */
#define IOC_TRILL_DOMAIN_UNREACHABLE	_IO(IOC_TRILL_MAGIC, 2) /* arg1 = domain */
#define IOC_TRILL_DOMAIN_CAP_AVAILABLE	_IO(IOC_TRILL_MAGIC, 3) /* arg1 = domain */
#define IOC_TRILL_DOMAIN_VALID			_IO(IOC_TRILL_MAGIC, 4) /* arg1 = domain arg2 = PRINCIPLE_SWITCH_ID arg3 = PRINCE wwn.word[0] arg4 = PRINCE wwn.word[1] */
#define IOC_TRILL_DOMAIN_INVALID		_IO(IOC_TRILL_MAGIC, 5) /* arg1 = domain */
#define IOC_TRILL_BCAST_ADD				_IO(IOC_TRILL_MAGIC, 6) /* arg1 = port arg2 = bp handle */
#define IOC_TRILL_BCAST_DEL				_IO(IOC_TRILL_MAGIC, 7) /* arg1 = port arg2 = bp handle */
#define IOC_TRILL_DOMAIN_OUTPORT		_IO(IOC_TRILL_MAGIC, 8) /* arg1 = domain arg2 = outport */
#define IOC_TRILL_PORT_NBR_ID			_IO(IOC_TRILL_MAGIC, 9) /* arg1 = domain arg2 = port*/
#define IOC_TRILL_NOTIFY_ISL_PORT_ADD	_IO(IOC_TRILL_MAGIC, 10)/* arg1 = trill_isl_port_update */
#define IOC_TRILL_NOTIFY_ISL_PORT_DEL	_IO(IOC_TRILL_MAGIC, 11)/* arg1 = trill_isl_port_update */
#define	IOC_TRILL_ROUTE_CHANGE			_IO(IOC_TRILL_MAGIC, 12)/* arg1 = domain */
#define	IOC_TRILL_FCOE_INIT				_IO(IOC_TRILL_MAGIC, 13)/* arg1 = nports arg2 = port map */
#define	IOC_TRILL_SWITCH_STATE			_IO(IOC_TRILL_MAGIC, 14)/* arg1 = online/offline 1/0 */
#define	IOC_TRILL_E_PORT_CHANGE			_IO(IOC_TRILL_MAGIC, 15)/* arg1 = old master arg2 = old master bp handle arg3 = new master arg4 = new master bp handle */
#define IOC_TRILL_MCAST_ROOT			_IO(IOC_TRILL_MAGIC, 16)/* arg1 = mcast_root */
#define IOC_TRILL_HSL_TRUNK	            _IO(IOC_TRILL_MAGIC, 17)



/* IOC_ELS_ */
#define IOC_ELS_PLOGI	10	/* matches in sys/dev/fc/fc.h */
#define IOC_ELS_RRQ		11	/* matches in sys/dev/fc/fc.h */
#define IOC_ELS_FARP	12	/* matches in sys/dev/fc/fc.h */

#define IOC_ELS_RXID_TIMEOUT_SET  13	/* Set ELS rx_id timeout */
#define IOC_ELS_LAST_FRAME_SENT   14	/* Let ELS know last frame of unsol command sent */

/* FC Driver's IOC_SW_ */
#define IOC_SW_RXID_TIMEOUT_SET   1	/* Set SW rx_id timeout */
#define IOC_SW_LAST_FRAME_SENT	  2	/* Let SW know last frame of unsol command sent */

/* FC Driver's IOC_FCP_ */
#define IOC_FCP_LAST_FRAME_SENT   1	/* Let FCP know last frame of unsol command sent */

/* FC Driver's IOC_CT_ */
#define IOC_CT_RXID_TIMEOUT_SET   1	/* Set CT rx_id timeout */
#define IOC_CT_LAST_FRAME_SENT	  2	/* Let SW know last frame of unsol command sent */

#define IOC_AGD_LAST_FRAME_SENT   14	/* Let AGD know last frame of unsol command sent */

/* FC Driver's IOC_FCR_ */
#define IOC_FCR_RXID_TIMEOUT_SET   1	/* Set FCR rx_id timeout */
#define IOC_FCR_LAST_FRAME_SENT	  2		/* Let FCR know last frame of unsol command sent */

/* FC Driver's IOC_ISCSI_ */
#define IOC_ISCSI_LAST_FRAME_SENT	1	/* Let iSNS client know last frame of 
										 * unsol command sent */

/*
 * command macro for RXID_TIMEOUT_SET
 */
#define IOC_TYPE_RXID_TIMEOUT_SET(x)	\
  	((x == TYPE_FC_SW) ? IOC_SW_RXID_TIMEOUT_SET : \
    (x == TYPE_FC_SERVICES) ? IOC_CT_RXID_TIMEOUT_SET : \
    (x == TYPE_EXPORT_FCR) ? IOC_FCR_RXID_TIMEOUT_SET: \
    (x == TYPE_EXTEND_LINK) ? IOC_ELS_RXID_TIMEOUT_SET : 0)

/*
 * command macro for LAST_FRAME_SENT
 */
#define IOC_TYPE_LAST_FRAME_SENT(x)	\
  	((x == TYPE_FC_SW) ? IOC_SW_LAST_FRAME_SENT : \
    (x == TYPE_FC_SERVICES) ? IOC_CT_LAST_FRAME_SENT : \
    (x == TYPE_EXTEND_LINK) ? IOC_ELS_LAST_FRAME_SENT : \
    (x == TYPE_EXPORT_FCR) ? IOC_FCR_LAST_FRAME_SENT : \
    (x == TYPE_SCSI_FCP) ? IOC_FCP_LAST_FRAME_SENT : \
	(x == TYPE_ISCSI) ? IOC_ISCSI_LAST_FRAME_SENT : 0)


#endif	/* __SYS_FABCMDS_H__ */
