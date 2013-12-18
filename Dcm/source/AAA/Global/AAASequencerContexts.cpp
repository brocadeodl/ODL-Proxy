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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Charanjith Reddy Kunduru                                    *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "DcmCore/DcmToolKit.h"

#include "AAA/Global/AAASequencerContexts.h"

using namespace WaveNs;

namespace DcmNs
{

AAASequencerContext::AAASequencerContext (PrismMessage *pPrismMessage)
	: m_pPrismMsg (pPrismMessage)
{
}

AAASequencerContext::~AAASequencerContext ()
{
}

PrismMessage* AAASequencerContext::getPPrismMessage() const
{
	return (m_pPrismMsg);
}

void AAASequencerContext::traceSendToClusterErrors(WaveSendToClusterContext *pWaveSendToClusterContext, const string &callerMethodName, string &errorStr, const bool &isPartialSuccessEnabled)
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage)
{
}

AAAAddRoleContext::~AAAAddRoleContext ()
{
}



AAAChangeRoleContext::AAAChangeRoleContext (PrismMessage *pMessage, RoleManagedObject *pRole)
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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

#if 0
TelnetServerConfigContext::TelnetServerConfigContext(PrismMessage *pMessage, TelnetServerGlobalManagedObject *pTelnetServerGlobalMO)
	: AAASequencerContext (pMessage),
	m_pTelnetServerGlobalMO (pTelnetServerGlobalMO)
{
}

TelnetServerConfigContext::~TelnetServerConfigContext()
{
}

TelnetServerGlobalManagedObject* TelnetServerConfigContext::getTelnetServerGlobalMO () const
{
	return (m_pTelnetServerGlobalMO);
}

SSHServerConfigContext::SSHServerConfigContext(PrismMessage *pMessage, SSHServerGlobalManagedObject *pSSHServerGlobalMO)
	: AAASequencerContext (pMessage),
	m_pSSHServerGlobalMO (pSSHServerGlobalMO)
{
}

SSHServerGlobalManagedObject* SSHServerConfigContext::getSSHServerGlobalMO () const
{
	return (m_pSSHServerGlobalMO);
}

SSHServerConfigContext::~SSHServerConfigContext()
{
}

SSHServerListAddContext::SSHServerListAddContext(PrismMessage *pMessage)
	: AAASequencerContext (pMessage)
{
}

SSHServerListAddContext::~SSHServerListAddContext()
{
}

SSHServerListDeleteContext::SSHServerListDeleteContext(PrismMessage *pMessage, SSHServerListManagedObject *pSSHServerListMO)
	: AAASequencerContext (pMessage),
	m_pSSHServerListMO (pSSHServerListMO)
{
}

SSHServerListManagedObject* SSHServerListDeleteContext::getSSHServerListMO () const
{
	return (m_pSSHServerListMO);
}

SSHServerListDeleteContext::~SSHServerListDeleteContext()
{
}
#endif

AAASetPasswordAttributesContext::AAASetPasswordAttributesContext(PrismMessage *pMessage, PasswordAttributesManagedObject *pPassword)
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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

AAAChangeExecAccountingContext::AAAChangeExecAccountingContext (PrismMessage *pMessage, ExecAccountingManagedObject *pExecAcc)
        : AAASequencerContext (pMessage),        m_pExecAcc (pExecAcc)
{       
}       
AAAChangeExecAccountingContext::~AAAChangeExecAccountingContext ()
{
}
ExecAccountingManagedObject* AAAChangeExecAccountingContext::getExecAccObj() const
{
    return (m_pExecAcc);
}

AAAChangeCmdAccountingContext::AAAChangeCmdAccountingContext (PrismMessage *pMessage, CmdAccountingManagedObject *pCmdAcc)
        : AAASequencerContext (pMessage),
        m_pCmdAcc (pCmdAcc)
{

}
AAAChangeCmdAccountingContext::~AAAChangeCmdAccountingContext ()
{
}

CmdAccountingManagedObject* AAAChangeCmdAccountingContext::getCmdAccObj() const
{
    return (m_pCmdAcc);
}

AAACleanCacheContext::AAACleanCacheContext (PrismMessage *pMessage)
	: AAASequencerContext (pMessage)
{
}

AAACleanCacheContext::~AAACleanCacheContext ()
{
}

AAAAddMapRoleContext::AAAAddMapRoleContext (PrismMessage *pMessage)
	: AAASequencerContext (pMessage)
{
}

AAAAddMapRoleContext::~AAAAddMapRoleContext ()
{
}



AAAChangeMapRoleContext::AAAChangeMapRoleContext (PrismMessage *pMessage, MapRoleManagedObject *pMapRole)
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage),
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
	: AAASequencerContext (pMessage)
{
}

AAAIpAclRuleContext::~AAAIpAclRuleContext ()
{
}

AAAStdAclRuleContext::AAAStdAclRuleContext (PrismMessage *pMessage)
	: AAASequencerContext (pMessage)
{
}

AAAStdAclRuleContext::~AAAStdAclRuleContext ()
{
}

AAAExtAclRuleContext::AAAExtAclRuleContext (PrismMessage *pMessage)
	: AAASequencerContext (pMessage)
{
}

AAAExtAclRuleContext::~AAAExtAclRuleContext ()
{
}

AAACertutilContext::AAACertutilContext (PrismMessage *pMessage)
	: AAASequencerContext (pMessage)
{
}

AAACertutilContext::~AAACertutilContext ()
{
}
}
