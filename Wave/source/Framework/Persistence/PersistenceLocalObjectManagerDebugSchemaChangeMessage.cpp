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
 *  @file : PersistenceLocalObjectManagerDebugSchemaChangeMessage.cpp                               *
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.                          *
 *   All rights reserved.                                                             *
 *   Description : Implements a Persistence OM handled message                        * 
 *   Author : Pritee Ghosh                                                            *
 *   Date   : 12/03/2011                                                              *
 **************************************************************************************/

#include "Framework/Persistence/PersistenceLocalObjectManagerDebugSchemaChangeMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"

namespace WaveNs
{

PersistenceLocalObjectManagerDebugSchemaChangeMessage::PersistenceLocalObjectManagerDebugSchemaChangeMessage ()
    : ManagementInterfaceMessage (PersistenceLocalObjectManager::getServiceName (), PERSISTENCE_DEBUG_SCHEMA_CHANGE)
{
}

PersistenceLocalObjectManagerDebugSchemaChangeMessage::PersistenceLocalObjectManagerDebugSchemaChangeMessage (const vector<string> &arguments)
    : ManagementInterfaceMessage (PersistenceLocalObjectManager::getServiceName (), PERSISTENCE_DEBUG_SCHEMA_CHANGE),
    m_cmdArgs     (arguments),
    m_resultPrint ("")
{
}   

PersistenceLocalObjectManagerDebugSchemaChangeMessage::~PersistenceLocalObjectManagerDebugSchemaChangeMessage ()
{
}

void PersistenceLocalObjectManagerDebugSchemaChangeMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeStringVector (&m_cmdArgs,       "cmdArgs"));
     addSerializableAttribute (new AttributeString       (&m_resultPrint,   "resultPrint"));
}

const vector<string> & PersistenceLocalObjectManagerDebugSchemaChangeMessage::getCmdArgs () const
{
    return (m_cmdArgs);
}

string PersistenceLocalObjectManagerDebugSchemaChangeMessage::getResultPrint () const
{
    return (m_resultPrint);
}

void PersistenceLocalObjectManagerDebugSchemaChangeMessage::setResultPrint (const string &result)
{
    m_resultPrint = result;
}

}
