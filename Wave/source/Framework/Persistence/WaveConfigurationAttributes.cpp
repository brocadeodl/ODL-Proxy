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

#include "Framework/Attributes/AttributeResourceId.h"
#include "Framework/Persistence/WaveConfigurationAttributes.h"

namespace WaveNs
{

WaveConfigurationAttributes::WaveConfigurationAttributes (string configName, string configValue, ResourceId configType)
    : SerializableObject         (),
      m_configName  (configName),
      m_configValue (configValue),
      m_configAttributeType  (configType)
{
}

WaveConfigurationAttributes::WaveConfigurationAttributes ()
    : SerializableObject         ()
{
}

WaveConfigurationAttributes::~WaveConfigurationAttributes ()
{
}

string WaveConfigurationAttributes::getConfigName ()
{
    return m_configName;
}

string WaveConfigurationAttributes::getConfigValue ()
{
    return m_configValue;
}

ResourceId WaveConfigurationAttributes::getConfigType ()
{
    return m_configAttributeType;
}

void WaveConfigurationAttributes::getConfigurationVectors (vector<WaveConfigurationAttributes> &configAttributes, vector<string> &configNames, vector<string> &configValues, vector<ResourceId> &configTypes)
{
    UI32 configSize = configAttributes.size ();
    if (0 == configSize)
    {
        return ;
    }
    for (UI32 i = 0; i < configSize; i++)
    {
        configNames.push_back  (configAttributes[i].getConfigName ());
        configValues.push_back (configAttributes[i].getConfigValue ());
        configTypes.push_back  (configAttributes[i].getConfigType ());
    }
}

void WaveConfigurationAttributes::setupAttributesForSerialization ()
{   
    addSerializableAttribute (new AttributeString           (&m_configName, "configName"));
    addSerializableAttribute (new AttributeString           (&m_configValue, "configValue"));
    addSerializableAttribute (new AttributeResourceId       (&m_configAttributeType, "configAttributeType"));
}

void WaveConfigurationAttributes::setupAttributesForSerializationInAttributeOrderFormat ()
{
    addAttributeNameForOrderToNameMapping ("configName");
    addAttributeNameForOrderToNameMapping ("configValue");
    addAttributeNameForOrderToNameMapping ("configAttributeType");
}

void WaveConfigurationAttributes::loadWaveConfiguration (const string &globalConfig)
{
    prepareForSerialization ();
    loadFromSerializedData2 (globalConfig);
}

void WaveConfigurationAttributes::getAttributeValue (const ResourceId attributeType, const string attributeValue, WaveCValue *pCValue)
{
    Attribute *pAttribute = Attribute::getAttributeFromAttributeType (attributeType);

    pAttribute->fromString (attributeValue);
    pAttribute->getCValue (pCValue);

    delete (pAttribute);
    pAttribute = NULL;
}

}
