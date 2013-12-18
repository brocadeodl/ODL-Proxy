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

#include "Framework/Persistence/PersistenceLocalObjectManagerSetStartupFileMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace WaveNs
{

PersistenceLocalObjectManagerSetStartupFileMessage::PersistenceLocalObjectManagerSetStartupFileMessage (const ResourceId &startupFileType, const string &fileName)
    : PrismMessage      (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_SET_STARTUP_FILE),
      m_startupFileType (startupFileType),
      m_fileName        (fileName)
{
}

PersistenceLocalObjectManagerSetStartupFileMessage::PersistenceLocalObjectManagerSetStartupFileMessage ()
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_SET_STARTUP_FILE),
      m_startupFileType(0)
{
}

PersistenceLocalObjectManagerSetStartupFileMessage::~PersistenceLocalObjectManagerSetStartupFileMessage ()
{
}

void PersistenceLocalObjectManagerSetStartupFileMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString  (&m_fileName,        "fileName"));
     addSerializableAttribute (new AttributeEnum    (&m_startupFileType, "startupFileType"));
}

string PersistenceLocalObjectManagerSetStartupFileMessage::getStartupFileName () const
{
    return (m_fileName);
}

void PersistenceLocalObjectManagerSetStartupFileMessage::setStartupFileName (const string &fileName)
{
    m_fileName = fileName;
}

ResourceId PersistenceLocalObjectManagerSetStartupFileMessage::getStartupFileType () const
{
    return (m_startupFileType);
}

void PersistenceLocalObjectManagerSetStartupFileMessage::setStartupFileType (const ResourceId &startupFileType)
{
    m_startupFileType = startupFileType;
}

}
