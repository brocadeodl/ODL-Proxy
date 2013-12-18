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

#include "Framework/Database/DatabaseObjectManagerEmptyMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"

namespace WaveNs
{

DatabaseObjectManagerEmptyMessage::DatabaseObjectManagerEmptyMessage ()
    : ManagementInterfaceMessage (DatabaseObjectManager::getClassName (), DATABASE_OBJECT_MANAGER_EMPTY)
{
    m_schemasToBeEmptied.clear ();

    m_executeEmptyDatabase = true;
    m_databaseEmptyType    = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_NOT_DEFINED;
    m_returnSqlForEmpty    = false;
    m_selectivelyEmpty     = false;
}

DatabaseObjectManagerEmptyMessage::~DatabaseObjectManagerEmptyMessage ()
{
}

void DatabaseObjectManagerEmptyMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization (); 

     addSerializableAttribute (new AttributeString (&m_sqlForEmpty, "sqlForEmpty"));
     addSerializableAttribute (new AttributeBool   (&m_executeEmptyDatabase, "executeEmptyDatabase"));
     addSerializableAttribute (new AttributeBool   (&m_returnSqlForEmpty, "returnSqlForEmpty"));
     addSerializableAttribute (new AttributeUI32   (&m_databaseEmptyType, "databaseEmptyType"));
     addSerializableAttribute (new AttributeBool   (&m_selectivelyEmpty, "selectivelyEmpty"));
}

void DatabaseObjectManagerEmptyMessage::setSchemasToBeEmptied (const set<string> schemasToBeEmptied)
{
    m_schemasToBeEmptied = schemasToBeEmptied;
}

set<string> DatabaseObjectManagerEmptyMessage::getSchemasToBeEmptied () const
{
    return (m_schemasToBeEmptied);
}

void DatabaseObjectManagerEmptyMessage::addToSchemasToBeEmptied (const string schemaToBeEmptied)
{
    m_schemasToBeEmptied.insert (schemaToBeEmptied);
}

void DatabaseObjectManagerEmptyMessage::setSqlForEmpty (const string &emptySql)
{
    m_sqlForEmpty = emptySql;
}

void DatabaseObjectManagerEmptyMessage::getSqlForEmpty (string &emptySql)
{
    emptySql = m_sqlForEmpty;
}

void DatabaseObjectManagerEmptyMessage::setExecuteEmptyDatabase (const bool &emptyDatabase)
{
    m_executeEmptyDatabase = emptyDatabase;
}

bool DatabaseObjectManagerEmptyMessage::getIsExecuteEmptyDatabase () const
{
    return (m_executeEmptyDatabase);
}

void DatabaseObjectManagerEmptyMessage::setReturnSqlForEmptyDatabase (const bool &returnSql)
{
    m_returnSqlForEmpty = returnSql;
}

bool DatabaseObjectManagerEmptyMessage::getIsReturnSqlForEmptyDatabase () const
{
    return (m_returnSqlForEmpty);
}

UI32 DatabaseObjectManagerEmptyMessage::getDatabaseEmptyType ()
{
    return (m_databaseEmptyType);
}

void DatabaseObjectManagerEmptyMessage::setDatabaseEmptyType (const UI32 &emptyType)
{
    m_databaseEmptyType = emptyType;
}

void DatabaseObjectManagerEmptyMessage::setSelectivelyEmptyDatabase (const bool &selectivelyEmpty)
{
    m_selectivelyEmpty = selectivelyEmpty;
}

bool DatabaseObjectManagerEmptyMessage::getIsSelectivelyEmptyDatabase () const
{
    return (m_selectivelyEmpty);
}


DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage::DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage ()
    : ManagementInterfaceMessage (DatabaseObjectManager::getClassName (), DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_PARAMETER_CONFIGURE),
      m_thresholdValue (0),
      m_numberOfEntries (0)
{
}

DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage::~DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage ()
{
}

void DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization (); 

     addSerializableAttribute (new AttributeUI32   (&m_thresholdValue, "thresholdValue"));
     addSerializableAttribute (new AttributeUI32   (&m_numberOfEntries, "numberOfEntries"));
}

void DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage::setEmptyDatabaseAutoDetectionThresholdValue (const UI32 &thresholdValue)
{
    m_thresholdValue = thresholdValue;
}

UI32 DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage::getEmptyDatabaseAutoDetectionThresholdValue ()
{
    return (m_thresholdValue);
}

void DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage::setNumberOfEntriesInDatabase (const UI32 &numberOfEntries)
{
    m_numberOfEntries = numberOfEntries;
}


UI32 DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage::getNumberOfEntriesInDatabase ()
{
    return (m_numberOfEntries);
}

}
