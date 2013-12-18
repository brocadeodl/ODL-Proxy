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
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerGetDbAccessDetailsMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Attributes/Attributes.h"

namespace WaveNs
{

DatabaseObjectManagerGetDbAccessDetailsMessage::DatabaseObjectManagerGetDbAccessDetailsMessage ()
    : ManagementInterfaceMessage (DatabaseObjectManager::getClassName (), DATABASE_OBJECT_MANAGER_GET_DB_ACCESS_DETAILS),
      m_port (0)
{
}

DatabaseObjectManagerGetDbAccessDetailsMessage::~DatabaseObjectManagerGetDbAccessDetailsMessage ()
{
}

void DatabaseObjectManagerGetDbAccessDetailsMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    // This class Specific Attributes below

    addSerializableAttribute (new AttributeString (&m_databaseName, "databaseName"));
    addSerializableAttribute (new AttributeUI32   (&m_port,         "port"));
}

string DatabaseObjectManagerGetDbAccessDetailsMessage::getDatabaseName () const
{
    return (m_databaseName);
}

void DatabaseObjectManagerGetDbAccessDetailsMessage::setDatabaseName (const string &databaseName)
{
    m_databaseName = databaseName;
}

UI32 DatabaseObjectManagerGetDbAccessDetailsMessage::getPort () const
{
    return (m_port);
}

void DatabaseObjectManagerGetDbAccessDetailsMessage::setPort (const UI32 &port)
{
    m_port = port;
}

}
