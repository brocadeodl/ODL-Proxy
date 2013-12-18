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

#ifndef __SYSTEMMONITOR_FWDPUB_H__
#define __SYSTEMMONITOR_FWDPUB_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include <utils/malloc.h>
/* dummy stub for fabric watch thresh fwdPub.h */

#define THA_MAX_NAME 48
#define SWSTATUS_LOG_SIZE 2048
#define PORT_NAME_MAX 48
#define IP_BUF_SZ 48
#define MAX_SUPPORTED_MAIL_ADDRESS 5
#define THA_ALL_AREAS -100
#define THA_ALL_INDEX -200
#define THA_LABEL_LEN 200
#define MAX_TIME_LEN    32
static inline int thaThresholdDisable(int agent, int area, int snmpIdx)
{
    return (0);
}

static inline int thaThresholdEnable(int agent, int area, int snmpIdx)
{
    return (0);
}

/* FRU definitions */
#define MAX_FRU_ELEM    12
#define FW_FRU_UNKNOWN  -1
#define FW_FRU_ABSENT   0x01  /* fru absent  */
#define FW_FRU_INSERTED 0x02  /* fru inserted and powered off */
#define FW_FRU_ON   0x04  /* fru ON     */
#define FW_FRU_OFF  0x08  /* fru ON but disabled  */
#define FW_FRU_FAULTY   0x010 /* fru faulty  */

typedef enum {
    thAbsent = -1,      /* Threshold does not exist */
    thStart,        /* Threshold created/started */
    thChanged,      /* Threshold value changed */
    thExceeded,     /* exceeded a limit (placeholder) */
    thBelow,        /* Threshold value exceeded the lower limit */
    thAbove,        /* Threshold value exceeded the upper limit */
    thInBetween,        /* Threshold value is in between */
    thLowerBufCrsd,     /* Threshold value has crossed lower buffer to in between*/
    thUpperBufCrsd      /* Threshold value has dropped below from upper limit to in between */
} thEventType;

#define thEventMin      thStart
#define thEventMax      thUpperBufCrsd //Changed this to thUpperBufCrsd from thInBetween after adding new thresholds.
#define thEvents        (thEventMax - thEventMin + 1)

#ifndef EVENT_MIN_TYPE
#define EVENT_MIN_TYPE  thEventMin
#endif

#ifndef EVENT_MAX_TYPE
#define EVENT_MAX_TYPE  (thEventMax + 1)
#endif


typedef enum {
    thInvalidClass = -1,
    thInvalidArea = -2,
    thInvalidIndex = -3,
    thInvalidLevel = -4,
    thInvalidEvent = -5,
    thInvalidAlarm = -6,
    thInvalidBS = -7,
    thInvalidLow = -8,
    thInvalidHigh = -9,
    thInvalidTB = -10,
    thInvalidUnit = -11,
    thInvalidField = -12,
    thUnlicensed = -13,
    thUninitialized = -14,
    thInvalidObjectType = -15,
    thInvalidFru    = -16,
    thInvalidFruNum = -17,
    thUnIntializedVar = -18,
    thCBError = -19,
    thUninitializedFWD = -20,
    thInvalidFruAlarmState = -21,
    thInvalidFruAlarmLevel = -22,
    thInvalidArg = -23,
    thInvalidSize = -24,
    thNullPtr = -25
} thAccessStatus;

/* Event types/reasons */
/*
 * These must be in sync with fwstat_e but the naming convention
 * is horrible, for backward compatibility we will keep it in
 */
typedef enum {
    swstatStatusUnknown = 0,    /* Unknown or unclear */
    swstatStatusHealthy,        /* Healthy or up */
    swstatStatusMarginal,       /* Marginal or a mixture */
    swstatStatusDown,       /* Down, with a major issue */
    swstatStatusUnmonitored     /* Don't want to tell for some reason */
} swstatStatus;

typedef struct swStatusPolicy {
    int down;
    int marginal;
} swStatusPolicy_t;

typedef struct {
    swstatStatus    swStatus;
    char        swStatusName[THA_MAX_NAME];
    char        statusLastLog[SWSTATUS_LOG_SIZE];
    int         retVal;
} fwSwStatus_t;

typedef enum {
    SWSTATUS_MIN = 0,
    SWSTATUS_SWMODULE = SWSTATUS_MIN,
    SWSTATUS_POWER_SUPPLIES,
    SWSTATUS_TEMPERATURES,
    SWSTATUS_FANS,
    SWSTATUS_WWN,
    SWSTATUS_CP,
    SWSTATUS_BLADE,
    SWSTATUS_CORE_BLADE,
    SWSTATUS_FLASH,
    SWSTATUS_MARGINAL_PORTS,
    SWSTATUS_FAULTY_PORTS,
    SWSTATUS_MISSING_GBICS,
    // SWSTATUS_PIC,
    SWSTATUS_ERROR_PORTS,
    SWSTATUS_MAX
} swStatusType;

/*
 * Possible health states of the switch
 */
/*
 * These must be in sync with swstatStatus but the naming convention
 * of it is horrible, for backward compatibility we will keep it
 * swstatStatus in.  The members are 1-1 mapped
 */
typedef enum {
    FW_SWITCH_UNKNOWN = 0,      /* Unknown state */
    FW_SWITCH_HEALTHY,      /* Healthy switch state */
    FW_SWITCH_MARGINAL,     /* Marginal switch state */
    FW_SWITCH_DOWN,         /* Down switch state */
    FW_SWITCH_UNMONITORED       /* For some reason is not monitored */
} fwState_e;

/*
 * Possible health states of the port
 */
typedef enum {
    FW_PORT_HEALTHY,    /* Healthy port state */
    FW_PORT_MARGINAL,   /* Marginal port state */
    FW_PORT_FAULTY,     /* Faulty port state */
    FW_PORT_OFFLINE,     /* Offline port state */
    FW_PORT_ERROR,      /* Error port state */
    FW_PORT_YELLOW,
    FW_PORT_GREEN,
    FW_PORT_NOT_MONITORED /* if all the areas a*/
} fwPortHealth_e;

/*
 * Struct for hardware components
 */
typedef struct {
    swStatusType    hwType;     /* hardware monitored */
    fwState_e   state;      /* health of each component */
    int     numBad;     /* number of bad components */
    int     numAbsent;  /* number of absent components */
} hwObj_t;

/*
 * Struct for each individual port and its health
 */
typedef struct {
    int area;               /* area of port */
    char    portName[PORT_NAME_MAX + 1];    /* name of port */
    fwPortHealth_e  healthState;        /* health state of port */
} portInfo_t;

typedef struct {
    int area;           /* area of port */
    fwPortHealth_e  healthState;    /* health state of port */
} portHealth_t;

/*
 * Struct for port health states
 *
 * IMPORTANT NOTE
 * This implementation is only good if FW_MAX_PORT <= 256
 * once/if it reaches 512, it will not be able to go through
 * the ipc because it exceeds the current 3k ipc limit
 *
 * numPorts = -1 signifies that some type of error has occured
 */

#define FW_MAX_PORT 512

typedef struct {
    int     numPorts;   /* number of ports in healthArray */
    portHealth_t    healthArray[FW_MAX_PORT];   /* porthealth_t array */
} portStateArray_t;

/*
 * Switch Health Report Struct
 *
 * IMPORTANT NOTE
 * Allocate sizeof(fwSwitchHealthReport_t) + sizeof(portInfo_t)*MAX_PORT
 * and assign to *portHealth prior to calling fwSwitchHealthReportGet()
 * and do not forget to FREE YOUR MEMORY
 */
typedef struct {
    char        switchName[PORT_NAME_MAX + 1]; /* switch name */
    char        ipAddress[40]; /* switch ip address */
    fwState_e   healthState;    /* health of switch */
    time_t      date;       /* report generation time */
    unsigned long   duration;   /* duration switch is in state */
    int         numHWObjects;   /* number of HW objects hwObj */
    hwObj_t     hwObj[SWSTATUS_MAX]; /* array of hw and healths */
    int         numPorts;   /* number or ports within portHealth */
    portInfo_t  *portHealth;    /* ptr to portInfo_t *arr[MAX_PORT] */
} fwSwitchHealthReport_t;

/* FRU data structure */
typedef enum {
    FW_FRU_MIN = 0,
    FW_FRU_SLOT = FW_FRU_MIN,
    FW_FRU_PS,
    FW_FRU_FAN,
    FW_FRU_WWN,
    FW_FRU_SFP,
    // FW_FRU_PIC,
    FW_FRU_MAX
} fwFruType_e;

#define FRU_NAME_MAX 32

// Maximum size of OUI
#define MAX_OUI_LEN 8

/*
 * Exported IPC functions from FWD
 */

typedef enum ops {
	CLI_OP_SHOW,
	CLI_OP_CONFIG,
	CLI_OP_ENABLE,
	CLI_OP_DISABLE
} OPS_T;

#define MAX_PROCESS_NAME 256

enum {
    MEM_ST_TH_UK =0,
    MEM_ST_TH1,
    MEM_ST_TH2,
    MEM_ST_TH3,
    MAX_MEM_TH_LEVEL
};

typedef struct pcbExtInterface {
	int index;
	char name[MAX_PROCESS_NAME ];
	unsigned int pid;

	int cpuUsage;
	int   cpuUsageLimit;
	int   pollingInterval;
	int   noOfRetries;
	int   cpuAction ;


	int   memUsage;
	int   memTh[MAX_MEM_TH_LEVEL];

	int   memRetries;
	int   memNoOfRetries;

	int   memPollingInterval;
	int   memAction;

	int   totalMemory;

	int   errorCode;
/* Need to remove later */
	int   memUsageLimit;
} pcbExtInterface_t;

typedef struct {
    fwFruType_e fru_type; /* i.e SLOT, FAN, WWN etc */
    int alarm_state; /* FRU state : FRU_OFF, FRU_INSERTED etc */
    int alarm_action; /* E-mail, Errlog */
    char fru_name[FRU_NAME_MAX]; /* string of fru object */
} fw_fru_cfg_t;

typedef struct {
    int fruNum;  /* number of FRU objects */
    fw_fru_cfg_t fruObj[FW_FRU_MAX];
    fw_fru_cfg_t defFruObj[FW_FRU_MAX];
    int retVal;
} fw_fruObj_t;

typedef enum {
    AGENT_MIN_TYPE = 0,
    AGENT_ENV = AGENT_MIN_TYPE,
    AGENT_GBIC,
    AGENT_PORT,
    AGENT_FABRIC,
    AGENT_FRU,  /* for fwMailCfg only */
	AGENT_GEPORT,
	AGENT_SECURITY,
    AGENT_MAX_TYPE
} agentType;

typedef enum usage {
	CPU_HANDLER,
	MEM_HANDLER
} USAGE_T;

typedef struct {
	int thThresholdIndex;
	int thStatus;
	char thName[THA_MAX_NAME];
	char thLabel[THA_LABEL_LEN];
	long long thCurVal;
	int thLastEvent;
	int thLastEventVal;
	char thLastEventTime[MAX_TIME_LEN];
	int thLastState;
	int thBehaviorType;
	int thBehaviorInt;
	int thLastSeverity;
	char oui[MAX_OUI_LEN];
} thThresholdEntry;

typedef enum {
    GBIC_MIN_TYPE   = 0,
    GBIC_TEMP       = GBIC_MIN_TYPE,
    GBIC_RX_POWER,
    GBIC_TX_POWER,
    GBIC_CURRENT,
    GBIC_VOLTAGE,
    GBIC_PWR_ON_HRS,
    GBIC_MAX_TYPE
} gbicArea;

typedef enum {
    GEPORT_MIN_TYPE = 0,
    GEPORT_FRAME = GEPORT_MIN_TYPE,
    GEPORT_CRCALIGN,
    GEPORT_SYMBOL,
    GEPORT_IFG,
    GEPORT_TXCFI,
    GEPORT_RXCFI,
    GEPORT_MAX_TYPE
} gePortArea;

#if 0
static inline int fw_fru_cfg_get_all(fw_fruObj_t *fruCfg)
{
}

static inline int fw_fru_cfg_set_all(fw_fruObj_t fruCfg)
{
}
#endif
    #define fw_fru_cfg_get_all(x) 0; printf ("fw_fru_cfg_get_all Successful")
    #define fw_fru_cfg_set_all(x) 0; printf ("fw_fru_cfg_set_all Successful")
    #define thaThLevelSet(x,y,z,k) 0; if (z) printf (" "); printf ("thaThLevelSet Successful")
    #define thaActLevelSet(x,y,z,k) 0; printf ("thaActLevelSet Successful")
static inline int switchStatusGet(fwSwStatus_t *swStat)
{
    strcpy(swStat->statusLastLog, "switch marginal because stuff went down");
    return 0;
}

static inline int fwSwitchHealthReportGet(fwSwitchHealthReport_t *pRpt,
int size)
{
    if (NULL == pRpt) {
        return thNullPtr;
    }

    if (size < (int)(sizeof (fwSwitchHealthReport_t) + sizeof (portInfo_t))) {
        return thInvalidSize;
    }

    strcpy(pRpt->switchName, "sw0");
    strcpy(pRpt->ipAddress, "1.2.3.4");
    pRpt->date = 0;
    pRpt->healthState = FW_SWITCH_MARGINAL;
    pRpt->numHWObjects = 6;
    pRpt->numPorts = 0;
    pRpt->hwObj[0].hwType = SWSTATUS_POWER_SUPPLIES;
    pRpt->hwObj[0].state = FW_SWITCH_HEALTHY;
    pRpt->hwObj[0].numBad = 0;
    pRpt->hwObj[0].numAbsent = 0;
    pRpt->hwObj[1].hwType = SWSTATUS_TEMPERATURES;
    pRpt->hwObj[1].state = FW_SWITCH_HEALTHY;
    pRpt->hwObj[1].numBad = 0;
    pRpt->hwObj[1].numAbsent = 0;
    pRpt->hwObj[2].hwType = SWSTATUS_FANS;
    pRpt->hwObj[2].state = FW_SWITCH_MARGINAL;
    pRpt->hwObj[2].numBad = 0;
    pRpt->hwObj[2].numAbsent = 0;
    pRpt->hwObj[3].hwType = SWSTATUS_WWN;
    pRpt->hwObj[3].state = FW_SWITCH_HEALTHY;
    pRpt->hwObj[3].numBad = 0;
    pRpt->hwObj[3].numAbsent = 0;
    pRpt->hwObj[4].hwType = SWSTATUS_FLASH;
    pRpt->hwObj[4].state = FW_SWITCH_HEALTHY;
    pRpt->hwObj[4].numBad = 0;
    pRpt->hwObj[4].numAbsent = 0;
    pRpt->hwObj[5].hwType = SWSTATUS_FAULTY_PORTS;
    pRpt->hwObj[5].state = FW_SWITCH_MARGINAL;
    pRpt->hwObj[5].numBad = 0;
    pRpt->hwObj[5].numAbsent = 0;
#if 0
    pRpt->numPorts = 14;
    pRpt->portHealth[0].area = 1;
    strcpy(pRpt->portHealth[0].portName, "portname");
    pRpt->portHealth[0].healthState = FW_PORT_HEALTHY;
    pRpt->portHealth[1].area = 2;
    strcpy(pRpt->portHealth[1].portName, "portname");
    pRpt->portHealth[1].healthState = FW_PORT_MARGINAL;
    pRpt->portHealth[2].area = 3;
    strcpy(pRpt->portHealth[2].portName, "portname");
    pRpt->portHealth[2].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[3].area = 4;
    strcpy(pRpt->portHealth[3].portName, "portname");
    pRpt->portHealth[3].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[4].area = 5;
    strcpy(pRpt->portHealth[4].portName, "portname");
    pRpt->portHealth[4].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[5].area = 6;
    strcpy(pRpt->portHealth[5].portName, "portname");
    pRpt->portHealth[5].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[6].area = 7;
    strcpy(pRpt->portHealth[6].portName, "portname");
    pRpt->portHealth[6].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[7].area = 8;
    strcpy(pRpt->portHealth[7].portName, "portname");
    pRpt->portHealth[7].healthState = FW_PORT_HEALTHY;
    pRpt->portHealth[8].area = 9;
    strcpy(pRpt->portHealth[8].portName, "portname");
    pRpt->portHealth[8].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[9].area = 10;
    strcpy(pRpt->portHealth[9].portName, "portname");
    pRpt->portHealth[9].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[10].area = 11;
    strcpy(pRpt->portHealth[10].portName, "portname");
    pRpt->portHealth[10].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[11].area = 12;
    strcpy(pRpt->portHealth[11].portName, "portname");
    pRpt->portHealth[11].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[12].area = 13;
    strcpy(pRpt->portHealth[12].portName, "portname");
    pRpt->portHealth[12].healthState = FW_PORT_FAULTY;
    pRpt->portHealth[13].area = 14;
    strcpy(pRpt->portHealth[13].portName, "portname");
    pRpt->portHealth[13].healthState = FW_PORT_FAULTY;
#endif

    return 0;
}

static inline const char *fwGetSwitchStName(int health)
{
    switch (health) {
    case FW_SWITCH_HEALTHY:
        return ("HEALTHY");
    case FW_SWITCH_MARGINAL:
        return ("MARGINAL");
    case FW_SWITCH_DOWN:
        return ("DOWN");
    default:
        return ("UNKNOWN");
    }
}

enum CONFIG_UPDATE_ENUM {
	DONT_SAVE = 0,

	/*
	 * APPLY_AND_OR_SAVE must be 1 because we are using TRUE
	 * in the code and we dont want to change the existing code
	 */

	APPLY_AND_OR_SAVE = 1,

	SAVE_ONLY,
	APPLY_ONLY
};

/*
 * Security Area Enum
 */
typedef enum {
	SECURITY_MIN_TYPE = 0,
	SECURITY_TELNET = SECURITY_MIN_TYPE,
	SECURITY_HTTP,
	SECURITY_API,
	SECURITY_RSNMP,
	SECURITY_WSNMP,
	SECURITY_SES,
	SECURITY_MS,
	SECURITY_SERIAL,
	SECURITY_PANEL,
	SECURITY_SCC,
	SECURITY_DCC,
	SECURITY_LOGIN,
	SECURITY_INVALID_TS,
	SECURITY_INVALID_SIGN,
	SECURITY_INVALID_CERT,
	SECURITY_SLAP_FAIL,
	SECURITY_SLAP_BAD_PKT,
	SECURITY_TS_OUT_SYNC,
	SECURITY_NO_FCS,
	SECURITY_INCOMP_DB,
	SECURITY_ILLEGAL_CMD,
	SECURITY_MAX_TYPE
} securityArea;

#define THA_MAX_UNIT    32

typedef struct {
	int thClassAreaIndex;
	int thWriteThVals;
	char thDefaultUnit[THA_MAX_UNIT];
	int thDefaultTimebase;
	int thDefaultLow;
	int thDefaultHigh;
	int thDefaultBufSize;
	char thCustUnit[THA_MAX_UNIT];
	int thCustTimebase;
	int thCustLow;
	int thCustHigh;
	int thCustBufSize;
	int thThLevel;
	int thWriteActVals;

	int thDefaultChangedActs;
	int thDefaultExceededActs;
	int thDefaultBelowActs;
	int thDefaultAboveActs;
	int thDefaultInBetweenActs;
	int thDefaultLowBufCrossed;
	int thDefaultUpperBufCrossed;

	int thCustChangedActs;
	int thCustExceededActs;
	int thCustBelowActs;
	int thCustAboveActs;
	int thCustInBetweenActs;
	int thCustLowBufCrossed;
	int thCustUpperBufCrossed;

	int thValidActs;
	int thActLevel;
	char areaName[THA_MAX_NAME];
	int config_status;
	int config_act_status;
	char oui[MAX_OUI_LEN];
} thClassAreaEntry;

/* Time bases */

#define TH_TMBASE_NONE		0
#define TH_TMBASE_SEC		1
#define TH_TMBASE_MIN		2
#define TH_TMBASE_HOUR		3
#define TH_TMBASE_DAY		4
#define CHASSIS_FLAG 1
#define SWITCH_FLAG  0
#define ACT_PORTBLOCK       0x00000020
static inline const char *fwGetHWObjectName(int hwObj)
{
    switch (hwObj) {
    case SWSTATUS_POWER_SUPPLIES:
        return ("Power supplies monitor");
    case SWSTATUS_TEMPERATURES:
        return ("Temperatures monitor");
    case SWSTATUS_FANS:
        return ("Fans monitor");
    case SWSTATUS_WWN:
        return ("WWN servers monitor");
    case SWSTATUS_CP:
        return ("Standby CP monitor");
    case SWSTATUS_BLADE:
        return ("Blades monitor");
    case SWSTATUS_CORE_BLADE:
        return ("Core Blades monitor");
    case SWSTATUS_FLASH:
        return ("Flash monitor");
    case SWSTATUS_MARGINAL_PORTS:
        return ("Marginal ports monitor");
    case SWSTATUS_FAULTY_PORTS:
        return ("Faulty ports monitor");
    case SWSTATUS_MISSING_GBICS:
        return ("Missing SFPs monitor");
    default:
        return ("Unknown");
    }
}

static inline const char *fwGetPortStName(int health)
{
    switch (health) {
    case FW_PORT_HEALTHY:
        return ("HEALTHY");
    case FW_PORT_MARGINAL:
        return ("MARGINAL");
    case FW_PORT_FAULTY:
        return ("FAULTY");
    case FW_PORT_OFFLINE:
        return ("OFFLINE");
    default:
        return ("UNKNOWN");
    }
}

static inline const char *getPortStNameFormat(int health)
{
    switch (health) {
    case FW_PORT_HEALTHY:
        return ("HEALTHY ");
    case FW_PORT_MARGINAL:
        return ("MARGINAL");
    case FW_PORT_FAULTY:
        return ("FAULTY  ");
    case FW_PORT_OFFLINE:
        return ("OFFLINE ");
    default:
        return ("UNKNOWN ");
    }
}

static inline int fwMailDisable(int agent)
{
    return 0;
}

static inline int fwCpuOrMemUsageHandler(struct pcbExtInterface *cfg,  OPS_T, USAGE_T)
{
	return (0);
}

static inline int fwMailEnable(int agent)
{
    return 0;
}

static inline int fwSetMailAddr(int agent, char *addr, char *errStr)
{
    return 0;
}
static inline int thaClassAreaStructGet(int agent, int area1, thClassAreaEntry *entry)
{
	return 0;
}

static inline int thaActFieldSet(int agent, int area1, thEventType event, int acts) {
	return 0;
}
static inline int thaThresholdStructGet(int agent, int area, int snmpIdx,
		     thThresholdEntry *entry) {
	return 0;
}

static inline int thaInterfaceThresholdStructGet(int agent, int area, int slot,
			int cage, int subport, thThresholdEntry *entry) {
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif // __FWDPUB_H__

