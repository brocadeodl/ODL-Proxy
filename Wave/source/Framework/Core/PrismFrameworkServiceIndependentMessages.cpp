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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               
 *   All rights reserved.  
 *   Description: This file implements the prism framework service independent
 *                messages.  These service independent messages are directly
 *                handled in the HaPeerMessageReceiverThread or 
 *                InterLocationMessageReceiverThread.
 *   Author : Brian Adaniya 
 *   Date   : 10/22/2012                                           
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkServiceIndependentMessages.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"

namespace WaveNs
{

FrameworkObjectManagerStoreConfigurationIntentMessage::FrameworkObjectManagerStoreConfigurationIntentMessage ()
    : WaveServiceIndependentMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_STORE_CONFIGURATION_INTENT),
    m_configurationIntentMessageId  (0)
{
}

FrameworkObjectManagerStoreConfigurationIntentMessage::FrameworkObjectManagerStoreConfigurationIntentMessage (const UI32 &configurationIntentMessageId)
    : WaveServiceIndependentMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_STORE_CONFIGURATION_INTENT),
    m_configurationIntentMessageId  (configurationIntentMessageId)
{
}

FrameworkObjectManagerStoreConfigurationIntentMessage::~FrameworkObjectManagerStoreConfigurationIntentMessage ()
{   
}

void FrameworkObjectManagerStoreConfigurationIntentMessage::setupAttributesForSerialization ()
{
    WaveServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32     (&m_configurationIntentMessageId, "configurationIntentMessageId"));
}

UI32 FrameworkObjectManagerStoreConfigurationIntentMessage::getConfigurationIntentMessageId () const
{
    return (m_configurationIntentMessageId);
}

void FrameworkObjectManagerStoreConfigurationIntentMessage::setConfigurationIntentMessageId (const UI32 &configurationIntentMessageId)
{
    m_configurationIntentMessageId = configurationIntentMessageId;
}


FrameworkObjectManagerRemoveConfigurationIntentMessage::FrameworkObjectManagerRemoveConfigurationIntentMessage ()
    : WaveServiceIndependentMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REMOVE_CONFIGURATION_INTENT),
    m_configurationIntentMessageId  (0)
{
}

FrameworkObjectManagerRemoveConfigurationIntentMessage::FrameworkObjectManagerRemoveConfigurationIntentMessage (const UI32 &configurationIntentMessageId)
    : WaveServiceIndependentMessage (PrismFrameworkObjectManager::getPrismServiceId (), FRAMEWORK_OBJECT_MANAGER_REMOVE_CONFIGURATION_INTENT),
    m_configurationIntentMessageId  (configurationIntentMessageId)
{
}

FrameworkObjectManagerRemoveConfigurationIntentMessage::~FrameworkObjectManagerRemoveConfigurationIntentMessage ()
{
}

void FrameworkObjectManagerRemoveConfigurationIntentMessage::setupAttributesForSerialization ()
{
    WaveServiceIndependentMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32 (&m_configurationIntentMessageId, "configurationIntentMessageId"));
}

UI32 FrameworkObjectManagerRemoveConfigurationIntentMessage::getConfigurationIntentMessageId () const
{
    return (m_configurationIntentMessageId);
}

void FrameworkObjectManagerRemoveConfigurationIntentMessage::setConfigurationIntentMessageId (const UI32 &configurationIntentMessageId)
{
    m_configurationIntentMessageId = configurationIntentMessageId; 
}

}
