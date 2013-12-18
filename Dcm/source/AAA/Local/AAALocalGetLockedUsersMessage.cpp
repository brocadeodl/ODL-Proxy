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
#include "AAA/Local/AAALocalGetLockedUsersMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

	AAALocalGetLockedUsersMessage::AAALocalGetLockedUsersMessage ()
		: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALGETLOCKEDUSERS)
	{
	}

	AAALocalGetLockedUsersMessage::AAALocalGetLockedUsersMessage (const vector<string> &userName)
		: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALGETLOCKEDUSERS),
		m_vUserName    (userName)
	{
	}

	AAALocalGetLockedUsersMessage::~AAALocalGetLockedUsersMessage ()
	{
	}

	void  AAALocalGetLockedUsersMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeStringVector(&m_vUserName,"vUserName"));
	}

	UI32  AAALocalGetLockedUsersMessage::getNumVectors() const
	{
		return (m_vUserName.size());
	}

	void AAALocalGetLockedUsersMessage::clearNumVectors()
	{
		m_vUserName.clear();
	}

	void  AAALocalGetLockedUsersMessage::setLockedUser(const string &userName)
	{
		m_vUserName.push_back(userName);
	}

	bool  AAALocalGetLockedUsersMessage::getLockedUser(const UI32 &index, string &userName)
	{
		if (index >= m_vUserName.size())
		{
			return false;
		}

		userName = m_vUserName[index];

		return true;
	}
}
