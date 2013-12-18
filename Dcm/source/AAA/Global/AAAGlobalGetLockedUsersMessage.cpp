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

#include "AAA/Global/AAAGlobalGetLockedUsersMessage.h"
#include "AAA/Global/AAAGlobalObjectManager.h"
#include "AAA/Global/AAAGlobalTypes.h"

namespace DcmNs
{

	AAAGlobalGetLockedUsersMessage::AAAGlobalGetLockedUsersMessage ()
		: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALGETLOCKEDUSERS)
	{
	}

	AAAGlobalGetLockedUsersMessage::AAAGlobalGetLockedUsersMessage (const vector<string> &userName,const vector<UI32> &switchid)
		: PrismMessage (AAAGlobalObjectManager::getPrismServiceId (), AAAGLOBALGETLOCKEDUSERS),
		m_vUserName    (userName),
		m_vSwitchid    (switchid)
	{
	}

	AAAGlobalGetLockedUsersMessage::~AAAGlobalGetLockedUsersMessage ()
	{
	}

	void  AAAGlobalGetLockedUsersMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeStringVector(&m_vUserName,"vUserName"));
		addSerializableAttribute (new AttributeUI32Vector(&m_vSwitchid,"vSwitchid"));
	}

	UI32  AAAGlobalGetLockedUsersMessage::getNumVectors() const
	{
		return (m_vUserName.size());
	}

	void AAAGlobalGetLockedUsersMessage::clearNumVectors()
	{
		m_vUserName.clear();
		m_vSwitchid.clear();
	}

	void  AAAGlobalGetLockedUsersMessage::setLockedUser(const string &userName, const UI32 &switchid)
	{
		m_vUserName.push_back(userName);
		m_vSwitchid.push_back(switchid);
	}

	bool  AAAGlobalGetLockedUsersMessage::getLockedUser(const UI32 &index, string &userName, UI32 &switchid)
	{
		if (index >= m_vUserName.size())
		{
			return false;
		}

		userName = m_vUserName[index];
		switchid = m_vSwitchid[index];

		return true;
	}
}
