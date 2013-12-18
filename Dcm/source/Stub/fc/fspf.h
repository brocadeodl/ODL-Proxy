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

/***************************************************************************
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Meenakshi Garg                                               *
 *   Description:                                                          *    
 *       This header file contains fspfd related stub routines             *
 *       and structures                                                    *    
 ***************************************************************************/

#ifndef FC_FSPF_H
#define FC_FSPF_H

#include <switch/switch.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Path Descriptor Block
 */     
typedef struct pdb {
        u_long  pflags;                 /* Flags */
//        portBitMap_t sPortsP;           /* Bit map of static ports */
        int     portcnt;                /* Number of ports on this path */
//        portBitMap_t portsP;            /* Bit map of ports on this path */
        int     metric;                 /* Cost of the path to domain */
        int     outport;                /* Outgoing port for this path */
        int     curr_bw_alloc;          /* Total bandwidth using this link */
        int     bw_utilization;         /* bandwidth utilization */
        int     total_bw;               /* Total Bandwidth of link/trunk */
} pdb_t;

/*      
 * pdb flags
 */     
#define F_STATIC        0x01            /* Static unicast path */
#define F_FLOAT         0x02            /* Floating static path */
#define F_NEW_PDB       0x04            /* PDB allocated, no routes assigned */
#define F_PATH_DOWN     0x80000000      /* Path is down */
#define F_DEBUG         0x10            /* Print the virtual port info */

#define MAX_PATHS       16              /* Max # of paths to a destination */

#ifdef __cplusplus
}
#endif

#endif // FC_FSPF_H
