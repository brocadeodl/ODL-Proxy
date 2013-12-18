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

#if 0
/***************************************************************************
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Charanjith Reddy Kunduru                                    *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "DcmCore/DcmToolKit.h"

#include "Ssm/Global/IP6IP6AAASequencerContexts.h"

using namespace WaveNs;

namespace DcmNs
{

IP6AAASequencerContext::IP6AAASequencerContext (PrismMessage *pPrismMessage)
	: m_pPrismMsg (pPrismMessage)
{
}

IP6AAASequencerContext::~IP6AAASequencerContext ()
{
}

PrismMessage* IP6AAASequencerContext::getPPrismMessage() const
{
	return (m_pPrismMsg);
}

void IP6AAASequencerContext::traceSendToClusterErrors(WaveSendToClusterContext *pWaveSendToClusterContext, const string &callerMethodName, string &errorStr, const bool &isPartialSuccessEnabled)
{
	trace (TRACE_LEVEL_INFO, "AAAContext::traceSendToClusterErrors : Error occured in SendToWaveCluster for " + callerMethodName);

	ResourceId status = WAVE_MESSAGE_SUCCESS;
	vector<LocationId> failedSwIds, succSwIds;
	vector<ResourceId> failedSwErrs;
	UI32 swId = 0;
	string traceStr;

	// Get the Location Ids of the switches to which the original message was sent and record them
	vector<LocationId> sendLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
	for (UI32 i = 0; i < sendLocations.size(); i++)
	{
		swId = DcmToolKit::getMappedIdFromLocationId(sendLocations[i]);

		status = pWaveSendToClusterContext->getSendStatusForPhase1(sendLocations[i]);
		if (status == WAVE_MESSAGE_SUCCESS)
		{
			status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(sendLocations[i]);
			if (status == FRAMEWORK_SUCCESS)
			{
				status = pWaveSendToClusterContext->getCompletionStatusForPhase1(sendLocations[i]);
				if (status != WAVE_MESSAGE_SUCCESS)
				{
					trace (TRACE_LEVEL_ERROR, callerMethodName + string(": Backend operation failed on switch Id ") + swId + string(" plugin with error - ") + FrameworkToolKit::localize (status));
				}
				else
				{
					succSwIds.push_back(swId);
				}
			}
			else
			{
				trace (TRACE_LEVEL_ERROR, string("Framework error occured during delegation of backend operation to local plugin on switch Id '") + swId + string("' with reason code - ") + FrameworkToolKit::localize(status));
			}
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, string("Sending original local message to switch Id '") + swId + string("' failed with reason code - ") + FrameworkToolKit::localize (status));
		}

		if ((status != WAVE_MESSAGE_SUCCESS) && (status != FRAMEWORK_SUCCESS))
		{
			failedSwIds.push_back(swId);
			failedSwErrs.push_back(status);
		}
	}

	if (failedSwIds.size() > 0)
	{
		errorStr = DcmToolKit::getFormattedSwitchIDs(failedSwIds);
		traceStr = "Operation failed on switch(es) " + errorStr;
		traceStr += " : Please check logs on the individual switch(es) for more info.";
		trace (TRACE_LEVEL_ERROR, traceStr);
	}

	if (isPartialSuccessEnabled)
	{
		if (succSwIds.size() > 0)
		{
			errorStr += ". Succeeded on Node(es): " + DcmToolKit::getFormattedSwitchIDs(succSwIds) + ".";
		}
		for (UI32 i = 0; i < failedSwIds.size() ; i++)
		{
			errorStr += string("\n\trbridge-id ") + failedSwIds[i] + string(" reason: ") + FrameworkToolKit::localize(failedSwErrs[i]);
		}
	}
	else
	{
	// Record the Location Ids of the switches on which rollback operation failed
	failedSwIds.clear();
	vector<LocationId> rollbackLocations = pWaveSendToClusterContext->getLocationsToSendToForPhase2();
	bool rollbackErr = false;
	for (UI32 i = 0; i < rollbackLocations.size(); i++)
	{
		swId = DcmToolKit::getMappedIdFromLocationId(rollbackLocations[i]);

		status = pWaveSendToClusterContext->getSendStatusForPhase2(rollbackLocations[i]);
		if (status == WAVE_MESSAGE_SUCCESS)
		{
			status = pWaveSendToClusterContext->getFrameworkStatusForPhase2(rollbackLocations[i]);
			if (status == FRAMEWORK_SUCCESS)
			{
				status = pWaveSendToClusterContext->getCompletionStatusForPhase2(rollbackLocations[i]);
				if (status != WAVE_MESSAGE_SUCCESS)
				{
					if (!rollbackErr) rollbackErr = true;
					trace (TRACE_LEVEL_ERROR, callerMethodName + string(": Backend operation for ROLLBACK failed on switch Id ") + swId + string(" plugin with error - ") + FrameworkToolKit::localize (status));
				}
			}
			else
			{
				trace (TRACE_LEVEL_ERROR, string("Framework error occured during delegation of ROLLBACK operation to local plugin on switch Id '") + swId + string("' with reason code - ") + FrameworkToolKit::localize (status));
			}
		}
		else
		{
			trace (TRACE_LEVEL_ERROR, string("Sending ROLLBACK local message to switch Id '") + swId + string("' failed with reason code - ") + FrameworkToolKit::localize (status));
		}

		if (status != WAVE_MESSAGE_SUCCESS && status != FRAMEWORK_SUCCESS)
		{
			failedSwIds.push_back(swId);
		}
	}

	if ((failedSwIds.size() > 0) && rollbackErr)
	{
		traceStr = "ROLLBACK operation FAILED on switch(es) " + DcmToolKit::getFormattedSwitchIDs(failedSwIds);
		traceStr += " and are in INCONSISTENT state. Please check logs on the individual switches for more info and RESTORE THEM.";
		trace (TRACE_LEVEL_FATAL, traceStr);
	}
	} // if (!isPartialSuccessEnabled)

}



AAAAddUserContext::AAAAddUserContext (PrismMessage *pMessage, RoleManagedObject *pRole, const string &usrsOfRole, const string &password, const UI8 &encryptionLevel)
	: IP6AAASequencerContext (pMessage),
	m_pRole (pRole),
	m_usersOfRole (usrsOfRole),
	m_password (password),
	m_encryptionLevel (encryptionLevel)
{
}

AAAAddUserContext::~AAAAddUserContext()
{
	if (m_pRole != NULL)
	{
		delete m_pRole;
	}
}

RoleManagedObject* AAAAddUserContext::getRoleObj() const
{
	return (m_pRole);
}

string AAAAddUserContext::getUsrsOfRole() const
{
	return (m_usersOfRole);
}

string AAAAddUserContext::getPassword() const
{
	return (m_password);
}

UI8 AAAAddUserContext::getEncryptionLevel() const
{
	return (m_encryptionLevel);
}



AAAChangeUserContext::AAAChangeUserContext (PrismMessage *pMessage, UserManagedObject *pUser, RoleManagedObject *pRole, RoleManagedObject *pOldRole, const string &password, const UI8 &encryptionLevel)
	: IP6AAASequencerContext (pMessage),
	m_pUser (pUser),
	m_pRole (pRole),
	m_pOldRole (pOldRole),
	m_password (password),
	m_encryptionLevel (encryptionLevel)
{
}

AAAChangeUserContext::~AAAChangeUserContext ()
{
}

UserManagedObject* AAAChangeUserContext::getUserObj() const
{
	return (m_pUser);
}

RoleManagedObject* AAAChangeUserContext::getRoleObj() const
{
	return (m_pRole);
}

RoleManagedObject* AAAChangeUserContext::getOldRoleObj() const
{
	return (m_pOldRole);
}

string AAAChangeUserContext::getPassword() const
{
	return (m_password);
}

UI8 AAAChangeUserContext::getEncryptionLevel() const
{
	return (m_encryptionLevel);
}



AAADeleteUserContext::AAADeleteUserContext (PrismMessage *pMessage, UserManagedObject *pUser, RoleManagedObject *pRole)
	: IP6AAASequencerContext (pMessage),
	m_pUser (pUser),
	m_pRole (pRole)
{
}

AAADeleteUserContext::~AAADeleteUserContext ()
{
}

UserManagedObject* AAADeleteUserContext::getUserObj() const
{
	return (m_pUser);
}

RoleManagedObject* AAADeleteUserContext::getRoleObj() const
{
	return (m_pRole);
}



AAAAddRoleContext::AAAAddRoleContext (PrismMessage *pMessage)
	: IP6AAASequencerContext (pMessage)
{
}

AAAAddRoleContext::~AAAAddRoleContext ()
{
}



AAAChangeRoleContext::AAAChangeRoleContext (PrismMessage *pMessage, RoleManagedObject *pRole)
	: IP6AAASequencerContext (pMessage),
	m_pRole (pRole)
{
}

AAAChangeRoleContext::~AAAChangeRoleContext ()
{
}

RoleManagedObject* AAAChangeRoleContext::getRoleObj () const
{
	return (m_pRole);
}



AAADeleteRoleContext::AAADeleteRoleContext (PrismMessage *pMessage, RoleManagedObject *pRole)
	: IP6AAASequencerContext (pMessage),
	m_pRole (pRole)
{
}

AAADeleteRoleContext::~AAADeleteRoleContext ()
{
}

RoleManagedObject* AAADeleteRoleContext::getRoleObj () const
{
	return (m_pRole);
}


AAASetBannerContext::AAASetBannerContext(PrismMessage *pMessage, BannerManagedObject *pBanner)
	: IP6AAASequencerContext (pMessage),
	m_pBanner (pBanner)
{
}

AAASetBannerContext::~AAASetBannerContext()
{
}
BannerManagedObject* AAASetBannerContext::getBannerObj () const
{
	return (m_pBanner);
}

AAASetPasswordAttributesContext::AAASetPasswordAttributesContext(PrismMessage *pMessage, PasswordAttributesManagedObject *pPassword)
	: IP6AAASequencerContext (pMessage),
	m_pPassword (pPassword)
{
}
AAASetPasswordAttributesContext::~AAASetPasswordAttributesContext()
{
}
PasswordAttributesManagedObject* AAASetPasswordAttributesContext::getPasswordAttributesObj () const
{
	return (m_pPassword);
}


AAAAddRadiusHostContext::AAAAddRadiusHostContext (PrismMessage *pMessage, UI16 index)
	: IP6AAASequencerContext (pMessage),
	m_index (index)
{
}

AAAAddRadiusHostContext::~AAAAddRadiusHostContext ()
{
}

UI16 AAAAddRadiusHostContext::getIndex () const
{
	return (m_index);
}



AAAChangeRadiusHostContext::AAAChangeRadiusHostContext (PrismMessage *pMessage, RadiusHostManagedObject *pRadiusHost)
	: IP6AAASequencerContext (pMessage),
	m_pRadiusHost (pRadiusHost)
{
}

AAAChangeRadiusHostContext::~AAAChangeRadiusHostContext ()
{
}

RadiusHostManagedObject* AAAChangeRadiusHostContext::getRadiusHostObj () const
{
	return (m_pRadiusHost);
}



AAADeleteRadiusHostContext::AAADeleteRadiusHostContext (PrismMessage *pMessage, RadiusHostManagedObject *pRadiusHost, vector<RadiusHostManagedObject *> *radiusHostsForUpdatingIndex)
	: IP6AAASequencerContext (pMessage),
	m_pRadiusHost (pRadiusHost),
	m_pRadiusHostsForUpdatingIndex(radiusHostsForUpdatingIndex)
{
}

AAADeleteRadiusHostContext::~AAADeleteRadiusHostContext ()
{
}

RadiusHostManagedObject* AAADeleteRadiusHostContext::getRadiusHostObj () const
{
	return (m_pRadiusHost);
}

vector<RadiusHostManagedObject *>* AAADeleteRadiusHostContext::getRadiusHostsForUpdatingIndex() const
{
	return (m_pRadiusHostsForUpdatingIndex);
}



AAAAddTacacsHostContext::AAAAddTacacsHostContext (PrismMessage *pMessage, UI16 index)
	: IP6AAASequencerContext (pMessage),
	m_index (index)
{
}

AAAAddTacacsHostContext::~AAAAddTacacsHostContext ()
{
}

UI16 AAAAddTacacsHostContext::getIndex () const
{
	return (m_index);
}



AAAChangeTacacsHostContext::AAAChangeTacacsHostContext (PrismMessage *pMessage, TacacsHostManagedObject *pTacacsHost)
	: IP6AAASequencerContext (pMessage),
	m_pTacacsHost (pTacacsHost)
{
}

AAAChangeTacacsHostContext::~AAAChangeTacacsHostContext ()
{
}

TacacsHostManagedObject* AAAChangeTacacsHostContext::getTacacsHostObj () const
{
	return (m_pTacacsHost);
}



AAADeleteTacacsHostContext::AAADeleteTacacsHostContext (PrismMessage *pMessage, TacacsHostManagedObject *pTacacsHost, vector<TacacsHostManagedObject *> *tacacsHostsForUpdatingIndex)
	: IP6AAASequencerContext (pMessage),
	m_pTacacsHost (pTacacsHost),
	m_pTacacsHostsForUpdatingIndex(tacacsHostsForUpdatingIndex)
{
}

AAADeleteTacacsHostContext::~AAADeleteTacacsHostContext ()
{
}

TacacsHostManagedObject* AAADeleteTacacsHostContext::getTacacsHostObj () const
{
	return (m_pTacacsHost);
}

vector<TacacsHostManagedObject *>* AAADeleteTacacsHostContext::getTacacsHostsForUpdatingIndex() const
{
	return (m_pTacacsHostsForUpdatingIndex);
}

AAAAddLDAPHostContext::AAAAddLDAPHostContext (PrismMessage *pMessage, UI16 index)
	: IP6AAASequencerContext (pMessage),
	m_index (index)
{
}

AAAAddLDAPHostContext::~AAAAddLDAPHostContext ()
{
}

UI16 AAAAddLDAPHostContext::getIndex () const
{
	return (m_index);
}



AAAChangeLDAPHostContext::AAAChangeLDAPHostContext (PrismMessage *pMessage, LDAPHostManagedObject *pLDAPHost)
	: IP6AAASequencerContext (pMessage),
	m_pLDAPHost (pLDAPHost)
{
}

AAAChangeLDAPHostContext::~AAAChangeLDAPHostContext ()
{
}

LDAPHostManagedObject* AAAChangeLDAPHostContext::getLDAPHostObj () const
{
	return (m_pLDAPHost);
}



AAADeleteLDAPHostContext::AAADeleteLDAPHostContext (PrismMessage *pMessage, LDAPHostManagedObject *pLDAPHost, vector<LDAPHostManagedObject *> *ldapHostsForUpdatingIndex)
	: IP6AAASequencerContext (pMessage),
	m_pLDAPHost (pLDAPHost),
	m_pLDAPHostsForUpdatingIndex(ldapHostsForUpdatingIndex)
{
}

AAADeleteLDAPHostContext::~AAADeleteLDAPHostContext ()
{
}

LDAPHostManagedObject* AAADeleteLDAPHostContext::getLDAPHostObj () const
{
	return (m_pLDAPHost);
}

vector<LDAPHostManagedObject *>* AAADeleteLDAPHostContext::getLDAPHostsForUpdatingIndex() const
{
	return (m_pLDAPHostsForUpdatingIndex);
}


AAAAuthLoginModeContext::AAAAuthLoginModeContext (PrismMessage *pMessage, AuthLoginModeManagedObject *pAuthLoginMode, const AAAServerType &first, const AAAServerType &second)
	: IP6AAASequencerContext (pMessage),
	m_pAuthLoginMode (pAuthLoginMode),
	m_first (first),
	m_second (second)
{
}

AAAAuthLoginModeContext::~AAAAuthLoginModeContext ()
{
}

AuthLoginModeManagedObject* AAAAuthLoginModeContext::getAuthLoginModeObj () const
{
	return (m_pAuthLoginMode);
}

AAAServerType AAAAuthLoginModeContext::getFirst () const
{
	return (m_first);
}

AAAServerType AAAAuthLoginModeContext::getSecond () const
{
	return (m_second);
}

AAACleanCacheContext::AAACleanCacheContext (PrismMessage *pMessage)
	: IP6AAASequencerContext (pMessage)
{
}

AAACleanCacheContext::~AAACleanCacheContext ()
{
}

AAAAddMapRoleContext::AAAAddMapRoleContext (PrismMessage *pMessage)
	: IP6AAASequencerContext (pMessage)
{
}

AAAAddMapRoleContext::~AAAAddMapRoleContext ()
{
}



AAAChangeMapRoleContext::AAAChangeMapRoleContext (PrismMessage *pMessage, MapRoleManagedObject *pMapRole)
	: IP6AAASequencerContext (pMessage),
	m_pMapRole (pMapRole)
{
}

AAAChangeMapRoleContext::~AAAChangeMapRoleContext ()
{
}

MapRoleManagedObject* AAAChangeMapRoleContext::getMapRoleObj () const
{
	return (m_pMapRole);
}



AAADeleteMapRoleContext::AAADeleteMapRoleContext (PrismMessage *pMessage, MapRoleManagedObject *pMapRole)
	: IP6AAASequencerContext (pMessage),
	m_pMapRole (pMapRole)
{
}

AAADeleteMapRoleContext::~AAADeleteMapRoleContext ()
{
}

MapRoleManagedObject* AAADeleteMapRoleContext::getMapRoleObj () const
{
	return (m_pMapRole);
}

AAAIpAclRuleContext::AAAIpAclRuleContext (PrismMessage *pMessage)
	: IP6AAASequencerContext (pMessage)
{
}

AAAIpAclRuleContext::~AAAIpAclRuleContext ()
{
}

/*
AAAStdAclRuleContext::AAAStdAclRuleContext (PrismMessage *pMessage)
	: IP6AAASequencerContext (pMessage)
{
}

AAAStdAclRuleContext::~AAAStdAclRuleContext ()
{
}

AAAExtAclRuleContext::AAAExtAclRuleContext (PrismMessage *pMessage)
	: IP6AAASequencerContext (pMessage)
{
}

AAAExtAclRuleContext::~AAAExtAclRuleContext ()
{
}
*/

AAACertutilContext::AAACertutilContext (PrismMessage *pMessage)
	: IP6AAASequencerContext (pMessage)
{
}

AAACertutilContext::~AAACertutilContext ()
{
}
}
#endif
