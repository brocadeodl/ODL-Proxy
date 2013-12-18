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
#include "Ha/Local/HaOpActionMessage.h"
#include "Ha/Local/HaOpTypes.h"

namespace DcmNs
{

    HaOpActionMessage::HaOpActionMessage ()
        : PrismMessage (HaOpLocalObjectManager::getPrismServiceId (), HAOPACTION)
    {
    }

    HaOpActionMessage::HaOpActionMessage (const string &command)
        : PrismMessage (HaOpLocalObjectManager::getPrismServiceId (), HAOPACTION),
        m_command    (command)
    {
    }

    HaOpActionMessage::~HaOpActionMessage ()
    {
    }

    void  HaOpActionMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
         addSerializableAttribute (new AttributeString  (&m_command,    "command"));
         addSerializableAttribute (new AttributeString  (&m_result,     "result"));
         addSerializableAttribute (new AttributeUI32    (&m_rbridge_id, "rbridge_id"));
         addSerializableAttribute (new AttributeUI32    (&m_flag, "flag"));
    }

    void  HaOpActionMessage::setCommand(const string &command)
    {
        m_command  =  command;
    }

    string  HaOpActionMessage::getCommand() const
    {
        return (m_command);
    }

    void  HaOpActionMessage::setResult(const string &result)
    {
        m_result  =  result;
    }

    string  HaOpActionMessage::getResult() const
    {
        return (m_result);
    }

    void  HaOpActionMessage::setRbridgeId(const UI32 &rbridge_id)
    {
        m_rbridge_id  =  rbridge_id;
    }
 
    UI32  HaOpActionMessage::getRbridgeId() const
    {
        return (m_rbridge_id);
    }

    void  HaOpActionMessage::setFlag(const UI32 &flag)
    {
        m_flag  =  flag;
    }
 
    UI32  HaOpActionMessage::getFlag() const
    {
        return (m_flag);
    }
}
