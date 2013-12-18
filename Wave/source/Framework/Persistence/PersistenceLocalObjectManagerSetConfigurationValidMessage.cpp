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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Anil ChannaveeraSetty                                        *
 ***************************************************************************/

#include "Framework/Persistence/PersistenceLocalObjectManagerSetConfigurationValidMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"

namespace WaveNs
{

PersistenceLocalObjectManagerSetConfigurationValidMessage::PersistenceLocalObjectManagerSetConfigurationValidMessage (const bool &validity, const string &schemaFileName)
    : ManagementInterfaceMessage (PersistenceLocalObjectManager::getServiceName (), PERSISTENCE_SET_CONFIGURATION_VALID),
     m_isConfigurationValid (validity),
     m_schemaFile (schemaFileName)
{
}

PersistenceLocalObjectManagerSetConfigurationValidMessage::PersistenceLocalObjectManagerSetConfigurationValidMessage ()
    : ManagementInterfaceMessage (PersistenceLocalObjectManager::getServiceName (), PERSISTENCE_SET_CONFIGURATION_VALID)
{
}

PersistenceLocalObjectManagerSetConfigurationValidMessage::~PersistenceLocalObjectManagerSetConfigurationValidMessage ()
{
}

void PersistenceLocalObjectManagerSetConfigurationValidMessage::setupAttributesForSerialization ()
{   
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool   (&m_isConfigurationValid,    "isConfigurationValid"));
     addSerializableAttribute (new AttributeString (&m_schemaFile,              "schemaFile"));
}

bool PersistenceLocalObjectManagerSetConfigurationValidMessage::getConfigurationValidity () const
{   
    return (m_isConfigurationValid);
}

void PersistenceLocalObjectManagerSetConfigurationValidMessage::setConfigurationValidity (const bool &validity)
{   
    m_isConfigurationValid = validity;
}

string PersistenceLocalObjectManagerSetConfigurationValidMessage::getSchemaFile () const
{
    return (m_schemaFile);
}

void PersistenceLocalObjectManagerSetConfigurationValidMessage::setSchemaFile (const string &schemaFileName)
{
    m_schemaFile = schemaFileName;
}

}
