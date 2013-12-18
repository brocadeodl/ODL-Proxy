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

#include "Framework/Database/DatabaseObjectManagerExecuteCopySchemaMessage.h"
#include "Framework/Database/DatabaseObjectManager.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"

namespace WaveNs
{

DatabaseObjectManagerExecuteCopySchemaMessage::DatabaseObjectManagerExecuteCopySchemaMessage (const string &prepareString, const string &sourceSchema, const string &destSchema)
    : PrismMessage (DatabaseObjectManager::getPrismServiceId (), DATABASE_OBJECT_MANAGER_EXECUTE_COPY_SCHEMA),
      m_pResults (NULL),
      m_prepareString (prepareString),
      m_sourceSchema (sourceSchema),
      m_destSchema (destSchema)
{
}

DatabaseObjectManagerExecuteCopySchemaMessage::DatabaseObjectManagerExecuteCopySchemaMessage (const string &sourceSchema, const string &destSchema)
    : PrismMessage (DatabaseObjectManager::getPrismServiceId (), DATABASE_OBJECT_MANAGER_EXECUTE_COPY_SCHEMA),
      m_pResults (NULL),
      m_prepareString (""),
      m_sourceSchema (sourceSchema),
      m_destSchema (destSchema)
{
}

DatabaseObjectManagerExecuteCopySchemaMessage::DatabaseObjectManagerExecuteCopySchemaMessage ()
    : PrismMessage (DatabaseObjectManager::getPrismServiceId (), DATABASE_OBJECT_MANAGER_EXECUTE_COPY_SCHEMA),
      m_pResults (NULL),
      m_prepareString (""),
      m_sourceSchema (""),
      m_destSchema ("")
{
}

DatabaseObjectManagerExecuteCopySchemaMessage::~DatabaseObjectManagerExecuteCopySchemaMessage ()
{
}

void DatabaseObjectManagerExecuteCopySchemaMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_prepareString,   "prepareString"));
     addSerializableAttribute (new AttributeString (&m_sourceSchema,    "sourceSchema"));
     addSerializableAttribute (new AttributeString (&m_destSchema,      "destSchema"));
}

void DatabaseObjectManagerExecuteCopySchemaMessage::setPResults (vector<WaveManagedObject *> *pResults)
{
    m_pResults = pResults;
}

vector<WaveManagedObject *> *DatabaseObjectManagerExecuteCopySchemaMessage::getPResults () const
{
    return (m_pResults);
}

string DatabaseObjectManagerExecuteCopySchemaMessage::getPrepareString () const
{
    return (m_prepareString);
}

void DatabaseObjectManagerExecuteCopySchemaMessage::setPrepareString (const string &prepareString)
{
    m_prepareString = prepareString;
}

string DatabaseObjectManagerExecuteCopySchemaMessage::getSourceSchema () const
{
    return (m_sourceSchema);
}

void DatabaseObjectManagerExecuteCopySchemaMessage::setSourceSchema (const string &sourceSchema)
{
    m_sourceSchema = sourceSchema;
}

string DatabaseObjectManagerExecuteCopySchemaMessage::getDestSchema () const
{
    return (m_destSchema);
}

void DatabaseObjectManagerExecuteCopySchemaMessage::setDestSchema (const string &destSchema)
{
    m_destSchema = destSchema;
}

}
