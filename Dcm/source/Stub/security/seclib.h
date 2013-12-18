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
 *    Copyright (c) 1996-2007 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *   This header file contains the common defines, macros, data structure
 *   and function prototype shared only between the security library
 *   and security server or security commands.
 *
 */
#ifndef SECLIB_H
#define	SECLIB_H
#include "security/public.h"

#define	BCD(x)	((((x) >> 4) & 0xf) * 10 + ((x) & 0xf))

/* preprocessor defines */

/* these are shared between PAM module and application */
/* login/password prompt */
#define	PASS_PROMPT		"Password: "
#define LOGIN_PROMPT		"login: "
#define SWINST_PROMPT		"Enter Switch Number to Login "
#define CHPASS_COMMENT		"Changing password for "
#define	INITPASS_COMMENT	"Setting initial password for "
#define	OLDPASS_PROMPT		"Enter old password: "
#define	NEWPASS_PROMPT		"Enter new password: "
#define NEWPASS_RETYPE		"Re-type new password: "
#define	OLDPASS			0
#define	NEWPASS			1
#define	SEC_MIN_PASS_LEN	8
#define SEC_MAX_PASS_LEN	40
#define HOST_MAX_LEN		256
#define MAX_LENGTH			32
#define FIPS_MODE			"fips.mode"
#define FIPS_SELFTESTS		"fips.selftests"
#define FIPS_PROMALLOW		"fips.promallow"

/*
 * define for bogus password applied to non fcs switches
 * the length of the passwd is shorter than the normal
 * hash, so that it can never be created.
 */
#define SEC_BOGUS_NFCS_PASSWD	"$1$XXXXXXX"

/* PAM environment variables */
#define OLDAUTHENV		"OLDCRYPT"
#define NEWAUTHENV		"NEWCRYPT"

/* environment variables */
#define ENV_LOGNAME		"LOGNAME"
#define ENV_SWLOGNAME	"SWLOGNAME"
#define	ENV_SWNO		"FABOS_SWITCHNO"
#define ENV_AUTHNMECH	"AUTHN_MECH"
#define ENV_DUP_SWNO	"DUP_FABOS_SWITCHNO"
#define ENV_PWD_DUP		"PWD_CHK_DUP"
#define ENV_LOGINID		EP_ENV_NAME_VAL
#define	ENV_ROLEID		EP_ENV_ROLE_VAL
#define	ENV_AD			EP_ENV_AD_VAL
#define	ENV_REMOTE_IP	EP_ENV_IP_VAL

#define	ENV_VF			EP_ENV_VF_VAL

#define AUTHMECH_LOCAL	"local"
#define AUTHMECH_RADIUS	"radius"
#define AUTHDESC_RADIUS "RADIUS Account"

#define ACTLOCKFILE	"/tmp/__actplock.%d"
#define DEFLOCKFILE	"/tmp/__defplock.%d"

#define FIPS_CONF_FILE			"/etc/fabos/fips_selftests.conf"
#define MNT_FIPS_CONF_FILE			"/mnt/etc/fabos/fips_selftests.conf"
#define FIPS_CONF_TEMP			"/var/run/fips_selftests.conf.tmp"
#define FIPS_CONF_LOCK			"/tmp/.__secfipslock"

#define NETFILTER_SCRIPT "/fabos/libexec/createiptab"

/* Supports switch instance up to 99 */
#define SWINST_MAX_DIGITS	(2)

/* TTY names used by swTTYLogout() and by TTY ACL file */
#define	SERIAL_TTYNAME	"/dev/ttyS0"
#define MODEM_TTYNAME	"/dev/ttyS1"

/* Make sure this is large enough to hold any TTY_NAME_... string */
#define MAX_TTY_NAME_SZ (max(sizeof (SERIAL_TTYNAME), sizeof (MODEM_TTYNAME)))

/* File name of TTY ACL file for each switch */
#define TTY_ACL_FN	"/tmp/ttyAcl.%d.conf"

#define	PWD_FILE		"/etc/passwd"
#define	PW_FILE			PWD_FILE				/* used by passwd.c */
#define PWD_FILE_TMP	"/var/run/passwd.tmp"
#define PWD_DEFAULT     "/etc/passwd.default"
#define	PWD_BAK			"/var/run/passwd.bak"
#define GRP_FILE        "/etc/group"
#define GRP_FILE_TMP	"/var/run/group.tmp"
#define GRP_DEFAULT     "/etc/group.default"
#define	GRP_BAK			"/var/run/group.bak"
#define SPWD_FILE		"/etc/shadow"
#define SPWD_FILE_TMP	"/var/run/shadow.tmp"
#define SPWD_DEFAULT	"/etc/shadow.default"
#define	SPWD_BAK		"/var/run/shadow.bak"
#define	SEC_SHADOW_PASSWD	"x"
#define	FAIL_LOG_FILE	"/etc/fabos/faillog"
#define	FAIL_LOG_FILE_TMP	"/etc/fabos/faillog.tmp"
/* defines for storing old passwds */
#define OPW_TMPFILE			"/var/run/nopasswd"
#define OLD_PASSWORDS_FILE	"/etc/fabos/opasswd"
#define OLDPW_BUF_SIZE		16380


/* Lock file name for changing password */
#define PW_LOCKFILE	"/tmp/.__secpwdlock"

/* Defines for security database */
/*
 * SEC_MEMBER_LEN must be big enough to hold a string of the
 * following format:
 * 11:22:33:44:55:66:77:88(0,1,2,3,4,...,max port supported-1)
 */
#define SEC_MEMBER_LEN			939 * 5
#define SEC_NAME_LEN			 32

/* command telnet type */
#define	SEC_SECURITY_ON			0x01
#define	SEC_SECURITY_DB			0x02
#define	SEC_SECURITY_OFF		0x03
#define	SEC_DELETING_ACTIVE		0x04
#define	SEC_DELETING_DEFINE		0x05
#define	SEC_ACTIVATING			0x06
#define	SEC_COMMITING			0x07
#define SEC_RESETING_STAMP		0x08
#define	SEC_EXCHANGE_FCS		0x09
#define	SEC_DOWNLOAD			0x0a
#define SEC_DOWNLOAD_CERT		0x0b
#define	SEC_FCS_QUERY_VSTAMP	0x0c
#define	SEC_ABORTING			0x0d
#define	SEC_CREATING_POLICY		0x0e
#define	SEC_DELETING_POLICY		0x0f
#define	SEC_ADDING_MEMBER		0x10
#define	SEC_REMOVING_MEMBER		0x11
#define	SEC_SHOW				0x12
#define	SEC_RESET_STAMP			0x13
#define	SEC_SHOW_MODE			0x14
#define SEC_MOVING_FCS			0x15
#define	SEC_SWITCH_READY		0x16
#define	SEC_SWITCH_SEGMENT		0x17
#define	SEC_FAILOVER		    0x18
#define	SEC_INFORM_PRIMARY		0x19
#define	SEC_TRANS_ABORT			0x1a
#define	SEC_SEND_FAILOVER		0x1b
#define SEC_SEND_FAB_STATS_RESET	0x1c
#define SEC_SEND_FAB_STATS_GET		0x1d
#define	SEC_FS_TOV				0x1e
#define	SEC_ZONE_DOWNLOAD		0x1f
#define SEC_ZONE_QUERY			0x20
#define	SEC_FCS_QUERY_CERT_DATA 0x21
#define	SEC_NO_COMMAND			0x2F
#define SEC_PW_SNMP				0x30
#define SEC_HEADER_PAYLOAD		0x40
#define SEC_FCS_SHOW			0x41
/* Begin - IU Command Types for ACL -- starting from 0x60 */
#define SEC_ACL_SAVE			0x60
#define SEC_ACL_ACTIVATE		0x61
#define SEC_ACL_DISTRIBUTE		0x62
#define SEC_FABWIDECFG_SEND		0x63
#define SEC_FDD_QUERY			0x64 /* Not used in IU but reqd for log */
/* End - IU Command Types for ACL -- starting from 0x60 */

#define SEC_CMD_TEST			0x01
#define SEC_CMD_PASSWD			0x10
#define SEC_CMD_SETSWITCHPASSWD		0x11
#define SEC_CMD_SETNONFCSPASSWD		0x12
#define SEC_CMD_GETFCSPASSWDDB		0x13
#define SEC_CMD_GETNONFCSPASSWDDB	0x14
#define SEC_CMD_ACTIVE_MAC	0x15
#define SEC_CMD_DEFINE_MAC	0x16
#define SEC_CMD_REFRESH_PASSWD		0x17
#define SEC_CMD_SETUSERPASSWD	0x18
#define SEC_CMD_SNMP			0x20

#define MYCPNUM getThisCPNum()
#define SINGLE_ITEM	1
#define CHAP_DB_DELIM	' '	/* space */

#define	SP_NA_VAL		-1
#define	STRDUP(s1)		(s1 != NULL) ? strdup(s1) : NULL

#define SWAP_LONG(x)	((0xff & x) << 24) |	\
					    ((0xff00 & x) << 8) |	\
					    ((0xff0000 & x) >> 8) |	\
					    ((0xff000000 & x) >> 24)

#define FABBIND_LIST_EXACT_MATCH	1

/* type defines */

/*
 * This define is required in public.h but is redefined as
 * SEC_PW_MAX_IDENTITY_LEN to avoid conflicts
 */
#define MAX_IDENTITY_LEN SEC_PW_MAX_IDENTITY_LEN

/*
 * structure for distributing single password. This is the same as in
 * os/passwd.h of vxWorks.
 */
/*
 * password PAM conversation prompt data
 */
typedef	struct {
	const char	*prompt[2];
} secPrompt_t;

/* a key value pair used for extended account attribute */
typedef struct {
	char	*key;				/* attribute key */
	char	*value;				/* attribute value */
} kv_t;

/* an array of extended account attribute key value pairs */
typedef struct {
	int		length;				/* array length */
	kv_t	*data;				/* array of key value pairs */
} kva_t;


typedef enum {
	ACCT_PWD_NAME	= 0,
	ACCT_GRP_NAME,
	ACCT_GRP_GID,
	ACCT_SPWD_NAME
} acctKey_e;

#endif

