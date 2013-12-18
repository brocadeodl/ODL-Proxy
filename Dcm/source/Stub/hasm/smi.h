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

#ifndef __SYS_SMI_H__
#define __SYS_SMI_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * support two protocol services: fcsw and ethsw. max. number of
 * services is the sum of protocol service instanaces plus one
 * chassis service instance.
 * note the definition of following limits is based on unified
 * instance space (for multiple service protocols, i.e. FC & ETH).
 */
#define	MAX_PARTITIONS		8
#define	MAX_FCSW_PARTITIONS	MAX_PARTITIONS
#define	MAX_ETHSW_PARTITIONS	MAX_PARTITIONS
#define	MAX_SERVICES		MAX_PARTITIONS
#define	MAX_INSTANCES		(MAX_PARTITIONS+1) /* plus chassis0 */

#define	SMI_MAX_PHYSICAL_PORTS	448
#define	SMI_MAX_PORTS		1800
#define	SMI_MAX_COMPS		50
#define	SMI_SVCS		4	/* maximum # of service types */
#define	SMI_SSIZE		16	/* max string length */
#define	SMI_MAX_PORTS_PER_SLOT	96	/* maximum physical ports per slot */

/*
 * switch protocol types
 */
#define	MIN_PROT_TYPE	0
#define	PROT_FC		0
#define	PROT_GE		1
#define	PROT_ETH	2
#define	MAX_PROT_TYPE	3
#define VALID_PROT(prot_type)	((MIN_PROT_TYPE <= prot_type) && \
				 (MAX_PROT_TYPE > prot_type))
#define	MIN_PROT	0
#define	MAX_PROT	3	/* max # of protocols per service type */

/*
 * the port bitmap carries the most significant bit to indicate whether
 * the port is defined, and the remaining for the port types.
 */
#define	SMI_PORT_DEFINED	0x80

/*
 * FC protocol port type
 */
#define	SMI_PORT_FC		0	/* FC port, default */
#define	SMI_PORT_VE		1	/* VE port */
#define	SMI_PORT_GE		2	/* GE port (GE prot) */
#define	SMI_PORT_LP		3	/* LP port */
#define	SMI_PORT_ISL		4	/* ISL port */
#define	SMI_PORT_FPGA		5	/* FPGA port */
#define	SMI_PORT_FTRK		6	/* Fport Trunking */
#define	SMI_PORT_LISL		7	/* LISL port */
#define	SMI_PORT_VSAN		8	/* VSAN Fport */
#define	SMI_PORT_RESV		9	/* RESERVED Port */
#define	SMI_PORT_LE		10	/* VFR LE port */
#define	SMI_PORT_FCOE		11	/* FCOE port */
#define	SMI_PORT_TRILL		12	/* TRILL port */
#define	SMI_MAX_PORT_TYPE	13

#define	VALID_TYPE(type)	((type) >= 0 && (type) < SMI_MAX_PORT_TYPE)

typedef	struct	{
	short	pb_flag;		/* non-zero for valid bitmap */
	short	pb_prot;		/* protocol type, FC/GE */
	unsigned char	pb_bmap[SMI_MAX_PORTS];	/* port bitmap */
} pbmp_t;

#define	FOR_EACH_PORT(i, bmp)	for (i = 0; i < SMI_MAX_PORTS; i++) \
					if (PBMP_ISSET(bmp, i))

#define	PBMP_SET(bmp, i)	(bmp)->pb_bmap[i] = SMI_PORT_DEFINED, \
				(bmp)->pb_flag = 1;
#define	PBMP_ISSET(bmp, i)	((bmp)->pb_bmap[i] & SMI_PORT_DEFINED)
#define	PBMP_UNSET(bmp, i)	(bmp)->pb_bmap[i] = 0
#define	PBMP_ZERO(bmp)		memset(bmp, '\0', sizeof (pbmp_t))
#define	PBMP_SETTYPE(bmp, i, t)	(bmp)->pb_bmap[i] |= (t)
#define	PBMP_GETTYPE(bmp, i)	((bmp)->pb_bmap[i] & ~SMI_PORT_DEFINED)
#define	PBMP_SETPROT(bmp, p)	(bmp)->pb_prot = (p)

#define SMI_SLOTS   32  /* max number of slots per chassis */

#define SMI_EVT_SERVICE_READY  	1
#define SMI_EVT_SLOT_READY  	2
#define SMI_EVT_SYSTEM_READY  	3
#define SMI_EVT_SLOT_NOT_READY  4
#define SMI_EVT_SLOT_UP         5
#define SMI_EVT_SLOT_ACTIVE     6
#define SMI_EVT_SYNC_DUMP       7

/*
 * sync start/stop as the argument on sync-dump event
 */
#define SMI_ARG_START   1   /* sync start */
#define SMI_ARG_STOP    2   /* sync stop */

/*
 * cold/warm tag argument for service ready, slot ready, system ready
 * and slot not ready.
 */
#define SMI_ARG_COLD    0   /* startup or cold failover */
#define SMI_ARG_CPWARM  1   /* CP warm failover */
#define SMI_ARG_BPWARM  2   /* BP warm failover */

#ifdef __cplusplus
}
#endif

#endif
