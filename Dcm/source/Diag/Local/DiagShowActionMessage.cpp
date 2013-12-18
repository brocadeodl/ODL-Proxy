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
 *   Author : Subhani Shaik                                                *
 **************************************************************************/

#include "Diag/Local/DiagShowLocalObjectManager.h"
#include "Diag/Local/DiagShowActionMessage.h"
#include "Diag/Local/DiagLocalTypes.h"

namespace DcmNs
{

    DiagShowActionMessage::DiagShowActionMessage ()
        : PrismMessage (DiagShowLocalObjectManager::getPrismServiceId (), DIAGSHOWACTION)
    {
    }

    DiagShowActionMessage::DiagShowActionMessage (const string &command)
        : PrismMessage (DiagShowLocalObjectManager::getPrismServiceId (), DIAGSHOWACTION),
        m_command    (command)
    {
    }

    DiagShowActionMessage::~DiagShowActionMessage ()
    {
    }

    void  DiagShowActionMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_command,"command"));
        addSerializableAttribute (new AttributeString(&m_result,"result"));
    }

    void  DiagShowActionMessage::setCommand(const string &command)
    {
        m_command  =  command;
    }

    string  DiagShowActionMessage::getCommand() const
    {
        return (m_command);
    }

    void  DiagShowActionMessage::setResult(const string &result)
    {
        m_result  =  result;
    }

    string  DiagShowActionMessage::getResult() const
    {
        return (m_result);
    }

}
