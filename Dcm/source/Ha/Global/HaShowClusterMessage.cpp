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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   December, 2012                                                        *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "Ha/Global/HaShowGlobalObjectManager.h"
#include "Ha/Global/HaShowClusterMessage.h"
#include "Ha/Local/HaOpTypes.h"

namespace DcmNs
{

    HaShowClusterMessage::HaShowClusterMessage ()
        : PrismMessage (HaShowGlobalObjectManager::getPrismServiceId (), HASHOWACTION)
    {
    }

    HaShowClusterMessage::HaShowClusterMessage (const string &command)
        : PrismMessage (HaShowGlobalObjectManager::getPrismServiceId (), HASHOWACTION),
        m_command    (command)
    {
    }

    HaShowClusterMessage::~HaShowClusterMessage ()
    {
    }

    void  HaShowClusterMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString   (&m_command,    "command"));
        addSerializableAttribute (new AttributeString   (&m_data,       "data"));
        addSerializableAttribute (new AttributeUI32     (&m_mappedId,   "mappedId"));
    }

    void  HaShowClusterMessage::setCmd(const string &command)
    {
        m_command  =  command;
    }

    string  HaShowClusterMessage::getCmd() const
    {
        return (m_command);
    }

    void  HaShowClusterMessage::setData(const string &data)
    {
        m_data  =  data;
    }

    string  HaShowClusterMessage::getData() const
    {
        return (m_data);
    }

	UI32 HaShowClusterMessage::getMappedId() const
	{
		return (m_mappedId);
	}

	void  HaShowClusterMessage::setMappedId(const  UI32 &mappedId)
	{
		m_mappedId = mappedId;
	}
}
