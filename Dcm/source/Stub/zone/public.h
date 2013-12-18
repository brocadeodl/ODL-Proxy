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

#ifndef __ZONEPUBLIC_H__
#define __ZONEPUBLIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define defaultZoneName         "d__efault__Zone"
#define defaultCfgName          "d__efault__Cfg"

#define ZN_FLAG_EFFECTIVE_CFG	0x08000000

#define MAX_ZONE_CFG_SZ (0x2U << 20) /* 2MB */

#define ZNIPC_ERRBUF_LEN	256

typedef enum {
    NAME = 0,           /* simple "C" style name */
	ZNCFG = 1,                 /* configuration entry */
//    CFG = 1,            /* configuration entry */
    ZONE = 2,           /* zone entry */
    ALIAS = 3,          /* alias entry */
    QLP = 4,            /* quickloop entry */
    CFG_END = 5,        /* end marker */
    IPO = 6,            /* ipozone entry */
//    ENABLE_CFG = 7,     /* effective config entry */
    ACTIVE_CFG = 8,     /* active config entry */
    ACTIVE_CFG_END = 9, /* active config end marker */
    INVALID_CFG = 0xff  /* invalid config type */
} zn_ctype_t;

/*
 * Every zone IPC call is passed an instance of this structure. It returns the
 * IPC call status, error number (in case of an error) and a detailed error
 * string (in case of error).
 */
typedef struct znIpcStatus {
	int	rc;
	int	err_num;
	char	err_buf[ZNIPC_ERRBUF_LEN];
} znIpcStatus_t;

#ifdef __cplusplus
}
#endif

using namespace WaveNs;

namespace DcmNs
{

static inline int groupCreate_telnet(int type, char *name, char *list) {
	return (0);
	}

static inline int groupAdd_telnet(int type, char *name, char *list) {
	return (0);
	}

static inline int groupRemove_telnet(int type, char *name, char *list) {
	return (0);
	}

static inline int groupDelete_telnet(int type, char *name) {
	return (0);
	}

static inline int cfgSave_telnet() {
	return (0);
	}

static inline int cfgClear_telnet() {
	return (0);
	}

static inline int cfgEnable_telnet(char *name) {
	return (0);
	}

static inline int cfgEnable_telnet_no_prompt(char *name, znIpcStatus_t *znStatus) {
	return (0);
	}

static inline int cfgDump(char *pBuf, int bufSz, int flags) {
	return (0);
	}

static inline int cfgTransDump(char *pBuf, int bufSz) {
	return (0);
	}

static inline int cfgDisable_telnet_no_prompt(znIpcStatus_t *pStatus) {
	return (0);
	}

static inline int cfgDisable_telnet_no_prompt_no_rcs() {
	return (0);
	}

static inline int cfgSave_telnet_no_prompt(znIpcStatus_t *znStatus) {
	return (0);
	}

static inline int cfgSave_telnet_no_prompt_no_rcs() {
	return (0);
	}

static inline int cfgClear_telnet_no_prompt(znIpcStatus_t *znStatus) {
	return (0);
	}

static inline int cfgSize_telnet_no_prompt(int *zone_db_max,
	int *zone_db_avail, int *zone_commited, int *zone_transaction) {
	*zone_db_max = 10000;
	*zone_db_avail = 9000;
	*zone_commited = 1000;
	*zone_transaction = 10;
	return (0);
	}

static inline int isDefzoneNoAccessSet(void) {
	return (1);
	}

static inline int isDefzonePendingTrans(void) {
	return (0);
	}

static inline int defZone_No_Access_telnet_no_prompt(void) {
	return (0);
	}

static inline int defZone_All_Access_telnet_no_prompt(void) {
	return (0);
	}

static inline int cfgTransShow(int *token, int *abortable) {
	*token = 7878;
	*abortable = 1;
	return (0);
	}

static inline int cfgTransForceAbort(int token) {
	return (0);
	}

static inline int cfgTransAbort_telnet(void) {
	return (0);
	}

static inline int isWWNFormatValid(char *wwn) {
	return (1);
	}

static inline void fmtParameterList(char *list, char **d_list) {
	return;
	}

static inline int IsNameValid(char *plist) {
	return (1); 
	}

static inline  int znGetLastZoneChangedTimestamp(char *lastChangedBuf){
	return (0);
}

static inline  int znUpdateLastZoneChangedTimestamp(int token){
	return (0);
}

static inline  int znGetLastZoneCommittedTimestamp(char *lastCommittedBuf){
	return (0);
}

static inline int isEffectiveAndDefinedConsistent(void) {
	return (0);
	}
}

#endif // __ZONEPUBLIC_H__
