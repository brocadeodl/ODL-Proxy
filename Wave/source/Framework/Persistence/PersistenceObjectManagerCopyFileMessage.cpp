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

#include "Framework/Persistence/PersistenceObjectManagerCopyFileMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceObjectManager.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace WaveNs
{

PersistenceObjectManagerCopyFileMessage::PersistenceObjectManagerCopyFileMessage (const ResourceId &startupFileType, const string &startupFileName, const bool &isLocalFileCopy)
    : PrismMessage (PersistenceObjectManager::getPrismServiceId (), PERSISTENCE_OBNJECT_MANAGER_COPY_FILE),
      m_startupFileType (startupFileType),
      m_startupFileName (startupFileName),
      m_isLocalFileCopy (isLocalFileCopy)
{
}

PersistenceObjectManagerCopyFileMessage::PersistenceObjectManagerCopyFileMessage ()
    : PrismMessage (PersistenceObjectManager::getPrismServiceId (), PERSISTENCE_OBNJECT_MANAGER_COPY_FILE),
      m_startupFileType (0),
      m_isLocalFileCopy(false)
{
}

PersistenceObjectManagerCopyFileMessage::~PersistenceObjectManagerCopyFileMessage ()
{
}

void PersistenceObjectManagerCopyFileMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString (&m_startupFileName, "startupFileName"));
     addSerializableAttribute (new AttributeEnum (&m_startupFileType,   "startupFileType"));
     addSerializableAttribute (new AttributeBool (&m_isLocalFileCopy,   "isLocalFileCopy"));
}

string PersistenceObjectManagerCopyFileMessage::getStartupFileName () const
{
    return (m_startupFileName);
}

void PersistenceObjectManagerCopyFileMessage::setStartupFileName (const string &startupFileName)
{
    m_startupFileName = startupFileName;
}

ResourceId PersistenceObjectManagerCopyFileMessage::getStartupFileType () const
{
    return (m_startupFileType);
}

void PersistenceObjectManagerCopyFileMessage::setStartupFileType (const ResourceId &startupFileType)
{
    m_startupFileType = startupFileType;
}

bool PersistenceObjectManagerCopyFileMessage::getIsLocalFileCopy () const
{
    return (m_isLocalFileCopy);
}

void PersistenceObjectManagerCopyFileMessage::setIsLocalFileCopy (const bool &isLocalFileCopy)
{
    m_isLocalFileCopy =isLocalFileCopy;
}
}
