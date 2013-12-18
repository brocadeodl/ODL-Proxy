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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef THRESHOLDMONITORLOCALTYPESH_H
#define THRESHOLDMONITORLOCALTYPESH_H

#include "vcs.h"
#include <thresh/fwdPub.h>
#include <thresh/fwd.h>
namespace DcmNs
{

    typedef enum
    {
	    // SFPMONITORLOCALCONFIG,
	    THRESHOLDMONITORSETCPU,
	    THRESHOLDMONITORSETMEMORY,
	    SFPMONITORLOCALCONFIG,
	    SFPLOCALCONFIG,
	    SECLOCALCONFIG,
	    SECURITYLOCALCONFIG,
		INTERFACEMONITORLOCALCONFIG,
		INTERFACELOCALCONFIG,
		INTERFACELOCALSHOW,
		SFPLOCALSHOW,
		SECURITYLOCALSHOW
    }ThresholdMonitor;

    typedef enum
    {
	    FW_NONE = 1,
	    RASLOG,
	    SNMP,
	    ALL,
	    FW_DEFAULT
    } actionType;


#define CPU_POLL_DEFAULT 120
#define CPU_RETRY_DEFAULT 3
#define CPU_LIMIT_DEFAULT 75
#define MEMORY_POLL_DEFAULT 120
#define MEMORY_RETRY_DEFAULT 3
#define MEMORY_LIMIT_DEFAULT 60
#define MEMORY_LOW_LIMIT_DEFAULT 40
#define MEMORY_HIGH_LIMIT_DEFAULT 70
#define DEFAULT_POLICY 0
#define CUSTOM_POLICY 1
#define TH_INVALID_THRESH_VALUE -255
#define TH_INVALID_ALERT_VALUE 0xffffffff
#define MAX_AREA_VAL 5
#define SET 1
#define UNSET 0
#define NONEACT 1
#define ALL 2
#define EMAIL 4
#define RASLOG 8
#define RASLOGEMAIL 12	
#define SECONDS 2
#define PORTFENCE 16

typedef enum {
        GBIC_MINTYPE = 0,
        GBIC_1GSR = GBIC_MINTYPE,
        GBIC_1GLR,
        GBIC_10GSR,
        GBIC_10GLR,
	GBIC_10GUSR,
        GBIC_QSFP,
        MAX_OUI_VAL
}gbicSfpTypes;

typedef enum {
        INT_MINTYPE = 0,
        INT_GIGE = GBIC_MINTYPE,
        MAX_INT_VAL
}IntTypes;

typedef struct {
        int lowthresh;
        int highthresh;
        int buffer;
} areaValue;

typedef struct {
	unsigned int aboveHigh;
	unsigned int aboveLow;
	unsigned int belowHigh;
	unsigned int belowLow;
}alertValue;


typedef struct  {
        int sfptypename;
        alertValue sfpAlertVal;
        areaValue sfpareaval[MAX_AREA_VAL];
}typeValue;

typedef struct {
	int area;
	int timebase;
        alertValue sfpAlertVal;
	areaValue threshVal;
}secThreshAlertVal;

/* the values are filled in format of
 * {sfptype, {lowthresh, highthresh, buffer}}
 */
static const typeValue sfptypeval[MAX_OUI_VAL] = {
	{GBIC_1GSR, 
		{threshold_monitor__bm_supported_actions_raslog, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_raslog},
		{{-40, 100, 0},
		 {8, 1122, 0},
		 {60, 1000, 0},
		 {2, 12, 0},
		 {3000, 3600, 0}}},

	{GBIC_1GLR,
		{threshold_monitor__bm_supported_actions_raslog, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_raslog},
		{{-45, 90, 0},
		 {6, 501, 0},
		 {71, 794, 0},
		 {1, 45, 0},
		 {2900, 3700, 0}}},

	{GBIC_10GSR,
		{threshold_monitor__bm_supported_actions_raslog, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_raslog},
		{{-5, 90, 0},
		 {32, 1000, 0},
		 {251, 794, 0},
		 {4, 11, 0},
		 {3000, 3600, 0}}},

	{GBIC_10GLR,
		{threshold_monitor__bm_supported_actions_raslog, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_raslog},
		{{-5, 88, 0},
		 {16, 1995, 0},
		 {158, 1585, 0},
		 {15, 85, 0},
		 {2970, 3630, 0}}},
	{GBIC_10GUSR,
		{threshold_monitor__bm_supported_actions_raslog, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_raslog},
		{{-5, 100, 0},
          	 {32, 2000, 0},
		 {126, 2000, 0},
		 {3, 11, 0},
		 {3000, 3600, 0}}},
		 
	{GBIC_QSFP,
		{threshold_monitor__bm_supported_actions_raslog, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_raslog},
		{{-5, 75, 0},
		 {40, 1995, 0},
	  	 {0, 0, 0},
		 {1, 10, 0},
		 {2970, 3630, 0}}}
};

/* the values are filled in format of
 * {secArea, 
	{aboveHigh, AL, BH, BL}
	{lowthresh, highthresh, buffer}}
 */

static const secThreshAlertVal secTypeVal [] = {
	{SECURITY_TELNET, TH_TMBASE_MIN,
		{threshold_monitor__bm_supported_actions_raslog, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none },
		{1, 2, 0}},
	{SECURITY_LOGIN, TH_TMBASE_MIN,
		{threshold_monitor__bm_supported_actions_raslog, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none },
		{1, 2, 0}}
};
#define MAX_SEC_AREA sizeof(secTypeVal)/sizeof(secThreshAlertVal)

typedef struct  {
	int area;
	int timebase;
	alertValue interfaceAlertVal;
   	areaValue interfaceareaval;
} interfaceAreaValue;

static const interfaceAreaValue interfacetypeval [] = {
	{GEPORT_FRAME, TH_TMBASE_MIN,
		{threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none },
		{12, 300, 0}},
	{GEPORT_CRCALIGN, TH_TMBASE_MIN,
		{threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none },
		{12, 300, 0}},
	{GEPORT_SYMBOL, TH_TMBASE_MIN,
		{threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none },
		{0, 5, 0}},
	{GEPORT_IFG, TH_TMBASE_MIN,
		{threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none },
		{5, 100, 0}},
	{GEPORT_TXCFI, TH_TMBASE_MIN,
		{threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none,
		 threshold_monitor__bm_supported_actions_none, threshold_monitor__bm_supported_actions_none },
		{1, 20, 5}}
};

}

#endif                                            //THRESHOLDMONITORLOCALTYPESH_H
