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
 *   Author : Kumar Ghanta                                                 *
 **************************************************************************/

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/AAALocalCleanCacheMessage.h"
#include "AAA/Local/AAALocalTypes.h"

namespace DcmNs
{

AAALocalCleanCacheMessage::AAALocalCleanCacheMessage ()
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCLEANCACHE)
{
}

AAALocalCleanCacheMessage::AAALocalCleanCacheMessage (const string &caller, const int &defaultPasswdSync, const vector<string> &path)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCLEANCACHE),
	m_callerOp (caller),
	m_defaultPasswdSync (defaultPasswdSync),
    m_path    (path)
{
}

#if 0
AAALocalCleanCacheMessage::AAALocalCleanCacheMessage (const string &caller, const int &defaultPasswdSync)
	: PrismMessage (AAALocalLocalObjectManager::getPrismServiceId (), AAALOCALCLEANCACHE),
	m_callerOp (caller),
	m_defaultPasswdSync (defaultPasswdSync)

{
}
#endif

AAALocalCleanCacheMessage::~AAALocalCleanCacheMessage ()
{
}

void AAALocalCleanCacheMessage::setupAttributesForSerialization()
{
	PrismMessage::setupAttributesForSerialization ();
	addSerializableAttribute (new AttributeString(&m_callerOp, "caller"));
	addSerializableAttribute (new AttributeSI32(&m_defaultPasswdSync, "defaultPasswdSync"));
	addSerializableAttribute (new AttributeStringVector(&m_path,"path"));
}

string AAALocalCleanCacheMessage::getCaller() const
{
	return (m_callerOp);
}

int AAALocalCleanCacheMessage::getDefaultPasswdSync() const
{
	return (m_defaultPasswdSync);
}
void AAALocalCleanCacheMessage::setPath(const vector<string> &path)
{
	m_path = path;
}

vector<string> AAALocalCleanCacheMessage::getPath() const 
{
	return (m_path);	
}
}
