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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Author : Vigneshkumar Bagavathsingh
 *   Description:
 *	   This header file contains license related stub routines
 *	   and structures
 */

#ifndef __MMAP_H__
#define __MMAP_H__

// using namespace WaveNs;
// using namespace DcmNs;

#ifdef __cplusplus
extern "C"
{
#endif

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
	eTYPE_PO,		/* Port Channel */
	eTYPE_TU,		/* Tunnel */
	eTYPE_INVALID
} interface_display_type_t;

typedef struct external_interface_s {
	port_tuple_t 				user_tuple;
	port_tuple_t				phy_tuple;
	interface_type_t			type;
	interface_display_type_t	display_type;
	int							QSFP_tuple_index;
} external_interface_t;

typedef struct neighbor_external_interface_s {
	port_tuple_t 				user_tuple;
	int							domain;
	interface_type_t			type;
	interface_display_type_t	display_type;
        int                         qsfp_idx;
} neighbor_external_interface_t;

#ifdef __cplusplus
}
#endif

#endif // LICENSE_H
