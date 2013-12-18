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
 *    Description:
 *      ASIC DRIVER BLOCK DEFINITION
 *
 *		This file used to be in this directory:
 *			fabos/src/sys/include/asic
 *		but it's "ct mv" here in order for the 1250 microcode SW
 *		to share the same PTIO_* codes defined in ptio_codes.h.
 */

#ifndef __SYS_ASIC_BLK_H__
#define	__SYS_ASIC_BLK_H__


/*
 * The MODULE in this structure is any log that is per module based, such as
 * during module init, or ASIC libraries. It is also used for defining various
 * IOCTL.
 *
 * Important:
 * 1) whenever a new type is added, please add it to the end of list.
 * 2) add a new RAS trace level control entry in asic_func_table[] for every
 *	   	existing asic driver. The asic_func_table[] is defined in
 *    	xxx_debug.c where xxx is the respective driver's name.
 *		(ie. condor, pinball)
 *		Even if that block is not defined for that asic driver, a dummy entry
 *		still needs to be created.
 */
#define	ASIC_MOD		0
#define UPSM			1
#define PHYSM			2
#define SNSM			3
#define LPSM			4
#define LKSM			5
#define TRKSM			6
#define FLTSM			7
#define ASIC_RTE		8
#define BUF				9
#define INF				10
#define TXRX			11
#define ASIC_INIT		12
#define	ASIC_INTR		13
#define ASIC_HA			14
#define ASIC_ERR		15
#define	ASIC_MISC		16	/* including hot plug, timer, debug */
#define	ASIC_BLM		17
#define ASIC_FCR		18
#define ASIC_FCIP		19
#define ASIC_ISCSI		20
#define ASIC_DEBUG		21	/* for swc_test */
#define ASIC_CPSAS		22
#define ASIC_CPCDC		22	/* no change! reuse the value for sas */

#define ASIC_DCE		23
#define ASIC_FCOE		24

#define ASIC_VF			25

/*
 * ASIC dependent blks. Each ASIC driver can rename the followings
 * to it's own blk. such as #define C2_FTB     ASIC_SPECIFIC1
 */
#define ASIC_SPECIFIC1	26
#define ASIC_SPECIFIC2	27
#define ASIC_SPECIFIC3	28
#define ASIC_SPECIFIC4	29
#define ASIC_SPECIFIC5	30
#define ASIC_SPECIFIC6	31
#define ASIC_SPECIFIC7	32
#define ASIC_SPECIFIC8	33
#define ASIC_SPECIFIC9	34
#define ASIC_SPECIFIC10	35
#define ASIC_SPECIFIC11	36
#define ASIC_SPECIFIC12	37
#define ASIC_SPECIFIC13	38
#define ASIC_SPECIFIC14	39
#define ASIC_SPECIFIC15	40
#define ASIC_SPECIFIC16	41

#define FUNC_ID_END		42

/*
 * ASIC driver event offset and mask.
 */
#define ASIC_SW_ACCESS_OFFSET		31	/* ASIC SW access offset */
#define ASIC_SW_ACCESS		0x80000000	/* ASIC SW access */
#define ASIC_BLK_OFFSET				24	/* ASIC functional block offset */
#define ASIC_BLK			0x7f000000	/* ASIC functional block mask */
#define ASIC_SUBBLK_OFFSET			22	/* ASIC sub-block offset */
#define ASIC_SUBBLK			0x00c00000	/* ASIC sub-block mask */
#define ASIC_BULK_CMD_OFFSET		21	/* Bulk ioctl flag offset */
#define ASIC_BULK_CMD		0x00200000	/* Bulk ioctl flag */
#define ASIC_ASYNC_CMD_OFFSET		20	/* ASNYC ioctl flag offset */
#define ASIC_ASYNC_CMD		0x00100000	/* ASYNC ioctl flag */
#define ASIC_EVT_TYPE_OFFSET		16	/* ASIC event type offset */
#define ASIC_EVT_TYPE		0x000f0000	/* ASIC event type */
#define ASIC_EVT_OFFSET				0	/* ASIC event offset */
#define ASIC_EVT			0x0000ffff	/* ASIC event */

/*
 * ASIC driver event types.
 */
#define EVT_TYPE_IOC		0x1			/* IOCTL (PTIO) */
#define EVT_TYPE_INT		0x2			/* Interrupt */
#define EVT_TYPE_INP		0x3			/* Event */
#define EVT_TYPE_TMR		0x4			/* Timer */
#define EVT_TYPE_CHIO		0x5			/* IOCTL (CHIO) */
#define EVT_TYPE_CHEVT		0x6			/* Chip event to upper layers */

#define GET_EVENT_BLK(evt)	((evt & ASIC_BLK) >> ASIC_BLK_OFFSET)
#define GET_EVENT_TYPE(evt)	((evt & ASIC_EVT_TYPE) >> ASIC_EVT_TYPE_OFFSET)

/* event definition macros */
#define _ASIC_IOC(blk, ioc)	\
	(((blk) << ASIC_BLK_OFFSET) | (EVT_TYPE_IOC << ASIC_EVT_TYPE_OFFSET) | \
	((ioc) << ASIC_EVT_OFFSET))
#define _ASIC_INT(blk, int) \
	(((blk) << ASIC_BLK_OFFSET) | (EVT_TYPE_INT << ASIC_EVT_TYPE_OFFSET) | \
	((int) << ASIC_EVT_OFFSET))
#define _ASIC_INP(blk, inp)	\
	(((blk) << ASIC_BLK_OFFSET) | (EVT_TYPE_INP << ASIC_EVT_TYPE_OFFSET) | \
	((inp) << ASIC_EVT_OFFSET))
#define _ASIC_TMR(blk, tmr) \
	(((blk) << ASIC_BLK_OFFSET) | (EVT_TYPE_TMR << ASIC_EVT_TYPE_OFFSET) | \
	((tmr) << ASIC_EVT_OFFSET))
#define _ASIC_CHIO(blk, chio) \
	(((blk) << ASIC_BLK_OFFSET) | (EVT_TYPE_CHIO << ASIC_EVT_TYPE_OFFSET) | \
	((chio) << ASIC_EVT_OFFSET))
#define _ASIC_CHEVT(blk, evt) \
	(((blk) << ASIC_BLK_OFFSET) | (EVT_TYPE_CHEVT << ASIC_EVT_TYPE_OFFSET) | \
	((evt) << ASIC_EVT_OFFSET))
#endif /* __SYS_ASIC_BLK_H__ */
