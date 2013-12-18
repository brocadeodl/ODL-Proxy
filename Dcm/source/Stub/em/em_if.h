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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef __EM_IF_H__
#define __EM_IF_H__

#include "Framework/Utils/FrameworkToolKit.h"
#include "Stub/sysmod/syscomm.h"
#include "Stub/hil/hil.h"
#include "Stub/sys/fabos/fabctrl.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * EM IPC ERROR codes
 */
#define EMNOTSUPPORTED		-101	/* Not supported in this platform */
#define EMNOTREADY		-102	/* EM has not built ODB */
#define EMINVALIDSWITCH		-103	/* Caller from wrong switch */
#define EMINVALIDNAME		-104	/* Invalid switch name */
#define EMINVALIDSLOT		-105	/* Invalid slot for this platform */
#define EMSLOTABSENT		-106	/* Slot request is vacant */
#define EMALREADY		-107	/* Already in the state */
#define EMOUTOFORDER		-108	/* Sequence out of order */
#define EMDISABLENOTOK		-109	/* Not okay to disable slot */
#define EMNOPOWER		-110	/* Not enough power */
#define EMHILERROR		-111	/* HIL error occured */
#define EMSLAVE			-112	/* Can not give data, because slave */
#define EMNOTFOUND		-113	/* Could not find addr in cache */
#define EMNOMEM			-114	/* Could not alloc enough memory */
#define EMINVALIDIP		-115    /* Invalid IP address */
#define EMINVALIDMASK		-116	/* Invalid IP mask address */
#define EMFAULTY		-117	/* Unit is faulty */
#define EMDUPLICATE		-118	/* Duplicate IP address */
#define EMNOSAVE		-119	/* Unable to save data */
#define EMNOSYSCTRL		-120	/* I2C bus timed out */
#define EMDEFAULT		-121	/* Can't obtain value(s)/default used */
#define EMDIAGRUNNING		-122	/* Can't because diag is running */
#define EMPROCESSHOTPLUG	-123	/* Processing a hot-plug operation */
#define EMNOSWITCH		-124	/* Switch was shutdown or is down */
#define EMSWITCHBUSY		-125	/* Could not get serialization lock from */
								/* FICON-CUP daemon.					 */
#define EMIPCERROR		-126	/* IPC to ficud failed.            */
#define EM_FICU_UNKNOWN		-127	/* Unknown error from ficud.   */
#define EM_OBJECT_NOT_FOUND	(-128)
					 /* No object matches oid_t */


/*
 * EM string length (including NULL)
 */
#define EM_BRCD_PN_LEN      (HIL_BRCD_PN_LEN+1) /* part number + \0 */
#define EM_BRCD_SN_LEN      (HIL_BRCD_SN_LEN+1) /* serial number + \0 */
#define EM_SUPP_ID_LEN      HIL_SUPP_ID_LEN     /* id + \0  */
#define EM_SUPP_PN_LEN      HIL_SUPP_PN_LEN     /* part number + \0 */
#define EM_SUPP_SN_LEN      HIL_SUPP_SN_LEN     /* serial number + \0 */
#define EM_SUPP_REV_LEN     HIL_SUPP_REV_LEN    /* revision number + \0 */

#define EM_CHASSIS_SN_LEN   HIL_BRCD_SN_LEN /* chassis serial number */


#define EM_CHASSIS_SN_LEN   HIL_BRCD_SN_LEN /* chassis serial number */

/*
 * EM MISC CODES
 */
#define EM_EMPTY            0
#define EM_UNKNOWN          1
#define EM_VALID            2
#define EM_HISTORY_ALL      3
#define EM_HISTORY_LAST     4

/*
 * to parse the swFruId within the emInventory_t.
 */
#define GLOBALSYSMOD_VER_MASK(x)    ((x) & 0xff)
#define GLOBALSYSMOD_ID_MASK(x)     (((x) >> 8) & 0xffff)
#define GLOBALSYSMOD_TYPE_MASK(x)   (((x) >> 24) & 0xff)

#define SWMODEL_NAME_SIZE 32
typedef char    swmodel_nm_t[SWMODEL_NAME_SIZE];
static inline int em_get_swmodel_name(swmodel_nm_t *swmodel_name)
{
    return (0);
}

typedef struct emChassisSNInfo {
    char    chassisSN[EM_CHASSIS_SN_LEN]; /* Chassis serial number */
} emChassisSNInfo_t;


static inline int em_GetChassisSN(emChassisSNInfo_t *pChassisSN)
{
    sprintf (pChassisSN->chassisSN, "%s:%d",
            (FrameworkToolKit::getThisLocationIpAddress()).c_str(),
             FrameworkToolKit::getThisLocationPort());

    return (0);
}

/* Max # slots in chassis */
#define DCE_MAX_SLOTS               12

typedef struct _mac_addr
{
    unsigned char addr[6];
} mac_addr_t;

typedef struct _mac_pool {
    unsigned int size;
    unsigned int MACaddressMS;
    unsigned int MACaddressLS;
} mac_pool_t;

typedef struct _mac_pool_list  {
    unsigned int  count;
    //mac_pool_t entries[0];
    mac_pool_t *entries;
} mac_pool_list_t;

typedef struct fabSysParam {
    int max_chips_per_system;
    int max_ge_ports_per_anvil;
    int max_lags_per_system;
    int max_ge_ports_per_system;
    unsigned char slot_chip_info[DCE_MAX_SLOTS + 1];
    mac_pool_t mac_pool;
	int max_slots_per_system;
	int max_lcs_per_system;
} fabSysParam_t;



static inline int sysCtrlGetSystemParameters(sysModHandle_t *pHandle, void *pSystemParam)
{
    fabSysParam_t *fabSys = (fabSysParam_t *)pSystemParam;
    fabSys->mac_pool.MACaddressMS = 0x11223344;
    fabSys->mac_pool.MACaddressLS = 0x5566;

    return (0);
}

static inline int sysModScnRegisterWrapper (sysModHandle_t *pHandle, u_int scn_types[][2], void* msg_q)
{
    return (0);
}

typedef unsigned int oid_t;
typedef struct emIndex {
    int start;  /* start value */
    int end;    /* end value */
} emIndex_t;

typedef struct emChassisInfo {
    int         status;     /* valid info */
    sysCtrlObjType_e sysObject; /* SYS_HW_SLOT, PS, BLOWER, WWN */
    int         unitNum;    /* ie swicht 1, fan 1, etc... */
    u_int       objType;    /* OBJ_CPBLADE, etc... */
    fruState_e  fruState;   /* current state of FRU */

    u_char      hdrVersion; /* 1-255 */
    int         pwrCF;      /* power consume factor */
    char        brcdPN[EM_BRCD_PN_LEN]; /* BRCD part number */
    char        brcdSN[EM_BRCD_SN_LEN]; /* BRDCD serial number */
    u_int       swFruId;    /* Software FRU ID, if applicable */
    char        manDay;     /* 1 - 31 */
    char        manMonth;   /* 1 - 12 */
    short       manYear;    /* manufacturing year */
    char        updateDay;  /* 1 - 31 */
    char        updateMonth;    /* 1 - 12 */
    short       updateYear; /* update year */
    int         timeAlive;  /* time the switch is alive */
    int         timeAwake;  /* time the switch is up */
    char        trackCfg;   /* FRU tracking configuration */
    int         i2cErr;     /* Private EM internal state */
    u_long      privState;  /* Private EM internal state */
    char        assyUnit;   /* Assembly unit if applicable */
    char        supplierID[EM_SUPP_ID_LEN]; /* supplier id */
    char        supplierPN[EM_SUPP_PN_LEN]; /* part number */
    char        supplierSN[EM_SUPP_SN_LEN]; /* serial number */
    char        supplierRev[EM_SUPP_REV_LEN];   /* revision number */
    oid_t       oid;        /* System wide OID for this object */
    char        removable;  /* boolean: FALSE (zero) if object */
                            /* cannot be removed. */
} emChassisInfo_t;

static inline int
em_GetChassisObjectCount(int *pCount)
{
    *pCount = 100;  /* arbitrary object count of 100 passed back */
    return (0);
}

static inline int
em_GetChassisInfo(emChassisInfo_t *pChassisInfo, emIndex_t *pEmCount)
{
    int i;
    for (i = pEmCount->start; i < pEmCount->end; i++) {
        pChassisInfo[i].oid = i;
        pChassisInfo[i].status = EM_VALID;
        if (i == 1)
            pChassisInfo[i].sysObject = SYS_HW_WWN;
    }
    return (0);
}

/*
 * Object types of the devices.
 * Use lower 16 bits.
 */
typedef enum {
        OBJ_CHASSIS             = HIL_OBJ_CHASSIS,
        OBJ_CPBLADE             = HIL_OBJ_CPBLADE,
        OBJ_GPBLADE             = HIL_OBJ_GPBLADE,
        OBJ_SWBLADE             = HIL_OBJ_SWBLADE,
        OBJ_WANBLADE    = HIL_OBJ_WANBLADE,
        OBJ_BLOWER              = HIL_OBJ_BLOWER,
        OBJ_POWER               = HIL_OBJ_POWER,
        OBJ_WWN                 = HIL_OBJ_WWN,
        OBJ_APBLADE             = HIL_OBJ_APBLADE,
        OBJ_CPCTRLBLADE = HIL_OBJ_CPCTRLBLADE,
        OBJ_COREBLADE   = HIL_OBJ_COREBLADE,
        OBJ_PIC                 = HIL_OBJ_PIC,
        OBJ_POL                 = HIL_OBJ_POL,

/*
 * changed for Tsunami. In EM ODB tree sensor will belong to object, rather
 * than subObject. SFP or XFP are possible leaves in the ODB tree now.
 * use bit 24-31.
 */
        OBJ_SFP                 = HIL_OBJ_SFP,
        OBJ_XFP                 = HIL_OBJ_XFP,
        OBJ_MEDIA_UNKNOWN       = HIL_OBJ_MEDIA_UNKNOWN,
        OBJ_UNKNOWN             = HIL_OBJ_UNKNOWN
} em_obj_kind_t;

/*
 * New structure to pass sensor information to other applications.
 */
typedef struct emSensor {
    sysCtrlObjType_e object;    /* SLOT, PS, FAN, WWN */
    int     unitNum;            /* Slot based for blades else unit number */
    oid_t oid;                  /* object that has this sensor */
    em_obj_kind_t   emObject;   /* see OBJ_XXX (above) */
    fruState_e emFruState;      /* see FRU_XXX (above) */
    int     i2cErr;             /* Probs accessing FRU */
    u_int   sensorType;         /* type of sensor, OBJ_SEN_TEMP, etc. */
    u_int   sensorSubType;      /* subtype of sensor, TEMP1, VOLT1, etc */
    hilSenStatus_e  sensorStatus;/* status of sensor */
    int     sensorValue;        /* value from sensor, -1 means error */
} emSensor_t;

/*
 * Sensor Type definitions
 */
#ifdef TEMP_SENSOR_TYPE
#undef TEMP_SENSOR_TYPE
#endif
#define TEMP_SENSOR_TYPE    0x10

#ifdef FAN_SENSOR_TYPE
#undef FAN_SENSOR_TYPE
#endif
#define FAN_SENSOR_TYPE     0x20

static inline int em_GetChassisTempSensorCount(int *pCount)
{
    *pCount = 10;
    return (0);
}

static inline int em_GetChassisTempSensor(emSensor_t *pSensor, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        pSensor[i].oid = i;
    }
    return (0);
}

static inline int em_GetChassisFanCount(int *pCount)
{
    *pCount = 10;
    return (0);
}

static inline int em_GetChassisFanRpm(emSensor_t *pSensor, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        pSensor[i].oid = i;
    }
    return (0);
}

static inline int em_SlotPowerOn(int sw, int slot)
{
	printf("******Linecard at slot %d is powered-on*****\n", slot);
	return (0);
}

static inline int em_SlotPowerOff(int sw, int slot)
{
	printf("******Linecard at slot %d is powered-off******\n", slot);
	return (0);
}

static inline int em_get_slot_count(int *slotcount)
{
	printf("*****slot count is 16***********\n");
	return (0);
} 

typedef struct emInventory {
	unsigned int swFruId;
} emInventory_t;

static inline int em_GetOneInventory(sysCtrlObjType_e obj, int unitNumber,
            emInventory_t *pInventory) {
	return (0);
}

#if 0
/* module handle data structure */
typedef struct sysModHandle {
    int     fd;     /* system module device file descriptor */
    int     wwn_fd; /* system module device file descriptor */
} sysModHandle_t;

static sysModHandle_t sysModHandle;

int sysModInit(sysModHandle_t **hdl);
int sysModClose(sysModHandle_t **hdl);
int sysModInit(sysModHandle_t **hdl)
{
    *hdl = &sysModHandle;
    return (0);
}

int sysModClose(sysModHandle_t **hdl)
{
    return (0);
}
#endif 
#ifdef __cplusplus
}
#endif

#endif // __EM_IF_H__
