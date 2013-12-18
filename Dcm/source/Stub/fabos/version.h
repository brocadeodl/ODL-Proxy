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

#ifndef __FABOS_VERSION_H__
#define __FABOS_VERSION_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define PRI_VERSION_FILE "/fabos/share/version"
#define MNT_VERSION_FILE "/mnt/fabos/share/version"

/*
 * firmware type
 */
#define FW_TYPE_FOS     1
#define FW_TYPE_FCIP    2
#define FW_TYPE_SAS     3

/*
 * Version information structure
 */
#define MAX_NUM_VERSION     5
#define FW_VALID            0x1234
#define FW_MAX_NAME         16  /* max size of symbolic name */
#define FW_MAX_VSTR         64  /* max # of bytes of version string */
#define FW_MAX_DATESTR      64  /* max # of bytes of date/time stamp */

#define FW_NAME_FABOS       "base"
#define FW_NAME_KERNEL      "kernel"
#define FW_NAME_PROM        "prom"

typedef struct  {
    char    fw_name[FW_MAX_NAME];   /* firmware symbolic type name */
    int     fw_valid;               /* The structure is valid? */
    int     fw_type;                /* CFOS/BFOS/APP */
    char    fw_major;               /* Major number */
    char    fw_minor;               /* Minor number */
    char    fw_patch;               /* patch number */
    char    fw_state;               /* state */
    char    fw_verstr[FW_MAX_VSTR]; /* version string */
    char    fw_bdate[FW_MAX_DATESTR]; /* build date */
    char    fw_idate[FW_MAX_DATESTR]; /* install date */
} fwversion_t;

typedef fwversion_t version_array_t[MAX_NUM_VERSION]; /* per partition */

static inline int version_get(const char *filename, char *fwname, fwversion_t *fwver)
{
    memset(fwver, 0, sizeof (fwversion_t));

    strcpy(fwver->fw_name, "base");
    fwver->fw_valid = FW_VALID;
    fwver->fw_type = FW_TYPE_FOS;
    fwver->fw_major = 3;
    fwver->fw_minor = 0;
    fwver->fw_patch = 0;
    strcpy(fwver->fw_verstr, "NOS-3.0.0");
    strcpy(fwver->fw_bdate, "10-10-2010");
    strcpy(fwver->fw_idate, "10-10-2011");

    return (0);
}

static inline fwversion_t* VERSION_FIND(version_array_t *va, char *name)
{
	int i;
	fwversion_t *ver = (fwversion_t *)va;
	for (i = 0; i < MAX_NUM_VERSION; i++, ver++) {
		if (ver && strcmp(ver->fw_name, name) == 0)
			return (ver);
	}
	return (NULL);
}

/* old still seems to be used */
#define MAX_MAJOR_NUM  255
#define MAX_MINOR_NUM  255
#define MAX_PATCH_NUM  255
#define MAX_DESCRIPTION_LEN 256
#define RV_UNKNOWN 0
#define RV_ALPHA   1
#define RV_BETA    2
#define RV_DEVELOPMENT 3
#define RV_RELEASE 4
#define FABOSVER_FILENAME "release"

#define PLIST_DESC_LEN  256

typedef struct r_version {
        uint8_t     rv_major;
        uint8_t     rv_minor;
        uint8_t     rv_patch;
        uint8_t     rv_state;
} r_version_t;

typedef struct release_info_s {
    r_version_t         ri_version;
    char                ri_time[32];
    char                ri_description[256];
} release_info_t;
typedef struct plist_hdr {
        uint32_t    plh_magic;
        uint32_t    plh_version;
        uint32_t    plh_sum;
        uint32_t    plh_size;
        struct {
       uint32_t r_platform;
       union {
          uint32_t  u_reserved[2];
          time_t    u_time;
       } r_u;
       r_version_t  r_version;
           char     r_desc[PLIST_DESC_LEN];
        } plh_release __attribute__((packed));
} plist_hdr_t;

static inline
int get_release_info(const char *path, release_info_t *rip)
{
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
