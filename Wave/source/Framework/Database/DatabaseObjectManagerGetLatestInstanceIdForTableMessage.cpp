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

#include "Framework/Database/DatabaseObjectManagerGetLatestInstanceIdForTableMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"

namespace WaveNs
{

DatabaseObjectManagerGetLatestInstanceIdForTableMessage::DatabaseObjectManagerGetLatestInstanceIdForTableMessage (const string &tableName)
    : PrismMessage (DatabaseObjectManager::getPrismServiceId (), DATABASE_OBJECT_MANAGER_GET_LATEST_INSTANCE_ID_FOR_TABLE),
      m_tableName (tableName),
      m_latestInstanceId (0)
{
}

DatabaseObjectManagerGetLatestInstanceIdForTableMessage::~DatabaseObjectManagerGetLatestInstanceIdForTableMessage ()
{
}

string DatabaseObjectManagerGetLatestInstanceIdForTableMessage::getTableName () const
{
    return (m_tableName);
}

UI32 DatabaseObjectManagerGetLatestInstanceIdForTableMessage::getLatestInstanceId () const
{
    return (m_latestInstanceId);
}

void DatabaseObjectManagerGetLatestInstanceIdForTableMessage::setLatestInstabceId (const UI32 &latestInstanceId)
{
    m_latestInstanceId = latestInstanceId;
}

}
