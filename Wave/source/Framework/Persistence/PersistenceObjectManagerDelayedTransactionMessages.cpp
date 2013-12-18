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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/Persistence/PersistenceObjectManagerDelayedTransactionMessages.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceObjectManager.h"

namespace WaveNs
{

PersistenceObjectManagerAddDelayedTransactionMessage::PersistenceObjectManagerAddDelayedTransactionMessage (const string &sql)
    : PrismMessage (PersistenceObjectManager::getPrismServiceId (), PERSISTENCE_OBJECT_MANAGER_ADD_DELAYED_TRANSACTION),
    m_sql (sql)
{
}

PersistenceObjectManagerAddDelayedTransactionMessage::PersistenceObjectManagerAddDelayedTransactionMessage ()
    : PrismMessage (PersistenceObjectManager::getPrismServiceId (), PERSISTENCE_OBJECT_MANAGER_ADD_DELAYED_TRANSACTION),
    m_sql ("")
{
}

PersistenceObjectManagerAddDelayedTransactionMessage::~PersistenceObjectManagerAddDelayedTransactionMessage ()
{
}

void PersistenceObjectManagerAddDelayedTransactionMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_sql, "sql"));
}

string PersistenceObjectManagerAddDelayedTransactionMessage::getSql () const
{
    return (m_sql);
}

void PersistenceObjectManagerAddDelayedTransactionMessage::setSql (const string &sql)
{
    m_sql = sql;
}

}
