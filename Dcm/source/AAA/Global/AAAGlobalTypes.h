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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : ckunduru                                                     *
 **************************************************************************/

#ifndef AAAGLOBALTYPES_H
#define AAAGLOBALTYPES_H

#include "brocade-aaa.h"

namespace DcmNs
{
	#define MAP_INDEX(index)    ((index) * (100))
	#define	ALL_CONTEXTS		"*"
	#define	ALL_COMMANDS		"*"
	#define	ALL_NAMESPACES		"*"
	#define	INVALID_ENUM_COMMAND	9999
	#define	INVALID_INTERFACE_COMMAND	""
	#define	MAX_RULES				801000
	#define	MAX_NO_CMDS				200
	#define	MIN_RULE_INDEX			1		//Minimum Rule index user can enter
	#define	MAX_RULE_INDEX			512	//Maximum Rule index user can enter	
	#define	DEFAULT_ADMIN_ROLE		"admin"
	#define	DEFAULT_USER_ROLE		"user"

	#define USER_ACCT_DEFAULT_PASSWORD	"password"
	#define USER_ACCT_DEFAULT_ENCRYPTED_PASSWORD	"BwrsDbB+tABWGWpINOVKoQ==\n"
	#define USER_ACCT_DEFAULT_MD5_PASSWORD	"$1$.rj8Rlo3$BQNdZbg0Kv1t9e5Z7dR/U0"
	#define LDAP_MAX_MAPPING 16     // Maximum limit for ldap mapping

	typedef enum
	{
		AAAGLOBALADDUSER,
		AAAGLOBALCHANGEUSER,
		AAAGLOBALDELETEUSER,
		AAAGLOBALUSERACTION,
		AAAGLOBALADDROLE,
		AAAGLOBALCHANGEROLE,
		AAAGLOBALDELETEROLE,
		AAAGLOBALADDRULE,
		AAAGLOBALCHANGERULE,
		AAAGLOBALDELETERULE,
		AAAGLOBALCHANGERADIUSSETTINGS,
		AAAGLOBALADDRADIUSHOST,
		AAAGLOBALCHANGERADIUSHOST,
		AAAGLOBALDELETERADIUSHOST,
		AAAGLOBALCHANGETACACSSETTINGS,
		AAAGLOBALADDTACACSHOST,
		AAAGLOBALCHANGETACACSHOST,
		AAAGLOBALDELETETACACSHOST,
		AAAGLOBALCHANGELDAPSETTINGS,
		AAAGLOBALADDLDAPHOST,
		AAAGLOBALCHANGELDAPHOST,
		AAAGLOBALDELETELDAPHOST,
		AAAGLOBALCHANGEAUTHLOGINMODE,
		AAAGLOBALSETPASSWORDATTRIBUTES,
		AAAGLOBALSETBANNER,
		AAAGLOBALGETACTIVEUSERS,
		AAAGLOBALGETLOCKEDUSERS,
		AAAGLOBALSETPASSWORDENCRYPTION,
		AAAGLOBALADDMAPROLE,
		AAAGLOBALCHANGEMAPROLE,
		AAAGLOBALDELETEMAPROLE,
		AAAGLOBALIPACLPOLICY,
		AAAGLOBALSTDIPACLRULE,
		AAAGLOBALEXTIPACLRULE,
		AAAGLOBALCERTUTILOPACTION,
		AAAGLOBALCHANGEACCOUNTING,
        TELNETSERVERGLOBALCONFIG,
        SSHSERVERGLOBALCONFIG,
        SSHSERVERLISTGLOBALADD,
        SSHSERVERLISTGLOBALDELETE
	} AAAGlobal;

	typedef enum
	{
		AAA_ADD_OPER,
		AAA_CHANGE_OPER,
		AAA_DELETE_OPER
	} AAAGlobalMessageOperationType;

	typedef enum
	{
		CHAP,
		PAP,
		PEAP_MSCHAP,
		NO_AUTHPROTO
	} AuthProtocol;

	typedef enum
	{
		RADIUS_DB = brcd_aaa_radius,
		TACACS_DB = brcd_aaa_tacacs0x2b,
		LOCAL_DB = brcd_aaa_local,
		LDAP_DB = brcd_aaa_ldap,
		LOCALBKUP_DB = brcd_aaa_local_auth_fallback,
		NO_AAA = 10
	} AAAServerType;

	typedef enum
    {
        NO_SRV = brcd_aaa_none,
        TACACS_SRV = brcd_aaa_tacacs0x2b
    } AccServerType;

    typedef enum
    {
        EXEC_ACC = 0,
        CMD_ACC
    } AccType;

	typedef enum
	{
		INVALID_OPERATION = -1,
		READ_ONLY = 0,
		READ_WRITE = 1,// This is internal field
		READ_EXECUTE = 2,
		READ_WRITE_EXECUTE = 3
	} RBACOperation;

	typedef enum
	{
		INVALID_ACTION = -1,
		ACCEPT = 0,
		REJECT = 1
	} RBACAction;

	typedef enum
	{
		CMD_TYPE_INVALID = -1,
		CMD_TYPE_ENUM_CMDS = 1,
		CMD_TYPE_INTERFACE,
		CMD_TYPE_INTERFACE_FCOE,
		CMD_TYPE_INTERFACE_TE,
		CMD_TYPE_INTERFACE_GE,
		CMD_TYPE_INTERFACE_PC,
		CMD_TYPE_INTERFACE_VLAN,
		CMD_TYPE_INTERFACE_FCOE_LEAF,
		CMD_TYPE_INTERFACE_TE_LEAF,
		CMD_TYPE_INTERFACE_GE_LEAF,
		CMD_TYPE_INTERFACE_PC_LEAF,
		CMD_TYPE_INTERFACE_VLAN_LEAF,
		CMD_TYPE_INTERFACE_MGMT,
		CMD_TYPE_COPY,
		CMD_TYPE_COPY_RUNNING,
		CMD_TYPE_COPY_SUPPORT,
		CMD_TYPE_CLEAR,
		CMD_TYPE_CLEAR_LOGGING,
		CMD_TYPE_CLEAR_SUPPORT,
		CMD_TYPE_PROTOCOL,
		CMD_TYPE_PROTOCOL_SPANNING_TREE,
		CMD_TYPE_PROTOCOL_LLDP
	} RBACCommandType;

	typedef enum
	{
		USER_UNLOCK
	} AAAActionType;

	typedef enum
	{
		ENCRYPTION_CLEAR_TEXT = 0,
		ENCRYPTION_HASH = 7
	} AAAEncryptionLevel;
	#define DEFAULT_ENCRYPTION_LEVEL	ENCRYPTION_CLEAR_TEXT

	typedef enum
	{
		STANDARD_IP_ACL = 0,
		EXTENDED_IP_ACL,
		ACL_TYPE_UNKNOWN
	} IpAclType;

	typedef enum
	{
		ACL_PERMIT = 1,
		ACL_DENY = 2,
		ACL_ACTION_UNDEFINED = 10
	} IpAclAction;

	typedef enum
	{
		ANY_HOST = 1,
		NO_WILD_CARD = 10
	} AclWildCard;

	typedef enum
	{
		ACL_PROTO_IP = 0,
		ACL_PROTO_TCP = 6,
		ACL_PROTO_UDP = 17,
		ACL_PROTO_UNDEFINED = 100
	} AclProtocol;

	typedef enum
	{
		ACL_IPV4 = 0,
		ACL_IPV6
	} IpAclVersion;
/*
	typedef enum
	{
		IPV4_ADDR_ANY = 1,
		IPV4_ADDR_HOST = 2,
		IPV4_ADDR_SUBNET,
		IPV4_ADDR_UNDEFINED = 10
	} IpAddrType;
*/
	typedef enum
	{
		ACL_TRAFFIC_IN = 1,
		ACL_TRAFFIC_OUT
	} ACLTrafficType;

	typedef enum
	{
		ACL_PORT_EQ = 1,
		ACL_PORT_GT,
		ACL_PORT_LT,
		ACL_PORT_RANGE = 5,
		ACL_PORT_UNDEFINED = 10
	} AclPortType;

// #define IP_ACL_NO_PORT_VAL 0


	typedef enum
	{
		CERT_LDAPCA_IMPORT,
		CERT_LDAPCA_DELETE,
		CERT_LDAPCA_SHOWCERT,
		CERT_SYSLOGCA_IMPORT,
		CERT_SYSLOGCA_DELETE,
		CERT_SYSLOGCA_SHOWCERT,
		CERT_SSHKEY_IMPORT,
		CERT_SSHKEY_SHOW,
		CERT_SSHKEY_DELETE,
        CERT_GENERATE_FCAPKEYCSR,
        CERT_EXPORT_FCAP,
        CERT_IMPORT_FCAP,
        CERT_DELETE_FCAPALL,
        CERT_DELETE_FCAPSWCERT,
        CERT_DELETE_FCAPCACERT,
        CERT_SHOW_FCAPALL,
        CERT_SHOW_FCAPSWCERT,
        CERT_SHOW_FCAPCACERT
	} CertType;

	typedef enum
	{
		ADMIN_HAS_DEFAULT_PASSWD,
		USER_HAS_DEFAULT_PASSWD,
		BOTH_HAS_DEFAULT_PASSWD,
		NONE_HAS_DEFAULT_PASSWD
	} AcctDefaultPasswd;

#if 0
    typedef enum
    {
        SSH_KEX_DEFAULT = 1,
        SSH_KEX_DH_GROUP_14
    } SSHKeyExchangeProtocol;
#endif

	#define IP_ACL_NO_PORT_VAL 65536

	#define RADIUS_DEF_AUTH_PORT    1812
    #define RADIUS_DEF_KEY  "sharedsecret"
    #define RADIUS_DEF_RETRANSMIT   5
    #define RADIUS_DEF_TMOUT    5
    #define RADIUS_DEF_PROTO	CHAP

	#define LDAP_DEF_BASEDN "local"
	#define LDAP_DEF_PORT	389
	#define LDAP_DEF_RETRY	5
	#define LDAP_DEF_TMOUT	5

	#define TAC_DEF_KEY	"sharedsecret"
	#define TAC_DEF_PORT	49
	#define TAC_DEF_PROTO	CHAP
	#define TAC_DEF_RETRY	5
	#define TAC_DEF_TMOUT	5
}
#endif // AAAGLOBALTYPES_H
