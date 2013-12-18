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

#include "Framework/Persistence/PersistenceLocalObjectManagerSavePrismConfigurationMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"

namespace WaveNs
{

PersistenceLocalObjectManagerSavePrismConfigurationMessage::PersistenceLocalObjectManagerSavePrismConfigurationMessage (const bool &isStartupValid)
    : PrismMessage      (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_SAVE_PRISM_CONFIGURATION),
      m_isStartupValid (isStartupValid)
{
}

PersistenceLocalObjectManagerSavePrismConfigurationMessage::PersistenceLocalObjectManagerSavePrismConfigurationMessage ()
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_SAVE_PRISM_CONFIGURATION)
{
    m_isStartupValid = false;
}

PersistenceLocalObjectManagerSavePrismConfigurationMessage::~PersistenceLocalObjectManagerSavePrismConfigurationMessage ()
{
}

void PersistenceLocalObjectManagerSavePrismConfigurationMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isStartupValid, "isStartupValid"));
}

bool PersistenceLocalObjectManagerSavePrismConfigurationMessage::getIsStartupValid () const
{
    return (m_isStartupValid);
}

void PersistenceLocalObjectManagerSavePrismConfigurationMessage::setIsStartupValid (const bool &isStartupValid)
{
    m_isStartupValid = isStartupValid;
}

}
