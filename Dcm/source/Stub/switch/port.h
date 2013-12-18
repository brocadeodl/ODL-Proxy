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
 * This file serves as a placeholder to satisfy compilation of the plugin infrastructure
 * outside of the "normal" make process.
 */

#ifndef __PORT_H_
#define __PORT_H_

#include "sys/fabos/fabcfg.h"
#include <fabos/mmap.h>

using namespace WaveNs;

namespace DcmNs
{

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {

    /* interface type under module ID 2 */
    IF_TYPE_UNKNOWN_IF  = 2,    /* reserved */
    IF_TYPE_PHYSICAL_SFP    = 3,    /* physical sfp  interface */
    IF_TYPE_PHYSICAL    = 6,    /* physical GE interface */
    IF_TYPE_L2_TRUNK    = 10 ,  /* external (user visible) trunk */
    IF_TYPE_L2_TRUNK_INT= 14 ,  /* internal L2 trunk */
    IF_TYPE_SVI         = 18,   /* Switch Virtual Ifc- IP maps on multiple L2 */
    IF_TYPE_LOOPBACK_IF = 22,   /* L3 loopback interface */
    IF_TYPE_EMB_PHYSICAL= 26 ,  /* embedded physical interface */
    IF_TYPE_L2_TRUNK_ISL= 30 ,  /* VCS Inter-Switch-Link Trunk */
    
    /* interface type under module ID 3 */
    IF_TYPE_MOD3_RSVD1  = 7 ,   /* reserved */
    IF_TYPE_MOD3_RSVD2  = 11 ,  /* reserved */
    IF_TYPE_MOD3_RSVD3  = 15 ,  /* reserved */
    IF_TYPE_MOD3_RSVD4  = 19 ,  /* reserved */
    IF_TYPE_MOD3_RSVD5  = 23 ,  /* reserved */
    IF_TYPE_MOD3_RSVD6  = 27 ,  /* reserved */
    IF_TYPE_MOD3_RSVD7  = 31 ,  /* reserved */
    /*Fix me is it 17 now ?*/
    IF_TYPE_EXT_MAX_TYPES = 16  /* the maximum snmp visible DCE types supported */
} ifm_type_t;

/* Interface speed */
typedef enum {
    IF_SPEED_UNKNOWN = -1,
    IF_SPEED_10G = 0,
    IF_SPEED_100M = 1,
    IF_SPEED_1G =2,
    IF_SPEED_40G = 3,
    IF_SPEED_100G = 4
} ifm_speed_t;

typedef unsigned int ifindex_t;

static inline ifm_speed_t ifindex_get_speed(ifindex_t ifidx)
{
    return (IF_SPEED_10G);
}

// declare port configuration function that is used to send port config bitmap to the switch driver
extern inline
int portSetCfgBm(int port, pcfg_bm_t *pcfg_bm) {
	return (1);
}

extern inline int
portSetLgclCfgBm(int port, pcfg_bm_t *pcfg_bm) {
	return (1);
}

extern inline int
portSetDistance(int port, int estd_dist) {
		return (1);
}

extern inline int
portEnable_direct(int port) {
	return (1);
}

extern inline int
portDisable_direct(int port) {
	return (1);
}

extern inline int
portPresent(int port) {
	return (1);
}

extern inline int
portDisableWithReason(int port, int dis_reason) {
	return (1);
}

extern inline int portFcoEPort(int port) 
{
	port = port;
	return (0);
}

extern inline int portNOSIsFC(int port) 
{
	port = port; 
	return (0);
}

extern inline int portTRILLPort(int port) 
{
	port = port;
	return (0);
}

extern inline int portGetLinkSpeed(int port)
{
	port = port;
	return (0);
}

#define getInterfaceDisplayTypeString(port_index) InterfaceTypeToString(getInterfaceDisplayType(port_index))
#define getInterfaceDisplayTypeStringRemote(port_index) InterfaceTypeToString(getInterfaceDisplayTypeRemote(port_index))

extern inline int
getPortTupleRemote(int port_index, int *tuple_domain, int *tuple_blade, int *tuple_port,  int *qsfp_idx)
{
	tuple_domain = tuple_domain;
	tuple_blade = tuple_blade;
	tuple_port = tuple_port;
        qsfp_idx = qsfp_idx;

	return (-1);
}

extern inline int
getPortTuple(int port_index, int *tuple_domain, int *tuple_blade, int *tuple_port)
{
	tuple_domain = tuple_domain;
	tuple_blade = tuple_blade;
	tuple_port = tuple_port;

	return (-1);
}

extern inline int
getPhyPortTuple(int port_index, int *tuple_domain, int *tuple_blade, int *tuple_port)
{
	tuple_domain = tuple_domain;
	tuple_blade = tuple_blade;
	tuple_port = tuple_port;

	return (-1);
}

extern inline interface_type_t getInterfaceType(int port_index) 
{
	port_index = port_index;

	return (eINT_INVALID);
}

extern inline interface_display_type_t getInterfaceDisplayType(int port_index)
{
	port_index = port_index;

	return (eTYPE_INVALID);
}

extern inline interface_type_t getInterfaceTypeRemote(int port_index) 
{
	port_index = port_index;

	return (eINT_INVALID);
}

extern inline int
getQSFPTupleIndex(int port_index)
{
        port_index = port_index;
        return (-1);
}

extern inline interface_display_type_t getInterfaceDisplayTypeRemote(int port_index)
{
	port_index = port_index;

	return (eTYPE_INVALID);
}

extern inline char * InterfaceTypeToString(interface_display_type_t display_type) 
{
	display_type = display_type;

	return (NULL);
}
extern inline int getPortByPWWN(wwn_t *p_wwn)
{

		return (p_wwn->chars[1] +
			((p_wwn->chars[0] == 0x2e) ? 256 : 0));

}

extern inline int getBladeID(int slot, int *blade)
{
        return (0);
}


#ifdef __cplusplus
}
#endif

}  // end namespace DcmNs

#endif // __PORT_H_
