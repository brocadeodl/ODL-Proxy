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
 *   Author : Charanjith Reddy Kunduru                                     *
 **************************************************************************/

#include "DcmResourceIdEnums.h"

#include "AAA/Utils/AAAErrorUtils.h"

using namespace WaveNs;
using namespace std;

namespace DcmNs
{

AAAErrorUtils::AAAErrorUtils()
{
	m_UserSecDcmErrorMap[ACCT_OK] = WAVE_MESSAGE_SUCCESS;
	m_UserSecDcmErrorMap[ACCT_CHANGE_FAILED] = USER_CHANGE_FS_ERROR;
	m_UserSecDcmErrorMap[ACCT_NAME_EXIST] = AAA_INCONSISTENT_FS_ERROR;
	m_UserSecDcmErrorMap[ACCT_NAME_NOT_EXIST] = AAA_INCONSISTENT_FS_ERROR;
	m_UserSecDcmErrorMap[ACCT_ATTR_NO_CHANGE] = AAA_INCONSISTENT_FS_ERROR;
	m_UserSecDcmErrorMap[ACCT_DB_ERROR] = USER_FS_DB_ERROR;
	m_UserSecDcmErrorMap[ACCT_NAME_SAME_AS_ROLE] = AAA_INCONSISTENT_FS_ERROR;
	m_UserSecDcmErrorMap[ACCT_RESERVED_NAME] = USER_NAME_RESERVED;

	m_RoleSecDcmErrorMap[DRBAC_OK] = WAVE_MESSAGE_SUCCESS;
	m_RoleSecDcmErrorMap[UDROLE_EXISTS_ERR] = AAA_INCONSISTENT_FS_ERROR;
	m_RoleSecDcmErrorMap[UDROLE_DOESNT_EXIST_ERR] = AAA_INCONSISTENT_FS_ERROR;
	m_RoleSecDcmErrorMap[UDROLE_USER_MATCH_ERR] = AAA_INCONSISTENT_FS_ERROR;
	m_RoleSecDcmErrorMap[UDROLE_RESERVED_ROLE_ERR] = ROLE_NAME_RESERVED;
}

AAAErrorUtils::~AAAErrorUtils()
{
}

ResourceId AAAErrorUtils::getUserDcmError(const secAcctErrCode& secErrCode) const
{
	map<secAcctErrCode, ResourceId>::const_iterator i;
	i = m_UserSecDcmErrorMap.find(secErrCode);
	if (i != m_UserSecDcmErrorMap.end())
	{
		return (i->second);
	}
	else
	{
		return AAA_GENERIC_ERROR;
	}
}

ResourceId AAAErrorUtils::getRoleDcmError(const drbac_err_t& drbacErrCode) const
{
	map<drbac_err_t, ResourceId>::const_iterator i;
	i = m_RoleSecDcmErrorMap.find(drbacErrCode);
	if (i != m_RoleSecDcmErrorMap.end())
	{
		return (i->second);
	}
	else
	{
		return AAA_GENERIC_ERROR;
	}
}

}
