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
 *   Copyright (C) 2008-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessageForCount.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"

namespace WaveNs
{

DatabaseObjectManagerExecuteQueryMessageForCount::DatabaseObjectManagerExecuteQueryMessageForCount (const string &className, WaveObjectManager *pWaveObjectManager, const string &sql)
    : PrismMessage                               (DatabaseObjectManagerExecuteQueryMessage::getDatabaseObjectManagerServiceId (), DATABASE_OBJECT_MANAGER_EXECUTE_QUERY_FOR_COUNT),
      m_className                                (className),
      m_pWaveObjectManager                       (pWaveObjectManager),
      m_sql                                      (sql),
      m_count                                    ((UI32) 0)
{
     setIsMessageSupportedWhenServiceIsPaused (true);
}

DatabaseObjectManagerExecuteQueryMessageForCount::~DatabaseObjectManagerExecuteQueryMessageForCount ()
{
}

string DatabaseObjectManagerExecuteQueryMessageForCount::getClassName () const
{
    return (m_className);
}

WaveObjectManager *DatabaseObjectManagerExecuteQueryMessageForCount::getPWaveObjectManager () const
{
    return (m_pWaveObjectManager);
}

string DatabaseObjectManagerExecuteQueryMessageForCount::getSql () const
{
    return (m_sql);
}

void DatabaseObjectManagerExecuteQueryMessageForCount::setCount (const UI32 &count)
{
    m_count = count;
}

UI32 DatabaseObjectManagerExecuteQueryMessageForCount::getCount () const
{
    return (m_count);
}

}
