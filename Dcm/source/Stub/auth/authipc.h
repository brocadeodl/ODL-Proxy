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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Newton                                                       *
 ***************************************************************************/

#ifndef __FCAUTH_AUTHIPC_H__
#define __FCAUTH_AUTHIPC_H__
#ifdef __cplusplus
extern "C"
{
#endif

/* dummy stub for fc auth authipc.h */

/* config string value */
#define AUTH_FCAP_STR     "fcap"
#define AUTH_DHCHAP_STR   "dhchap"
#define AUTH_ALLTYPE_STR "fcap,dhchap"
#define AUTH_GRP0_STR     "0"
#define AUTH_GRP1_STR     "1"
#define AUTH_GRP2_STR     "2"
#define AUTH_GRP3_STR     "3"
#define AUTH_GRP4_STR     "4"
#define AUTH_ALLGRP_STR   "0,1,2,3,4"
#define AUTH_SHA1_STR     "sha1"
#define AUTH_MD5_STR      "md5"
#define AUTH_ALLHASH_STR  "sha1,md5"

typedef enum {
    IPC_AUTH_SUCCESS        = 0,
    IPC_AUTH_DONE_WAS_SENT  = 1,    /* AUTH Done(Success) was sent */
    IPC_AUTH_DONE_NOT_SENT  = 2,    /* AUTH Done was not sent */
    IPC_AUTH_START_RECEIVED = 3,    /* AUTH start was recvd by fabricd */
    IPC_AUTH_NOT_STARTED    = 4,    /* AUTH not started since the policy is OFF/
PASSIVE or secmode disabled */
    IPC_AUTH_IN_PROGRESS    = 5,    /* AUTH is in progress */
    IPC_AUTH_DONE_FAILURE_SENT = 6, /* AUTH Done(failure) was sent */
    IPC_AUTH_DATA_ERROR     = -1    /* data recvd for auth tstart was bad */
} authRespCode;
/*
 * AUTH IPC Callback code
 */
typedef enum {
    CB_AUTH_START               = 0x0001,
    CB_AUTH_ISDONE_SENT         = 0x0002,
    CB_AUTH_OUTPUT_REDIRECT     = 0x0003, /* AUTH stdout/err redirection. */
    CB_AUTH_START_TEST          = 0x0004, /* library definitions */
    CB_AUTH_PROTOCOL_TYPE       = 0x0005,
    CB_AUTH_PORT_OFFLINE        = 0x0006,
    CB_AUTH_HBA_START           = 0x0007,
    CB_AUTH_AUTO_START          = 0x0008,
    CB_AUTH_LOG_CACHE           = 0x0009,
    CB_AUTH_SET_STATE           = 0x000a,
    CB_AUTH_POLICY_UTIL         = 0x000b,
    CB_AUTH_MAX_TYPE
} CbAuthEnum;

/* AUTHENTICATION type */
#define AUTH_FCAP   0x00000001
#define AUTH_DHCHAP 0x00000002
#define AUTH_SLAP   0x00000003

/* Auth Policy Utility Command codes */
#define AUTH_GET_POLICY 0x01
#define AUTH_SET_POLICY 0x02
#define AUTH_AUTHINIT   0x03
#define AUTH_GET_DEVICE_POLICY  0x04
#define AUTH_SET_DEVICE_POLICY  0x05

/* Auth Policy Utility Error codes */
typedef enum {
    AUTH_SUCCESS    = 0,
    AUTH_NO_MEMORY  = -1, /* Malloc failed */
    AUTH_CMD_ERR    = -2, /* No such command */
    AUTH_INPUT_ERR  = -3, /* User input error */
    AUTH_DATA_ERROR = -4,  /* Internal data corruption */
    AUTH_NULL_PTR   = -5,   /* Null pointer */
    AUTH_INVALID_POLICY = -6,   /* Invalid Auth policy */
    AUTH_SET_FAILED = -7,   /* Auth policy set failed */
    AUTH_NOT_CFG_KEY = -8,  /* Not an auth policy config key */
    AUTH_SECMODE_ENABLED = -9,  /* Sec mode enabled */
    AUTH_NO_DHCHAP  = -10,  /* DH_CHAP protocol is not configured */
    AUTH_IPC_ERR = -11,     /* IPC Init failure */
    AUTH_INVALID_PROTOCOL = -12,    /* Invalid Auth protocol */
    AUTH_PROTOCOL_CONFLICT = -13,   /* fcap when device policy is passive */
    AUTH_INVALID_GROUP = -14,       /* Invalid Group */
    AUTH_INVALID_HASH = -15,        /* Invalid Hash */
    AUTH_MD5_IN_FIPS = -16,         /* MD5 in fips mode */
    AUTH_INVALID_GROUP_FIPS = -17,          /* Invalid Group in fips mode */
    AUTH_LS_PORT_NOT_PRESENT    = -18,   /** Port not present in LS */
	AUTH_DB_MAX_ENTRIES_LIMIT_REACHED = -20 /** maxEntries*/
} authErrCode;

static inline int authProtocolType(int port, int *auth, int *group, int *hash)
{
    return AUTH_SUCCESS;
}

static inline int authGetPolicy(int cmd)
{
    return AUTH_SUCCESS;
}

static inline int authSetPolicy(int cmd, int policy)
{
    return AUTH_SUCCESS;
}

static inline int authApiSet(char *message[])
{
    return 0;
}

static inline int authApiSecretSet(char *pWwnValue, char *peerSecret,
    char *localSecret)
{
    return 0;
}

static inline int authApiSecretRemove(char *pWwnValue)
{
    return 0;
}

static inline  char *authApiSecretGet(int *count)
{
    *count = 0;
    return NULL;
}

static inline int secPolicyInit(void)
{
	return (0);
}

static inline char *convertLinkTypeToStr(UI32 lt)
{
	static char buff[24];
	memset(buff, 0, sizeof(buff));
	snprintf(buff,23,"%d",lt);
	return (buff);
	
}

#ifdef __cplusplus
}
#endif

#endif // __AUTHIPC_H__

