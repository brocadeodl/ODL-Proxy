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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "Framework/Persistence/PersistenceLocalObjectManagerLiveSyncEnableMessages.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"

namespace WaveNs
{

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------PersistenceLocalObjectManagerGetLiveSyncEnableMessage-------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

PersistenceLocalObjectManagerGetLiveSyncEnableMessage::PersistenceLocalObjectManagerGetLiveSyncEnableMessage ()
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_GET_LIVE_SYNC_ENABLE), 
      m_isLiveSyncEnabled (false)
{
}

PersistenceLocalObjectManagerGetLiveSyncEnableMessage::~PersistenceLocalObjectManagerGetLiveSyncEnableMessage ()
{
}

void PersistenceLocalObjectManagerGetLiveSyncEnableMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isLiveSyncEnabled, "isLiveSyncEnabled"));
}

bool PersistenceLocalObjectManagerGetLiveSyncEnableMessage::getIsLiveSyncEnabled ()
{
    return (m_isLiveSyncEnabled);
}

void PersistenceLocalObjectManagerGetLiveSyncEnableMessage::setIsLiveSyncEnabled (const bool &isLiveSyncEnabled)
{
    m_isLiveSyncEnabled = isLiveSyncEnabled;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------PersistenceLocalObjectManagerSetLiveSyncEnableMessage-------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

PersistenceLocalObjectManagerSetLiveSyncEnableMessage::PersistenceLocalObjectManagerSetLiveSyncEnableMessage ()
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_SET_LIVE_SYNC_ENABLE), 
      m_isLiveSyncEnabled (false)
{
    setIsMessageSupportedWhenServiceIsPaused (true);
}

PersistenceLocalObjectManagerSetLiveSyncEnableMessage::PersistenceLocalObjectManagerSetLiveSyncEnableMessage (const bool &isLiveSyncEnabled)
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_SET_LIVE_SYNC_ENABLE), 
      m_isLiveSyncEnabled (isLiveSyncEnabled)
{
    setIsMessageSupportedWhenServiceIsPaused (true);
}

PersistenceLocalObjectManagerSetLiveSyncEnableMessage::~PersistenceLocalObjectManagerSetLiveSyncEnableMessage ()
{
}

void PersistenceLocalObjectManagerSetLiveSyncEnableMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isLiveSyncEnabled, "isLiveSyncEnabled"));
}

bool PersistenceLocalObjectManagerSetLiveSyncEnableMessage::getIsLiveSyncEnabled ()
{
    return (m_isLiveSyncEnabled);
}

void PersistenceLocalObjectManagerSetLiveSyncEnableMessage::setIsLiveSyncEnabled (const bool &isLiveSyncEnabled)
{
    m_isLiveSyncEnabled = isLiveSyncEnabled;
}

}
