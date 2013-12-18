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

#include "Framework/Persistence/PersistenceObjectManagerExecuteTransactionMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

PersistenceObjectManagerExecuteTransactionMessage::PersistenceObjectManagerExecuteTransactionMessage (const string &sql)
    : PrismMessage (PersistenceObjectManager::getPrismServiceId (), PERSISTENCE_OBNJECT_MANAGER_EXECUTE_TRANSACTION),
      m_sql (sql)
{
}

PersistenceObjectManagerExecuteTransactionMessage::PersistenceObjectManagerExecuteTransactionMessage ()
    : PrismMessage (PersistenceObjectManager::getPrismServiceId (), PERSISTENCE_OBNJECT_MANAGER_EXECUTE_TRANSACTION)
{
}

PersistenceObjectManagerExecuteTransactionMessage::~PersistenceObjectManagerExecuteTransactionMessage ()
{
}

void PersistenceObjectManagerExecuteTransactionMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_sql, "sql"));

    addSerializableAttribute (new AttributeStringVector     (&m_configNamesForInsert,             "configNamesForInsert"));
    addSerializableAttribute (new AttributeStringVector     (&m_configValuesForInsert,            "configValuesForInsert"));
    addSerializableAttribute (new AttributeResourceIdVector (&m_configTypesForInsert,             "configTypesForInsert"));

    addSerializableAttribute (new AttributeStringVector     (&m_configNamesForUpdate,             "configNamesForUpdate"));
    addSerializableAttribute (new AttributeStringVector     (&m_configValuesForUpdate,            "configValuesForUpdate"));
    addSerializableAttribute (new AttributeResourceIdVector (&m_configTypesForUpdate,             "configTypesForUpdate"));
}

const string &PersistenceObjectManagerExecuteTransactionMessage::getSql () const
{
    return (m_sql);
}

void PersistenceObjectManagerExecuteTransactionMessage::setSql (const string &sql)
{
    m_sql = sql;
}

void PersistenceObjectManagerExecuteTransactionMessage::setConfigNamesForInsert (const vector<string> &configNamesForInsert)
{
    m_configNamesForInsert = configNamesForInsert;
}

vector<string> PersistenceObjectManagerExecuteTransactionMessage::getConfigNamesForInsert () const
{
    return (m_configNamesForInsert);
}

void PersistenceObjectManagerExecuteTransactionMessage::setConfigValuesForInsert (const vector<string> &configValuesForInsert)
{
    m_configValuesForInsert = configValuesForInsert;
}

vector<string> PersistenceObjectManagerExecuteTransactionMessage::getConfigValuesForInsert () const
{
    return (m_configValuesForInsert);
}

void PersistenceObjectManagerExecuteTransactionMessage::setConfigTypesForInsert (const vector<ResourceId> &configTypesForInsert)
{
    m_configTypesForInsert = configTypesForInsert;
}

vector<ResourceId> PersistenceObjectManagerExecuteTransactionMessage::getConfigTypesForInsert () const
{
    return (m_configTypesForInsert);
}

void PersistenceObjectManagerExecuteTransactionMessage::setConfigNamesForUpdate (const vector<string> &configNamesForUpdate)
{
    m_configNamesForUpdate = configNamesForUpdate;
}

vector<string> PersistenceObjectManagerExecuteTransactionMessage::getConfigNamesForUpdate () const
{
    return (m_configNamesForUpdate);
}

void PersistenceObjectManagerExecuteTransactionMessage::setConfigValuesForUpdate (const vector<string> &configValuesForUpdate)
{
    m_configValuesForUpdate = configValuesForUpdate;
}

vector<string> PersistenceObjectManagerExecuteTransactionMessage::getConfigValuesForUpdate () const
{
    return (m_configValuesForUpdate);
}

void PersistenceObjectManagerExecuteTransactionMessage::setConfigTypesForUpdate (const vector<ResourceId> &configTypesForUpdate)
{
    m_configTypesForUpdate = configTypesForUpdate;
}

vector<ResourceId> PersistenceObjectManagerExecuteTransactionMessage::getConfigTypesForUpdate () const
{
    return (m_configTypesForUpdate);
}

}
