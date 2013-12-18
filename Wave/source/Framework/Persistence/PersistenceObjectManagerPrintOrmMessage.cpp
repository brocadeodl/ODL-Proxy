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

/**************************************************************************************
 *  @file : PersistenceObjectManagerPrintOrmMessage.cpp                               *
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.                          *
 *   All rights reserved.                                                             *
 *   Description : Implements a Persistence OM handled message                        * 
 *   Author : Brian Adaniya                                                           *
 *   Date   : 11/07/2011                                                              *
 **************************************************************************************/

#include "Framework/Persistence/PersistenceObjectManagerPrintOrmMessage.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"

namespace WaveNs
{

PersistenceObjectManagerPrintOrmMessage::PersistenceObjectManagerPrintOrmMessage ()
    : ManagementInterfaceMessage (PersistenceObjectManager::getPrismServiceName (), PERSISTENCE_OBJECT_MANAGER_PRINT_ORM),
    m_ormInfo   ("")
{
}

PersistenceObjectManagerPrintOrmMessage::PersistenceObjectManagerPrintOrmMessage (const vector<string> &arguments)
    : ManagementInterfaceMessage (PersistenceObjectManager::getPrismServiceName (), PERSISTENCE_OBJECT_MANAGER_PRINT_ORM),
    m_ormArgs   (arguments),
    m_ormInfo   ("")
{
}

PersistenceObjectManagerPrintOrmMessage::~PersistenceObjectManagerPrintOrmMessage ()
{
}

void PersistenceObjectManagerPrintOrmMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeStringVector (&m_ormArgs, "ormArgs"));
     addSerializableAttribute (new AttributeString       (&m_ormInfo, "ormInfo"));
}

const vector<string> & PersistenceObjectManagerPrintOrmMessage::getOrmArgs () const
{
    return (m_ormArgs);
}

string PersistenceObjectManagerPrintOrmMessage::getOrmInfo () const
{
    return (m_ormInfo);
}

void PersistenceObjectManagerPrintOrmMessage::setOrmInfo (const string &ormInfo)
{
    m_ormInfo = ormInfo;
}

}
