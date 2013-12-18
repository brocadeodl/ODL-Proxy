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

#ifndef AAALOCALTYPESH_H
#define AAALOCALTYPESH_H

namespace DcmNs
{
#define LAST_CMD_SYNC_TEST   1
#define YEAR_OFFSET 1900
#define MONTH_OFFSET 1
#define 	STD_ACL_INDEX(seqnum)		((seqnum * 2) + 1)
#define 	EXT_ACL_INDEX(seqnum)		((seqnum) + 1)
/* Cert-util */
#define LDAP_CERT_FILE "/etc/fabos/certs/ldap/swLdapca.pem"
#define SYSLOGCA_CERT_FILE "/etc/syslog-ng/ca.d/ca.pem"
#define SSHKEY_AUTH_FILE "/fabos/users/admin/.ssh/authorized_keys"

	typedef enum
	{
		AAALOCALADDUSER,
		AAALOCALCHANGEUSER,
		AAALOCALDELETEUSER,
		AAALOCALUSERACTION,
		AAALOCALADDROLE,
		AAALOCALCHANGEROLE,
		AAALOCALDELETEROLE,
		AAALOCALADDRADIUSHOST,
		AAALOCALCHANGERADIUSHOST,
		AAALOCALDELETERADIUSHOST,
		AAALOCALADDTACACSHOST,
		AAALOCALCHANGETACACSHOST,
		AAALOCALDELETETACACSHOST,
		AAALOCALADDLDAPHOST,
		AAALOCALCHANGELDAPHOST,
		AAALOCALDELETELDAPHOST,
		AAALOCALCHANGEAUTHLOGINMODE,
		AAALOCALSETPASSWORDATTRIBUTES,
		AAALOCALSETBANNER,
		AAALOCALCLEANCACHE,
		AAALOCALGETACTIVEUSERS,
		AAALOCALGETLOCKEDUSERS,
		AAALOCALADDMAPROLE,
		AAALOCALCHANGEMAPROLE,
		AAALOCALDELETEMAPROLE,
		AAALOCALIPACLCONFIG,
		AAALOCALIPACLDELETE,
		AAALOCALSTDIPACLRULE,
		AAALOCALEXTIPACLRULE,
		CERTUTILOPACTION,
		CERTUTILOPSHOW,
		AAALOCALASSOCIPRULETOPOLICY,
		AAALOCALIPACLRESEQ,
		AAALOCALASSOCIP6RULETOPOLICY,
		AAALOCALCHANGEACCOUNTING,
		AAALOCALSHOWIPACL,
        TELNETSERVERLOCALCONFIG,
        TELNETSERVERLOCALSHOW,
        SSHSERVERLOCALCONFIG,
        SSHSERVERLISTLOCALADD,
        SSHSERVERLISTLOCALDELETE,
        SSHSERVERLOCALSHOW,
        SSHSERVERLISTLOCALSHOW,
        CLEARSESSIONSLOCAL
	} AAALocal;

	typedef enum
    {   
        SSH_KEX_DEFAULT = 1,
        SSH_KEX_DH_GROUP_14
    } SSHLocalKeyExchangeProtocol;
}

#endif //AAALOCALTYPESH_H
