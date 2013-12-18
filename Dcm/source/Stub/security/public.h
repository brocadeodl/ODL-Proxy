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
 *   Author : Charanjith Kunduru                                           *
 ***************************************************************************/

#ifndef __SECURITY_PUBLIC_H__
#define __SECURITY_PUBLIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define FREE(ptr) if (ptr != NULL) { free(ptr); ptr = NULL; }

#define RADIUS_CHAP_PROTO   0
#define RADIUS_PAP_PROTO    1
#define RADIUS_PEAP_PROTO   2
#define RADIUS_DEFAULT_PORT 1812
#define RADIUS_DEFAULT_TIMEOUT  5
#define RADIUS_DEFAULT_RETRANSMIT  5
#define RADIUS_DEFAULT_PROTO    RADIUS_CHAP_PROTO
#define RADIUS_MIN_TIMEOUT  1
#define RADIUS_MAX_TIMEOUT  60
#define RADIUS_DEFAULT_DEADTIME 60
#define RADIUS_DEFAULT_SECRET "sharedsecret"
#define MIN_RADIUS_KEY_LEN 8
#define MAX_RADIUS_KEY_LEN 40
#define MIN_RADIUS_RETRANS 0
#define MAX_RADIUS_RETRANS 100
#define MIN_RADIUS_DEADTIME 1
#define MAX_RADIUS_DEADTIME 2880

#define TACACS_DEFAULT_PORT 49

#define MIN_TACACS_KEY_LEN 1

#define ACCT_MIN_NAME_LEN 1
#define ACCT_MAX_NAME_LEN 40

#define ACCT_MAX_DESC_LEN       64

#define ACCT_SYS_DEF_PW_UID     0
#define ACCT_SYS_DEF_PW_DIR     "/fabos/users/user"
#define ACCT_SYS_DEF_PW_SSH_DIR "/fabos/users/user/.ssh"
#define ACCT_SYS_DEF_PW_ADMIN_DIR     "/fabos/users/admin/.ssh"
#define ACCT_SYS_DEF_PW_ADMIN_SSH_DIR "/fabos/users/admin"

#define ACCT_ROOT_ROLE         "root"

#define ACCT_ADMIN_ROLE         "admin"
#define ACCT_ADMIN_DESC         "Administrator"
#define ACCT_ADMIN_GID          600

#define ACCT_USER_ROLE         "user"
#define ACCT_USER_DESC         "User"
#define ACCT_USER_GID          602

#define ROLE_USERS_DELIMITER	" "

#define ACCT_MAX_NUM 64

#define PAM_1ST_PASSWD_SERVICE 1stpasswd

#define ROLE_MIN_NAME_LEN 4
#define ROLE_MAX_NAME_LEN 16

#define ROLE_MAX_NUM 64
#define RULE_MAX_NUM 1024
#define UDROLE_BASE_GID 700
#define RADIUS_MAX_SERVERS 5
#define TACACS_MAX_SERVERS 5
#define LDAP_MAX_SERVERS 5
#define PW_CFG_NO_CHANGE -9999
#define SEC_PWERR_LOWER -43
#define SEC_PWERR_INVALID_CHAR -52

#define RADIUS_CONFIG_DUP   -7
#define RADIUS_CONFIG_NOT_FOUND -13
#define RADIUS_CONFIG_NOT_CHANGED   -16
#define TACACS_CONFIG_DUP   -7
#define TACACS_CONFIG_NOT_FOUND -13
#define TACACS_CONFIG_NOT_CHANGED   -16
#define LDAP_CONFIG_DUP       -7
#define LDAP_CONFIG_NOT_FOUND -13
#define LDAP_CONFIG_NOT_CHANGED   -16
#define LDAP_CONFIG_ENABLED -17 // Legacy

#define SEC_MAX_BANNER_LEN 2050
#define	PW_CFG_SUCCESS 0
#define	SEC_OK 0

#define LOGIN_BANNER 2
#define MOTD_BANNER 3
#define INCOMING_BANNER 4

typedef struct {
    char    *hostname;
    char    *secret;
    int timeout;
    int retries;
    u_short port;
    u_char  proto;
} secRadConf_t;

typedef secRadConf_t secTacacsConf_t;

typedef enum {
	LOCAL_ONLY= 0,
	RADIUS_ONLY,
	notUsed1,
	RADIUS_LOCLBKUP,
	notUsed2,
	RADIUS_LOCAL,
	LDAP_ONLY,
	LDAP_LOCAL,
	LDAP_LOCLBKUP,
	LDAP_RADIUS,
	RADIUS_LDAP,
	LDAP_RADIUS_LOCAL,
	RADIUS_LDAP_LOCAL,
	TACACS_ONLY,
	TACACS_LOCAL,
    TACACS_LOCLBKUP,
	MAX_AUTHTYPE_CONF
} secAuthTypeConf_t;

#define secRadiusAdd(x) 0; printf("secRadiusAdd is successful\n")
#define secRadiusChange(x) 0; printf("secRadiusChange is successful\n")
#define secRadiusRemove(x) 0; printf("secRadiusRemove is successful\n")
#define secRadiusGet(x,y) 0; printf("secRadiusGet is successful\n")
#define secRadiusConfigClean() 0; printf("secRadiusConfigClean is successful\n")

#define secTacacsAdd(x) 0; printf("secTacacsAdd is successful\n")
#define secTacacsChange(x) 0; printf("secTacacsChange is successful\n")
#define secTacacsRemove(x) 0; printf("secTacacsRemove is successful\n")
#define secTacacsGet(x,y) 0; printf("secTacacsGet is successful\n")
#define secTacacsConfigClean() 0; printf("secTacacsConfigClean is successful\n")

#define secLdapAdd(x) 0; printf("secLDAPAdd is successful\n")
#define secLdapChange(x) 0; printf("secLDAPChange is successful\n")
#define secLdapRemove(x) 0; printf("secLDAPRemove is successful\n")
#define secLdapGet(x,y) 0; printf("secLDAPGet is successful\n")
#define secLdapConfigClean() 0; printf("secLDAPConfigClean is successful\n")
#define secLdapMapClean() 0; printf("secLDAPMapClean is successful\n")
#define secLdapUnmapRole(x) 0; printf("secLDAPUnmapRole is successful\n")
#define secLdapMapRole(x, y) 0; printf("secLDAPMapRole is successful\n")

#define secAuthConfig(v,w,x,y,z) 0; if (((w == x) == y) == z) printf (" "); printf("secAuthConfig is successful\n")

typedef u_long acctAD_t[8];
typedef enum {
	    AUTH_PEER = 0,
		AUTH_LOCAL = 1
} AuthEnum;

/* user account field structure */
typedef struct {
	char    *name;          /* account name */
	char    *role;          /* account role name */
	char    *desc;          /* account description */
	char    *passwd;        /* encrypted password */
	int     enabled;        /* flag indicating if account is enabled */
	int     locked;         /* flag indicating if account is locked out */
	int     homeAD;         /* the account's designated homeAD  or home VF*/
	int     promptchg;      /* flag for prompting password change */
} secAcct_t;

/* User management error codes */
typedef enum {
    ACCT_OK             =  0,
    ACCT_CHANGE_FAILED  = -6,
    ACCT_NAME_EXIST     = -8,
    ACCT_NAME_NOT_EXIST = -9,
    ACCT_ATTR_NO_CHANGE = -10,
    ACCT_DB_ERROR       = -11,
    ACCT_NAME_SAME_AS_ROLE = -22,
    ACCT_RESERVED_NAME   = -46
} secAcctErrCode;

typedef enum {
	DRBAC_OK                        = 0,
	UDROLE_EXISTS_ERR               = -4,
	UDROLE_DOESNT_EXIST_ERR         = -5,
	UDROLE_USER_MATCH_ERR           = -23,
	UDROLE_RESERVED_ROLE_ERR        = -24,
	DRBAC_MAX_ERR                   = -100
} drbac_err_t;

#define secEncryptedPasswdGet(a,b,c,d,e,f,g) 0; \
					int passlen = strlen(d); \
					md5hash = strdup("$1$xx$ABCabcDEFdefGHIghiJKjk"); \
					sprintf(&(md5hash[28-passlen]), "%s", d);

#define secAcctAdd(x) 0; printf("secAcctAdd OK");
#define secAcctChange(x) 0; printf("secAcctChange OK");
#define secAcctDelete(x) 0; printf("secAcctDelete OK");
#define secAcctGet(w,x,y,z) 0; printf("secAcctGet OK");

typedef struct {
	char	*name;     /* Name of the role */
	char	*desc;
	char	**users;
	unsigned int	userNum;
	unsigned int	gid;
} secRole_t;

#define secRoleAdd(x) 0; printf("secRoleAdd OK\n")
#define secRoleChange(x) 0; printf("secRoleChange OK\n")
#define secRoleDelete(x) 0; printf("secRoleDelete OK\n")
#define secRoleGetAll(x,y) 0; printf("secRoleGetAll OK\n")

typedef struct
{
	int min_len;
	int lockthresh;
	int lower;
	int upper;
	int digit;
	int punct;

	/* Unused in Virgo release */
	int history;
	int minpassage;
	int maxpassage;
	int warn;
	int lockdur;
	int adminlockout;
	int repeat;
	int sequence;
	int status;
} secPwdCfg_t;

/* LDAP configuration information */
typedef struct {
        char    *hostname;
		char	*domain;
        int     timeout;
		int		retries;
        u_short port;
} secLdapConf_t;
#define secPwdCfgSet(x,y,z) 0; if (x.min_len == 0); printf("secPwdCfgSet is OK\n")
#define secPwdCfgSetDefault() 0; printf("secPwdCfgSetDefault is OK\n")
#define secPwdCfgGetAll(x) 0
#define secBannerSet(x,y,z) 0; printf("secBannerSet is OK\n")
#define secBannerGet(x,y, z, w) 0

/* security policy SCC */
typedef enum {
	SEC_NO_ERROR        = 0,
	SEC_NO_MEMORY       = -1,   /* memory allocation failed */
	SEC_NO_ACCESS       = -2,   /* access temporary not available */
	SEC_INPUT_ERROR     = -3,   /* this usually means user input */
	SEC_DATA_ERROR      = -4,   /* internal data corruption? */
	SEC_PERM_DENY       = -7,
	SEC_DB_TOO_LARGE	= -53	
} secErrorCode;

#define secPolicyCreateNos(x,y) 0; printf("secPolicyCreate is OK\n")
#define secPolicyAddNos(x,y) 0; printf("secPolicyAdd is OK\n")
#define secPolicySaveNos() 0; printf("secPolicySave is OK\n")
#define secPolicyRemoveNos(x,y) 0; printf("secPolicyRemove is OK\n")
#define secPolicyDeleteNos(x) 0; printf("secPolicyDelete is OK\n")
#define secPolicyActivateNos() 0; printf("secPolicyActivate is OK\n")
#define secPolicyCfgDownloadNos(x,y) 0; printf("secPolicyCfgDownload is OK\n")
#define secPolicyEnableFCPorts() 0; printf("secPolicyEnableFCPorts is OK\n")

/* Login and Command Accounting related APIs */
#define secTacacsExecAccSet(x) 0; printf("secTacacsExecAccSet is OK\n")
#define secTacacsCmdAccSet(x) 0; printf("secTacacsCmdAccSet is OK\n")
#define secTacacsExecAccGet() 0; printf("secTacacsExecAccGet is OK\n")
#define secTacacsCmdAccGet() 0; printf("secTacacsCmdAccGet is OK\n")
#define secSendTacacsAcctMsg(a, b, c, d, e, f, g) 0; printf("secSendTacacsAcctMsg is OK\n")

/* FIPS cfg */
typedef enum { STESTS_ERR = -1, STESTS_DEFAULT, STESTS_SET_NK, STESTS_KF,
		STESTS_SET, STESTS_RESET_NR } sTestStates;
typedef enum { FIPS_ERR = -1, FIPS_DEFAULT, FIPS_SET } fipsStates;
//#define secFipsModeGet() printf("secFipsModeGet is OK\n"), 0
#define secFipsModeGet() 1
#define secCfgloadSecureGet() 0; printf("secCfgloadSecureGet is OK\n")
#define secFipsSelfTestsGet() 0; printf("secFipsSelfTestsGet is OK\n")

#define system(x) 0; printf(x);

#ifdef __cplusplus
}
#endif

#endif // __SECURITY_PUBLIC_H__


