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

#include "Diag/Local/DiagTestsLocalObjectManager.h"
#include "Diag/Local/DiagTestsActionMessage.h"
#include "Diag/Local/DiagLocalTypes.h"

namespace DcmNs
{

    DiagTestsActionMessage::DiagTestsActionMessage ()
        : PrismMessage (DiagTestsLocalObjectManager::getPrismServiceId (), DIAGTESTSACTION)
    {
    }

    DiagTestsActionMessage::DiagTestsActionMessage (const string &command)
        : PrismMessage (DiagTestsLocalObjectManager::getPrismServiceId (), DIAGTESTSACTION),
        m_command    (command)
    {
    }

    DiagTestsActionMessage::~DiagTestsActionMessage ()
    {
    }

    void  DiagTestsActionMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_command,"command"));
        addSerializableAttribute (new AttributeString(&m_result,"result"));
    }

    void  DiagTestsActionMessage::setCommand(const string &command)
    {
        m_command  =  command;
    }

    string  DiagTestsActionMessage::getCommand() const
    {
        return (m_command);
    }

    void  DiagTestsActionMessage::setResult(const string &result)
    {
        m_result  =  result;
    }

    string  DiagTestsActionMessage::getResult() const
    {
        return (m_result);
    }

}
