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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Author : Vigneshkumar Bagavathsingh
 *   Description:
 *	   This header file contains license related stub routines
 *	   and structures
 */

#ifndef __LICENSE_H__
#define __LICENSE_H__

using namespace WaveNs;
using namespace DcmNs;

#include "fos_swbd.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_LICENSEDATA_SIZE 512
#define EXPIRYSTR_SIZE 64
#define MAX_FEATURENAMESTR_SIZE 64
#define MAX_CAPACITYSTR_SIZE 32
#define MAX_CONSUMEDSTR_SIZE 64
#define MAX_VALIDSTR_SIZE 20
#define ENH_PODSTR_LEN_MAX 4096
#define LICENSE_FEATURES_MAX 96

#define ENH_LICENSE_KEY_ADDED       1
#define ENH_LICENSE_KEY_REMOVED     1
#define ENH_LICENSE_NOT_ENABLED     2
#define ENH_LICENSE_DB_FAILED       3
#define ENH_LICENSE_ADD_FAILED      4
#define ENH_LICENSE_REMOVE_FAILED   4
#define POD1_LICENSE                5
#define POD2_LICENSE                5
#define BASE_FCOE_LICENSE           6
#define VCS_FABRIC_LICENSE          7
#define LAYER_3_LICENSE             8
#define ADVANCED_SERVICES_LICENSE   9
#define TEST_LICENSE                10
#define PORT_10G_UPGRADE_LICENSE    11
#define PORT_40G_UPGRADE_LICENSE    12

#define INTERFACE "tengigabitethernet"
#define TRUE                        1
#define FALSE                       0
#define	ENH_LICENSE_LEN_MAX	 		256

typedef struct enh_license_info {
    char    license[MAX_LICENSEDATA_SIZE];
    char    expirydatestr[EXPIRYSTR_SIZE];
    char    featureName[MAX_FEATURENAMESTR_SIZE];
    char    capacity[MAX_CAPACITYSTR_SIZE];
    char    consumed[MAX_CONSUMEDSTR_SIZE];
     char   validState[MAX_VALIDSTR_SIZE];
} enh_license_entry_t;

typedef struct licenseid_info {
	char	license_id[24];
} licenseid_entry_t;

typedef struct pod_info {
    char podstr[ENH_PODSTR_LEN_MAX];
    int max_DPOD_Ports;
    int num_DPOD_Licenses;
    int max_DPOD_provisioned;
    int reserved_ports;
    int released_ports;
    int max_40G_DPOD_Ports;
    int num_40G_DPOD_Licenses;
    int max_40G_DPOD_provisioned;
    int reserved_40G_ports;
    int released_40G_ports;
} pod_entry_t;

typedef struct lic_ch_stat {
	int status;
	int num_change;
	int sw_en_req;
	int sw_reboot_req;
} lic_ch_stat_t;

typedef uint32_t license_feature_t;
typedef uint32_t license_capacity_t;    /* Capacity license attribute type */

#define ENH_L_P(p,i) ((enh_license_entry_t*)( p + i ))
#define switchOffline() 			1

#define POD_ERR_LAST				-1
#define POD_ERR_NOT_SUPPORTED		-2
#define POD_ERR_INACTIVE			-3
#define POD_ERR_SYSMOD_INIT			-4
#define POD_ERR_FABOS_INIT			-5
#define POD_ERR_GET_POD_DATA		-6
#define POD_ERR_WRONG_PORT_NUM		-7
#define POD_ERR_PORT_ONLINE			-8
#define POD_ERR_RELEASE_POD_PORT	-9
#define POD_ERR_ALREADY_LICENSED	-10
#define POD_ERR_NO_LICENSE			-11
#define POD_ERR_RESERVE_POD_PORT	-12
#define POD_ERR_INVALID_ARG			-13
#define DPOD_ERR_NOT_SUPPORTED		-14
#define POD_ERR_IS_FC_PORT_LICENSED	-15
#define POD_ERR_IS_GE_PORT_LICENSED	-16
#define POD_ERR_IS_40GE_PORT_LICENSED -17
#define POD_ERR_SET_POD_DATA        -18
#define POD_ERR_READ_MAX_PORT       -19

#define	LICENSE_DB_OK				0	/* No status to report */
#define	LICENSE_DB_EXISTS			1	/* Record already exists */
#define	LICENSE_DB_NOENT			2	/* No such record exists */
#define	LICENSE_DB_FULL				3	/* No space available */
#define	LICENSE_DB_EMPTY			4	/* No records exist */
#define	LICENSE_DB_INVALID			5	/* Invalid record */
#define	LICENSE_DB_IOERR			6	/* I/O error occurred */
#define	LICENSE_DB_LRR				7	/* Last record reached */
#define LICENSE_DB_CAP_FULL			8	/* License Capacity/Count limit is reached */
#define LICENSE_DB_DOWNGRADE        9	/* Downgrade the record */
#define LICENSE_DB_INVALID_DATA     10	/* Invalid data */
#define LICENSE_NO_SUPPORT          11	/* License not supported on the platform */
#define LICENSE_DB_SLOTS_STILL_CFG  12	/* Slots still configured on slot-based license */
#define LICENSE_DB_EXPIRED          13	/* License Expired */
#define LICENSE_DB_TIME_TAMPER      14	/* Time based licenses are inconsistent with date */
#define LICENSE_DB_NO_TRIAL         15	/* Regular license already installed */
#define LICENSE_DB_YES_TRIAL        16	/* Trial license already installed */
#define LICENSE_DB_FTR_VER_EXISTS   17	/* License feature/version already exists */
#define LICENSE_DB_FMS_STILL_CFG    18	/* FMS is still enabled */
#define LICENSE_DB_START_DATE       19	/* License start date later than current system date */
#define LICENSE_DB_ERR_LAST_ENTRY   20	/* Last entry, must be incremented when new entry added */

static char	default_err_str[] = "Unknown error.\n";
static unsigned char default_license_str[] = "Unknown License.\n";
static struct err_handle {
	int error_code;
	char	*error_msg;
} pod_error_table[] =
{
	{ POD_ERR_NOT_SUPPORTED,	(char *) "The Ports-on-Demand (POD) feature is "
								"not supported on this product." },
	{ POD_ERR_INACTIVE, (char *) "The Ports-on-Demand (POD) feature is not active." },
	{ POD_ERR_SYSMOD_INIT,  (char *) "Failed to open sysMod." },
	{ POD_ERR_GET_POD_DATA, (char *) "Failed to get POD data." },
	{ POD_ERR_WRONG_PORT_NUM, (char *) "Invalid port number argument." },
	{ POD_ERR_PORT_ONLINE,  (char *) "Port should be Offline to "
								"change POD assignment." },
	{ POD_ERR_RELEASE_POD_PORT, (char *) "Failed to release POD port." },
	{ POD_ERR_ALREADY_LICENSED, (char *) "Port is already assigned a POD license." },
	{ POD_ERR_NO_LICENSE,   (char *) "No more license available to assign to port." },
	{ POD_ERR_RESERVE_POD_PORT, (char *) "Failed to reserve POD port." },
	{ POD_ERR_INVALID_ARG, (char *) "Invalid argument passed." },
	{ DPOD_ERR_NOT_SUPPORTED,
						(char *) "This command is not supported on this product." },
	{ POD_ERR_IS_FC_PORT_LICENSED,  (char *) "Failed to get fc port licensing." },
	{ POD_ERR_IS_GE_PORT_LICENSED,  (char *) "Failed to get ge port licensing." },
	{ POD_ERR_SET_POD_DATA, (char *) "failed to write POD data to the fabsys kernel." },
	{ POD_ERR_READ_MAX_PORT, (char *) "failed to read maximum available POD ports." },
	{ -1,   NULL }  /* mark the end */
};

static inline int
licenseGetFeatures(const char *key, license_feature_t *licFeature) {
	if (key != NULL) {
		*licFeature = BASE_FCOE_LICENSE;
	}
	return (1);
}

static inline license_capacity_t licenseGetCapacity(int feature) {
	return (8);
}

/* Handle POD license assignment */
static inline int
enh_LicensePod(const char *option, const char *interface,
			   const char *swslotport) {

	int opt[3];
	int i = 0;
	char val[11];
	char delims[] = "/";
	char *result = NULL;
	strcpy(val, swslotport);
	result = (char *)strtok((char *)swslotport, delims);
	/*
	 * opt[0] - switchId info
	 * opt[1] - Slot Number must be 0
	 * opt[2] - Port Number
	 */
	while (result != NULL) {
		opt[i] = atoi(result);
		result = (char *)strtok(NULL, delims);
		i++;
	}

	/* Slot must be 0 and port number >= 0 */
	if ((opt[1] != 0) || (opt[2] < 0)) {
		return (POD_ERR_INVALID_ARG);
	}

	if (((strcmp(option, "release") != 0) ||
		 (strcmp(option, "reserve") != 0)) &&
		 (strcmp(interface, "TengigabitEthernet") != 0)) {
		return (POD_ERR_INVALID_ARG);
	}

	/* Returns 0 on success */
	return (0);
}

static inline char *
pod_err_msg(int err_code)
{
	int i;

	for (i = 0; pod_error_table[i].error_code != -1; i++) {
		if (err_code == pod_error_table[i].error_code) {
			return (pod_error_table[i].error_msg);
		}
	}
	return (default_err_str);
}

static inline int
LicenseIdShow(licenseid_entry_t *licenseid_entry)
{
	if (licenseid_entry != NULL) {
		memset(licenseid_entry, 0, sizeof (licenseid_entry_t));
		strcpy((char *)licenseid_entry, "10:00:05:1e:55:66:77:FF");
	} else {
		return (-1);
	}
	return (0);
}

static inline int sRMS_license_show(int flag,
									enh_license_entry_t **license_data,
									int *licenseCount)
{
#define PRINTF_DISABLED 0
#define MAX_SENTINAL_LICENSES 3

	enh_license_entry_t *licenseEntry;
	int i=0;
	if (flag == PRINTF_DISABLED) {
		licenseEntry = (enh_license_entry_t *)
						malloc(sizeof (enh_license_entry_t)
									* MAX_SENTINAL_LICENSES);
		memset(licenseEntry, 0,
				(sizeof (enh_license_entry_t) * MAX_SENTINAL_LICENSES));
		for (i=0; i<MAX_SENTINAL_LICENSES; i++) {
			strcpy(licenseEntry[i].license, "SzzzydRyddV000z9\n");
			sprintf(licenseEntry[i].expirydatestr, "License expires 02/02/200%d\n", i);
			sprintf(licenseEntry[i].featureName, "PORTS_ON_DEMAND_1\n");
            sprintf(licenseEntry[i].validState, "License is valid\n");
		}
	}
	*license_data = licenseEntry;
	*licenseCount = 0;
	return i;
}

static inline int
licenseCheck (int feature)
{
    return (1);
}

/* Displays the pod licenes info */ 
static inline int
enh_LicensePodShow(pod_entry_t *podlicense, unsigned int rbridge_id)
{
	sprintf(podlicense->podstr, "32 ports are available in this switch\n");
	sprintf(podlicense->podstr, "%s 2 POD licenses are installed\n",
			podlicense->podstr);
	sprintf(podlicense->podstr, "%s     Dynamic POD method is in use\n",
		podlicense->podstr);
	podlicense->max_DPOD_Ports =  32;
	podlicense->num_DPOD_Licenses =  2;
	podlicense->max_DPOD_provisioned = 32;
	podlicense->reserved_ports =  20;
	podlicense->released_ports = 12;

	return (0);
}

/* Handle DPOD license assignment */
static inline int
enh_LicenseDPod(const char *option, const char *slotport) {
    if (strcmp(option, "release") == 0) {
            return (0);
    } else {	//in case of reserve assign single
            return (2);
    }
}

/* Store the pod_t information in the fabsys kernel */
static inline int
set_pod_data(void *pinfo_p) {
	return (0);
}

static inline int
get_pod_data(void *pinfo_p) {
	return (0);
}
/* Get the switch platform number */
static inline int 
get_pod_SWBD(void) {
	return (SWBD_ELARA2E); // Elara platform
}

static inline int 
licenseAdd(const char *license, lic_ch_stat_t *stat) {
	return (1);
}

static inline int 
licenseRemove(const char *license, lic_ch_stat_t *stat) {
	return (1);
}

static inline int 
licenseShow (int flag, enh_license_entry_t *license_data, int *licenseCount) {
	*licenseCount = 0 ;
	return (LICENSE_DB_OK);
}


/* Get the max available pod ports on the switch*/
static inline int 
get_max_pod_ports(void) {
	return (24); // Elara Max Ports
}

/* does this platform support Dynamic POD? */
static inline int isDPODplatform(int swModel)
{
    return (SWBD_ELARA2E == swModel || SWBD_ELARA2F == swModel || SWBD_CALLISTOE == swModel ||
            SWBD_CALLISTOF == swModel || SWBD_CASTOR == swModel || SWBD_CASTORT == swModel);
}

/* does this platform support Enhanced Dynamic POD? ie multiple POD pools */
static inline int isEPODplatform(int swModel)
{
    return (SWBD_CASTOR == swModel || SWBD_CASTORT == swModel);
}

static inline int licenseInstalled(const char * feature) 
{
	return (1);
}

static inline unsigned char * licenseNameByFeatureBit(int feature) {
	return (default_license_str);
}

#ifdef __cplusplus
}
#endif
#endif // LICENSE_H
