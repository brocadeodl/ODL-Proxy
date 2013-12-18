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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#include "Framework/Persistence/WaveConfigManagedObject.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{
/*
 * This Managed Object stores all Global Config/Cluster wide configurations
 * Eg. Gvlan state 
 */
WaveConfigManagedObject::WaveConfigManagedObject (WaveObjectManager *pWaveObjectManager)
    : WaveConfigurationAttributes ("", "", 0),
      PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (WaveConfigManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager)
{
    setEmptyNeededOnPersistentBoot (false);
    setEmptyNeededOnPersistentBootWithDefault (true);
}

WaveConfigManagedObject::~WaveConfigManagedObject ()
{
}

string WaveConfigManagedObject::getClassName ()
{
    return ("WaveConfigManagedObject");
}

void WaveConfigManagedObject::setConfigName (const string &configName)
{
    m_configName = configName;
}

string WaveConfigManagedObject::getConfigName () const
{
    return (m_configName);
}

void WaveConfigManagedObject::setConfigValue (const string &configValue)
{
    m_configValue = configValue;
}

string WaveConfigManagedObject::getConfigValue () const
{
    return (m_configValue);
}

void WaveConfigManagedObject::setConfigAttributeType (const ResourceId &configAttributeType)
{
    m_configAttributeType = configAttributeType;
}

ResourceId WaveConfigManagedObject::getConfigAttributeType () const
{
    return (m_configAttributeType);
}

void WaveConfigManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_configName, "configName"));
    addPersistableAttribute (new AttributeString (&m_configValue, "configValue"));
    addPersistableAttribute (new AttributeResourceId (&m_configAttributeType, "configAttributeType"));
}

void WaveConfigManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString (&m_configName, "configName"));
    addPersistableAttributeForCreate (new AttributeString (&m_configValue, "configValue"));
    addPersistableAttributeForCreate (new AttributeResourceId (&m_configAttributeType, "configAttributeType"));

    vector<string > keyName;
    keyName.push_back ("configName");
    setUserDefinedKeyCombination (keyName);
}

}
