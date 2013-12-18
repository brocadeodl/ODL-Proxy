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
 *   Author : Priya Ahuja                                                  *
 ***************************************************************************/

#ifndef __SYSTEMMONITOR_FWD_H__
#define __SYSTEMMONITOR_FWD_H__

/*
 * ATTENTION:
 *	All new FOS includes should be added to this
 *	global header file.  This will help to ensure
 *	that libInit() has been called for any libraries
 *	that are required.
 */
#include "dcm/fos_headers.h"

#ifdef __cplusplus
extern "C"
{
#endif
    #define SWSTATUS_NAME_MAX 15

    typedef char ssName_t[SWSTATUS_NAME_MAX];
    
    typedef struct swExPolicy {
        int idx;
    /*
    * Created union to hold float threshold values for marginal,
    * faulty, error ports and Missing SFPs
    * Components and integer for all the other components
    */
        union {
            int down;
            float down_per;
        } down;
        union {
            int marginal;
            float marginal_per;
        } marginal;
        ssName_t swName;
    } swExPolicy_t;

    typedef struct {
        swExPolicy_t  statPolicy[SWSTATUS_MAX];
        u_int num;
        u_int  ps;
        u_int  fan;
        u_int  temp;
        u_int  cpNum;
        u_int  wwnNum;
        u_int  bladeNum;
        u_int  coreBladeNum;
        u_int  picNum;
        u_int  maxport;
        int retVal;
        int numOfPorts;
        swExPolicy_t defStatPolicy[SWSTATUS_MAX];
    } swPolicyList_t;
    #define switchStatusPolicyGetData(x) 0; printf ("switchStatusPolicyGetData Successful\n")
    #define switchStatusPolicySetData(x, y) 0; printf ("switchStatusPolicySetData Successful\n")
	#define fwApplyCustomOrDefaultConfig(x, y, z) 0; if (0 == z) printf ("Flag is 0\n"); printf ("fwApplyCustomOrDefaultConfig Succesful\n")
	#define fwdSetCmdConfig(w, x, y, z) 0; printf ("fwSetCmdConfig Succesful\n")
	#define checkThreshConfig(x, y) 0; printf ("checkThreshConfig Succesful\n")
	#define fwApplyCustomOrDefaultConfigForagent(x, y, z) 0; if (0 == z) printf ("Flag is 0\n"); printf ("fwApplyCustomOrDefaultConfigForagent Successful\n")

#ifdef __cplusplus
}
#endif

#endif // __FWD_H__

