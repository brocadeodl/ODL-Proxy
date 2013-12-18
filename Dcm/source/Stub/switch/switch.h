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
 *       FabOS Switch Driver library header file.			   *
 ***************************************************************************/

#ifndef SWITCH_H
#define SWITCH_H 

#include "sys/fabos/fabcfg.h"
#include <switch/port.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define FREE(ptr) if (ptr != NULL) { free(ptr); ptr = NULL; }

#define MAX_DOMAIN 255
#define MIN_DOMAIN 1
#define MAX_PORT 256
#define SMI_MAX_PORTS 1800
//#define MYSWITCH 0

#define	FabOS_SW_FD	1

#define INVALID_DOMAIN(d)       \
        ((d) < MIN_DOMAIN || (d) >= MAX_DOMAIN)

typedef enum {
                FDS_INVALID                                     = -1,       /* Invalid State */
                FDS_UNSUPPORTED_SW                             = 0x0,    /* 00 */
                FDS_NOT_CAPABLE                                        = 0x1,    /* 01 */
                FDS_CAPABLE_AND_NOT_CONFIGURED             = 0x2,    /* 10 */
                FDS_CAPABLE_AND_CONFIGURED         = 0x3     /* 11 */
} fds_state_t;

extern inline int
slotPortToUserPort(int slot, int pic, int port) {
	return (1);
}


extern inline int
switchSetPortCfg(void *cfgData) 
{ 
	return (0); 
}

extern inline bool
switchFaulty()
{
    return false;
}

extern inline void switchDomainJoinComplete(int domain)
{
   return;
}

extern inline void switchReadyToAcceptCommands()
{
   return;
}

extern inline void switchActivationComplete()
{
   return;
}

extern inline int chassisStatusGet ()
{
    return (1);
}

static inline int chassisenable(void)
{
	printf("************ Calling Chassisenable Stub ******\n");
	return (0);
}

static inline int chassisdisable(void)
{
	printf("************ Calling Chassisdisable Stub ******\n");
	return (0);
}

static inline void switchDisable(void)
{
    return;
}

static inline void switchDisableNoFicon(void)
{
    return;
}

static inline int chassisBeaconSet(int on)
{
	if (on == 1) {
		printf("************ Calling ChassisBeaconSet(1) Stub ******\n");
	} else {
		printf("************ Calling ChassisBeaconSet(0) Stub ******\n");
	}
	return (0);
}

static inline int
hal_if_beacon_set( int blade, int uport,  uint32_t on)
{
	printf("\n hal_if_beacon_set *************");
	return (0);
}

extern inline int
switchGetPortCfg(sys_pcfg_bm_t *cfgData)
{
	return (0);
}

extern inline int
switchSetFDSState(fds_state_t state)
{
   return 1;
}

extern inline int
switchDisabled()
{
   return 1;
}

static inline int portBeaconSet(int port, int on, int slot) {
	printf("************ Calling Port Beacon  Stub ******\n");
	return (0);
}

static inline int portBeaconGet(int port) {
	return (0);
}

static inline int chassisNumSlots(void) {
	return (4);
}

#ifdef __cplusplus
}   
#endif
        
#endif // SWITCH_H
