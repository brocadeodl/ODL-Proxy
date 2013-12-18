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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#include "Framework/Persistence/PersistenceLocalObjectManagerPushConfigToKernelMessaage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"

namespace WaveNs
{

PersistenceLocalObjectManagerPushConfigToKernelMessaage::PersistenceLocalObjectManagerPushConfigToKernelMessaage (const string &globalConfigs)
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_PUSH_CONFIG_TO_KERNEL),
      m_globalConfigs (globalConfigs)
{
}

PersistenceLocalObjectManagerPushConfigToKernelMessaage::PersistenceLocalObjectManagerPushConfigToKernelMessaage()
    : PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (), PERSISTENCE_PUSH_CONFIG_TO_KERNEL)
{
}

PersistenceLocalObjectManagerPushConfigToKernelMessaage::~PersistenceLocalObjectManagerPushConfigToKernelMessaage ()
{
}

void PersistenceLocalObjectManagerPushConfigToKernelMessaage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_globalConfigs, "globalConfigs"));
}

string PersistenceLocalObjectManagerPushConfigToKernelMessaage::getGlobalConfigs () const
{
    return (m_globalConfigs);
}

void PersistenceLocalObjectManagerPushConfigToKernelMessaage::setGlobalConfigs (const string &globalConfigs)
{
    m_globalConfigs = globalConfigs;
}

}
