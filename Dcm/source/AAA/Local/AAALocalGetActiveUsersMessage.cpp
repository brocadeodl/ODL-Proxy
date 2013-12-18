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
 *   Author : noku                                                     *
 **************************************************************************/

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalGetActiveUsersMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

	AAALocalGetActiveUsersMessage::AAALocalGetActiveUsersMessage ()
		: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALGETACTIVEUSERS)
	{
	}

	AAALocalGetActiveUsersMessage::AAALocalGetActiveUsersMessage (
		const vector<string> &userName, const vector<string> &hostName,
		const vector<UI32> &sessionId, const vector<string> &ipAddr,
		const vector<string> &deviceName, const vector<UI32> &pid,
		const vector<DateTime> &timeOfLog, const vector<string> &role)
		: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALGETACTIVEUSERS),
		m_vUserName    (userName),
		m_vHostName    (hostName),
		m_vSessionId   (sessionId),
		m_vIpAddr      (ipAddr),
		m_vDeviceName  (deviceName),
		m_vPid         (pid),
		m_vTimeOfLog   (timeOfLog),
		m_vRole        (role)
	{
	}

	AAALocalGetActiveUsersMessage::~AAALocalGetActiveUsersMessage ()
	{
	}

	void  AAALocalGetActiveUsersMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeStringVector(&m_vUserName,"vUserName"));
		addSerializableAttribute (new AttributeStringVector(&m_vHostName,"vHostName"));
		addSerializableAttribute (new AttributeUI32Vector(&m_vSessionId,"vSessionId"));
		addSerializableAttribute (new AttributeStringVector(&m_vIpAddr,"vIpAddr"));
		addSerializableAttribute (new AttributeStringVector(&m_vDeviceName,"vDeviceName"));
		addSerializableAttribute (new AttributeUI32Vector(&m_vPid,"vPid"));
		addSerializableAttribute (new AttributeDateTimeVector(&m_vTimeOfLog,"vTimeOfLog"));
		addSerializableAttribute (new AttributeStringVector(&m_vRole,"vRole"));
	}

	UI32  AAALocalGetActiveUsersMessage::getNumVectors() const
	{
		return (m_vUserName.size());
	}

	void AAALocalGetActiveUsersMessage::clearNumVectors()
	{
		m_vUserName.clear();
		m_vHostName.clear();
		m_vSessionId.clear();
		m_vIpAddr.clear();
		m_vDeviceName.clear();
		m_vPid.clear();
		m_vTimeOfLog.clear();
	}

	void  AAALocalGetActiveUsersMessage::setActiveUser(const string &userName,
		const string &hostName, const UI32 &sessionId,
		const string &ipAddr, const string &deviceName,
		const UI32 &pid, const DateTime &timeOfLog, const string &role)
	{
		m_vUserName.push_back(userName);
		m_vHostName.push_back(hostName);
		m_vSessionId.push_back(sessionId);
		m_vIpAddr.push_back(ipAddr);
		m_vDeviceName.push_back(deviceName);
		m_vPid.push_back(pid);
		m_vTimeOfLog.push_back(timeOfLog);
		m_vRole.push_back(role);
	}

	bool  AAALocalGetActiveUsersMessage::getActiveUser(const UI32 &index,
		string &userName, string &hostName, UI32 &sessionId, string &ipAddr,
		string &deviceName, UI32 &pid, DateTime &timeOfLog, string &role)
	{
		if (index >= m_vUserName.size())
		{
			return false;
		}

		userName = m_vUserName[index];
		hostName = m_vHostName[index];
		sessionId = m_vSessionId[index];
		ipAddr = m_vIpAddr[index];
		deviceName = m_vDeviceName[index];
		pid = m_vPid[index];
		timeOfLog = m_vTimeOfLog[index];
		role = m_vRole[index];

		return true;
	}

}
