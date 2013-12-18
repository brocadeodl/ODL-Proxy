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

#include "Framework/Database/DatabaseObjectManagerExecuteTransactionMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"

namespace WaveNs
{

DatabaseObjectManagerExecuteTransactionMessage::DatabaseObjectManagerExecuteTransactionMessage (const string &sql)
    : PrismMessage (DatabaseObjectManager::getPrismServiceId (), DATABASE_OBJECT_MANAGER_EXECUTE_TRANSACTION),
      m_sql (sql),
      m_isCommit (false),
      m_isPrepare (false)
{
}

DatabaseObjectManagerExecuteTransactionMessage::DatabaseObjectManagerExecuteTransactionMessage ()
    : PrismMessage (DatabaseObjectManager::getPrismServiceId (), DATABASE_OBJECT_MANAGER_EXECUTE_TRANSACTION),
      m_isCommit (false),
      m_isPrepare (false)
{
}

DatabaseObjectManagerExecuteTransactionMessage::~DatabaseObjectManagerExecuteTransactionMessage ()
{
}

void DatabaseObjectManagerExecuteTransactionMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_sql,         "sql"));
     addSerializableAttribute (new AttributeBool   (&m_isCommit,    "isCommit"));
     addSerializableAttribute (new AttributeBool   (&m_isPrepare,   "isPrepare"));
}

const string &DatabaseObjectManagerExecuteTransactionMessage::getSql () const
{
    return (m_sql);
}

void DatabaseObjectManagerExecuteTransactionMessage::setSql (const string &sql)
{
    m_sql = sql;
}

bool DatabaseObjectManagerExecuteTransactionMessage::getIsCommit () const
{
    return (m_isCommit);
}

void DatabaseObjectManagerExecuteTransactionMessage::setIsCommit (const bool &isCommit)
{
    m_isCommit = isCommit;
}

bool DatabaseObjectManagerExecuteTransactionMessage::getIsPrepare () const
{
    return (m_isPrepare);
}

void DatabaseObjectManagerExecuteTransactionMessage::setIsPrepare (const bool &isPrepare)
{
    m_isPrepare = isPrepare;
}

}
