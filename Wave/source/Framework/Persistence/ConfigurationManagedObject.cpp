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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Persistence/ConfigurationManagedObject.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Attributes/Attribute.h"

namespace WaveNs
{

ConfigurationManagedObject::ConfigurationManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject (ConfigurationManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject (pWaveObjectManager)
{
    setEmptyNeededOnPersistentBoot (false);

    m_configurationNumber = 0;
}

ConfigurationManagedObject::~ConfigurationManagedObject ()
{
}

string ConfigurationManagedObject::getClassName ()
{
    return ("ConfigurationManagedObject");
}

void ConfigurationManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI64 (&m_configurationNumber, "configurationNumber"));
    addPersistableAttribute (new AttributeDateTime (&m_configurationTime,   "configurationTime"));
    addPersistableAttribute (new AttributeDateTime (&m_latestUpdateTime, "latestUpdateTime"));
}

void ConfigurationManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI64 (&m_configurationNumber, "configurationNumber"));
    addPersistableAttributeForCreate (new AttributeDateTime (&m_configurationTime,   "configurationTime"));    
    addPersistableAttributeForCreate (new AttributeDateTime (&m_latestUpdateTime, "latestUpdateTime"));
}

UI64 ConfigurationManagedObject::getConfigurationNumber () const
{
    return (m_configurationNumber);
}

DateTime ConfigurationManagedObject::getConfigurationTime () const
{
    return (m_configurationTime);
}

DateTime ConfigurationManagedObject::getLatestUpdateTime() const
{
    return (m_latestUpdateTime);
}

void ConfigurationManagedObject::setConfigurationNumber (const UI64 &configurationNumber)
{
    m_configurationNumber = configurationNumber;
}

void ConfigurationManagedObject::setConfigurationTime (const DateTime &configurationTime)
{
    m_configurationTime = configurationTime;
}

void ConfigurationManagedObject::setLatestUpdateTime (const DateTime &latestUpdateTime)
{
    m_latestUpdateTime = latestUpdateTime;
}

void ConfigurationManagedObject::resetConfigurationManagedObject ()
{
    m_configurationNumber = 0;
    DateTime currentDateTime;
    m_configurationTime = currentDateTime;
    m_latestUpdateTime = currentDateTime;
}

XPathStringManagedObject::XPathStringManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement                      (pWaveObjectManager),
      PrismPersistableObject            (XPathStringManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject                 (pWaveObjectManager)
{
    setEmptyNeededOnPersistentBoot (true);
}

XPathStringManagedObject::~XPathStringManagedObject ()
{
}

string XPathStringManagedObject::getClassName ()
{
    return ("XPathStringManagedObject");
}

void XPathStringManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_xPathString, "xPathString"));
}

void XPathStringManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString (&m_xPathString, "xPathString"));
}

void XPathStringManagedObject::setupKeys ()
{
    vector<string> keys;

    keys.push_back ("xPathString");

    setUserDefinedKeyCombination (keys);
}

void XPathStringManagedObject::setXPathString (const string &xPathString)
{
    m_xPathString = xPathString;
}

string XPathStringManagedObject::getXPathString () const
{
    return m_xPathString;
}

}
