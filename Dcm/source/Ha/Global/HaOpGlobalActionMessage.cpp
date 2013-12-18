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

#include "Ha/Global/HaOpGlobalObjectManager.h"
#include "Ha/Global/HaOpGlobalActionMessage.h"
#include "Ha/Local/HaOpTypes.h"

namespace DcmNs
{

    HaOpGlobalActionMessage::HaOpGlobalActionMessage ()
        : PrismMessage (HaOpGlobalObjectManager::getPrismServiceId (), HAOPACTION)
    {
    }

    HaOpGlobalActionMessage::HaOpGlobalActionMessage (const string &command)
        : PrismMessage (HaOpGlobalObjectManager::getPrismServiceId (), HAOPACTION),
        m_command    (command)
    {
    }

    HaOpGlobalActionMessage::~HaOpGlobalActionMessage ()
    {
    }

    void  HaOpGlobalActionMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_command, "command"));
        addSerializableAttribute (new AttributeString(&m_result,  "result"));
        addSerializableAttribute (new AttributeUI32(&m_mappedId,  "mappedId"));
    }

    void  HaOpGlobalActionMessage::setCommand(const string &command)
    {
        m_command  =  command;
    }

    string  HaOpGlobalActionMessage::getCommand() const
    {
        return (m_command);
    }

    void  HaOpGlobalActionMessage::setResult(const string &result)
    {
        m_result  =  result;
    }

    string  HaOpGlobalActionMessage::getResult() const
    {
        return (m_result);
    }

	UI32 HaOpGlobalActionMessage::getMappedId() const
	{
		return (m_mappedId);
	}

	void  HaOpGlobalActionMessage::setMappedId(const  UI32 &mappedId)
	{
		m_mappedId = mappedId;
	}
}
