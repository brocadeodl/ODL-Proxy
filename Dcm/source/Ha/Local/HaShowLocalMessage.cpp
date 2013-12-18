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
 *   October, 2011                                                         *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "Ha/Local/HaOpLocalObjectManager.h"
#include "Ha/Local/HaShowLocalMessage.h"
#include "Ha/Local/HaOpTypes.h"

namespace DcmNs
{

    HaShowLocalMessage::HaShowLocalMessage ()
        : PrismMessage (HaOpLocalObjectManager::getPrismServiceId (), HASHOWACTION)
    {
    }

    HaShowLocalMessage::HaShowLocalMessage (const string &command)
        : PrismMessage (HaOpLocalObjectManager::getPrismServiceId (), HASHOWACTION),
        m_command    (command)
    {
    }

    HaShowLocalMessage::~HaShowLocalMessage ()
    {
    }

    void  HaShowLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString   (&m_command,    "command"));
        addSerializableAttribute (new AttributeString   (&m_data,       "data"));
        addSerializableAttribute (new AttributeUI32     (&m_mappedId,   "mappedId"));
    }

    void  HaShowLocalMessage::setCmd(const string &command)
    {
        m_command  =  command;
    }

    string  HaShowLocalMessage::getCmd() const
    {
        return (m_command);
    }

    void  HaShowLocalMessage::setData(const string &data)
    {
        m_data  =  data;
    }

    string  HaShowLocalMessage::getData() const
    {
        return (m_data);
    }

    void  HaShowLocalMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }
 
    UI32  HaShowLocalMessage::getMappedId() const
    {
        return (m_mappedId);
    }
}
