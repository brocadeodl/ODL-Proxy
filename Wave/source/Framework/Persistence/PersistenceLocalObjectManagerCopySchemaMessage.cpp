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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "Framework/Persistence/PersistenceLocalObjectManagerCopySchemaMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"

namespace WaveNs
{

PersistenceLocalObjectManagerCopySchemaMessage::PersistenceLocalObjectManagerCopySchemaMessage (const string &sourceSchema, const string &destSchema)
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_COPY_SCHEMA),
      m_sourceSchema (sourceSchema),
      m_destSchema (destSchema)
{
}

PersistenceLocalObjectManagerCopySchemaMessage::PersistenceLocalObjectManagerCopySchemaMessage ()
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_COPY_SCHEMA)
{
}

PersistenceLocalObjectManagerCopySchemaMessage::~PersistenceLocalObjectManagerCopySchemaMessage ()
{
}

void PersistenceLocalObjectManagerCopySchemaMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_sourceSchema,    "sourceSchema"));
     addSerializableAttribute (new AttributeString (&m_destSchema,      "destSchema"));
}

string PersistenceLocalObjectManagerCopySchemaMessage::getSourceSchema () const
{
    return (m_sourceSchema);
}

void PersistenceLocalObjectManagerCopySchemaMessage::setSourceSchema (const string &sourceSchema)
{
    m_sourceSchema = sourceSchema;
}

string PersistenceLocalObjectManagerCopySchemaMessage::getDestSchema () const
{
    return (m_destSchema);
}

void PersistenceLocalObjectManagerCopySchemaMessage::setDestSchema (const string &destSchema)
{
    m_destSchema = destSchema;
}

}
