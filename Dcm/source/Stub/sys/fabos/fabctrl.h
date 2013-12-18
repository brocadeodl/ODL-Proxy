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

#ifndef __SYS_FABCTRL_H__
#define __SYS_FABCTRL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <fos_swbd.h>   /* BLADE_ID_INITIALIZER */

/* Addressable object types */
typedef enum {
    SYS_HW_UNKNOWN  = 0x0,
    SYS_HW_SLOT = 0x1,
    SYS_HW_PS   = 0x2,
    SYS_HW_BLOWER   = 0x3,
    SYS_HW_WWN  = 0x4,
    SYS_HW_PIC  = 0x5,
    SYS_HW_MEDIA    = 0x6,
    SYS_HW_CHASSIS  = 0x7,
    SYS_HW_MMI      = 0x8,
    /* always add new type before this line!! */
    SYS_HW_MAX

} sysCtrlObjType_e;

typedef enum {
    SYS_BLADE_UNKNOWN   = 0x00,
    SYS_BLADE_SW        = 0x10,
    SYS_BLADE_CP        = 0x20,
    SYS_BLADE_GP        = 0x30,
    SYS_BLADE_WAN       = 0x40,
    SYS_BLADE_AP        = 0x50,
    SYS_BLADE_CPCTRL    = 0x60,
    SYS_BLADE_CORE      = 0x70

} sysCtrlCatType_e;

typedef enum {
    SYS_SLOT_UNKNOWN    = 0x00,
    SYS_SLOT_LC         = 0x10, /* SW */
    SYS_SLOT_FMM        = 0x20, /* CP */
    SYS_SLOT_MM         = 0x60, /* CPCTRL */
    SYS_SLOT_SFM        = 0x70  /* CORE */
} sysCtrlSlotType_e;

#define UNKNOWN_BLADE_ID 0

typedef enum {
    BLADE_ID_INITIALIZER
} bladeID_t; /* the definition used to be in fabsys.h */

typedef enum {
    SYS_UNIT_UNKNOWN    = 0x00000000,   /* unknown */
    SYS_UNIT_OUT        = 0x00000001,   /* not physical IN */
    SYS_UNIT_IN         = 0x00000002,   /* physical IN and no DC */
    SYS_UNIT_READY      = 0x00000004,   /* physical IN and DC is GOOD */
    SYS_UNIT_UP         = 0x00000008,   /* READY state and POST1 passed */
    SYS_UNIT_ON         = 0x00000010,   /* UP state and POST2 passed */
    SYS_UNIT_OFF        = 0x00000020,   /* ON state and diabled */
    SYS_UNIT_FAULT      = 0x00000040,   /* fails on POST1 or POST2 */
    SYS_CORES_DOWN      = 0x00000080,   /* No core switching asics available */
    SYS_SEM_LOCK        = 0x00000100,   /* Slot semaphore locked by spinfab */
    SYS_SEM_ULOCK       = 0x00000200    /* Slot semaphore unlocked by spinfab */
} sysCtrlSS_e;

typedef struct sysCtrlHwRecord {
    u_long      type    : 8;    /* hardware type */
    u_long      id  : 16;   /* hardware id */
    u_long      version : 8;    /* hardware version */

} sysCtrlHwRecord_t;


/* unit data holder - hardware/software */
typedef struct sysCtrlData {
    sysCtrlObjType_e    object;     /* object type */
    int                 unitNumber; /* hardware unit number */
    sysCtrlHwRecord_t   record;     /* hardware record data */
    sysCtrlSS_e         state;      /* power/slot states */
    u_int               reason;     /* why posting/setting */
    int                 ret_val;    /* ret value of state change operation */
    int                 blade_mode; /* added only for use by DCMD */
} sysCtrlData_t;

/* state data structure for communications */
typedef struct sysCtrlState {
    int             total;  /* total unit sents */
    sysCtrlData_t       data[1]; /* hardware data */

} sysCtrlState_t;


#ifdef __cplusplus
}
#endif

#endif
