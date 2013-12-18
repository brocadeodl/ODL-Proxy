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

#ifndef __HIL_H__
#define __HIL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define HIL_BRCD_PN_LEN 14
#define HIL_BRCD_SN_LEN 20
#define HIL_SUPP_ID_LEN 11
#define HIL_SUPP_PN_LEN 21
#define HIL_SUPP_SN_LEN 21
#define HIL_SUPP_REV_LEN 5

// actual value in hil.h is the commented one below.
// #define HIL_BRCD_SN_LEN 12

/*
 * FRU track configuration
 * These definitions seem to be used by EM to determine what parts of the
 * fru header is valid for display.  Also used by SNMP.
 *
 * These definitions were originally in em_if.h (see the comments with each
 * definition that indicate the original name), but have been moved here
 * since these are board specific.
 *
 * The change to move these here is in the Voyager (5.1) release.
 */
/* #define EM_NO_CONSUMEPOWER   0x01    No power consumption factors */
#define FRU_TRACK_CFG_NO_CONSUMEPOWER       0x01    /* No power consumption factors */
/* #define EM_NO_BRCD_PN_SN 0x02    No Brocade Part/Serial Number */
#define FRU_TRACK_CFG_NO_BRCD_PN_SN         0x02    /* No Brocade Part/Serial Number */
/* #define EM_NO_MANUFACTURE_DATE   0x04    No manufacturing date */
#define FRU_TRACK_CFG_NO_MANUFACTURE_DATE   0x04    /* No manufacturing date */
/* #define EM_NO_TIME_ALIVE 0x08    No permanent TIME ALIVE storage */
#define FRU_TRACK_CFG_NO_TIME_ALIVE         0x08    /* No permanent TIME ALIVE storage */
/* #define EM_NO_SUPPLIER       0x10    No storage for supplier data */
#define FRU_TRACK_CFG_NO_SUPPLIER           0x10    /* No storage for supplier data */
/* #define EM_SUPPLIER_PERMANENT    0x20    Supplier info is permanent data */
#define FRU_TRACK_CFG_SUPPLIER_PERMANENT    0x20    /* Supplier info is permanent data */
/* #define EM_ASSEMBLY_UNIT 0x40    Part of an Assembly */
#define FRU_TRACK_CFG_ASSEMBLY_UNIT			0x40    /* Part of an Assembly */
/* This is a replacement for a HIL only shortcut */
#define FRU_TRACK_CFG_NO_FRU    FRU_TRACK_CFG_NO_CONSUMEPOWER | \
                    FRU_TRACK_CFG_NO_BRCD_PN_SN | \
                    FRU_TRACK_CFG_NO_MANUFACTURE_DATE | \
                    FRU_TRACK_CFG_NO_TIME_ALIVE | FRU_TRACK_CFG_NO_SUPPLIER

/*
 * FRU state
 * Used as part of em_if.h
 */
typedef enum {
	FRU_STATE_UNKNOWN = -1, /* unknown state */
	FRU_ABSENT = 0x1,   /* FRU is absent */
	FRU_INSERTED = 0x2, /* FRU is inserted and "powered off" */
	FRU_READY = 0x4,    /* FRU is inserted and transitioning */
	FRU_UP = 0x8,       /* FRU is READY and past POST 1 */
	FRU_ON = 0x10,      /* FRU is UP and past POST2 */
    FRU_OFF = 0x20,     /* FRU is ON and disabled */
    FRU_FAULTY = 0x40   /* FRU is faulty and can be powered on or off */
} fruState_e;

/*
 * Enumeration for sensor status
 * Used as part of em_if.h
 */
typedef enum {
    HS_UNKNOWN = 0,
    HS_NOMINAL = 0x1,
    HS_FAULTY = 0x2,
    HS_BELOW = 0x4,
    HS_ABOVE = 0x8,
    HS_ABSENT = 0x10,
    HS_NOMINAL_UNKNOWN = 0x21, /* OK, but don't know precise value */
    HS_FAULTY_UNKNOWN = 0x22, /* faulty, but don't know precise value */

    HS_MAX_STATUS /* please place any new entries before this entry */
} hilSenStatus_e;

/*
 * Object types of the devices.
 * Use lower 16 bits.
 */
typedef enum {
    HIL_OBJ_CHASSIS     = 0x1,
    HIL_OBJ_CPBLADE     = 0x2,
    HIL_OBJ_GPBLADE     = 0x3,
    HIL_OBJ_SWBLADE     = 0x4,
    HIL_OBJ_WANBLADE    = 0x5,
    HIL_OBJ_BLOWER      = 0x6,
    HIL_OBJ_POWER       = 0x7,
    HIL_OBJ_WWN         = 0x8,
    HIL_OBJ_APBLADE     = 0x9,
    HIL_OBJ_CPCTRLBLADE = 0xa,
    HIL_OBJ_COREBLADE   = 0xb,
    HIL_OBJ_PIC         = 0x10,
    HIL_OBJ_POL         = 0xfe,

/*
 * changed for Tsunami. In EM ODB tree sensor will belong to object, rather
 * than subObject. SFP or XFP are possible leaves in the ODB tree now.
 * use bit 24-31.
 */
    HIL_OBJ_SFP         = 0x1000000,
    HIL_OBJ_XFP         = 0x2000000,
    HIL_OBJ_MEDIA_UNKNOWN   = 0x7f000000,
    HIL_OBJ_UNKNOWN     = 0xff
} hil_obj_kind_t;

#define SWNAME_SIZE     32
typedef char    swname_t[SWNAME_SIZE];
static inline int hilGetSwName(sysModHandle_t *handle, int unitToken, int tokenNum,
        swname_t *switchName)
{
	strncpy((char *const) switchName, "X86-SW0", SWNAME_SIZE);
    return (0);
}

#ifdef __cplusplus
}
#endif

#endif
