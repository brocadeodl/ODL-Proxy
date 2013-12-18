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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerRestoreMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"

namespace WaveNs
{

DatabaseObjectManagerRestoreMessage::DatabaseObjectManagerRestoreMessage (const string &fileToRestoreFrom)
    : PrismMessage (DatabaseObjectManager::getPrismServiceId (), DATABASE_OBJECT_MANAGER_RESTORE),
      m_fileToRestoreFrom (fileToRestoreFrom),
      m_restoreSchema (false),
      m_dataOnlyRestore (false),
      m_saveConfiguration (true)
{
}

DatabaseObjectManagerRestoreMessage::~DatabaseObjectManagerRestoreMessage ()
{
}

string DatabaseObjectManagerRestoreMessage::getFileToRestoreFrom () const
{
    return (m_fileToRestoreFrom);
}

bool DatabaseObjectManagerRestoreMessage::getRestoreSchema() const
{
    return (m_restoreSchema);
}

void DatabaseObjectManagerRestoreMessage::setRestoreSchema(const bool isRestoreNeeded)
{
    m_restoreSchema = isRestoreNeeded;
}

bool DatabaseObjectManagerRestoreMessage::getDataOnlyRestore () const
{
    return (m_dataOnlyRestore);
}

void DatabaseObjectManagerRestoreMessage::setDataOnlyRestore (const bool dataOnlyRestore)
{
    m_dataOnlyRestore = dataOnlyRestore;
}

bool DatabaseObjectManagerRestoreMessage::getSaveConfiguration () const
{
    return (m_saveConfiguration);
}

void DatabaseObjectManagerRestoreMessage::setSaveConfiguration (const bool saveConfiguration)
{
    m_saveConfiguration = saveConfiguration;
}

}
