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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Test/FrameworkTestability6ObjectManagerGetManagedObjects.h"
#include "Framework/Core/Test/FrameworkTestabilityTypes.h"
#include "Framework/Core/Test/FrameworkTestability6ObjectManager.h"

namespace WaveNs
{

FrameworkTestability6ObjectManagerGetManagedObjects::FrameworkTestability6ObjectManagerGetManagedObjects()
        : ManagementInterfaceMessage (FrameworkTestability6ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_6_GET_MANAGED_OBJECTS),
        m_managedObjectClassName   (""),
        m_schemaName               ("WaveCurrent"),
        m_numberOfManagedObjects   (0),
        m_useRawFormat             (false)
{
}

FrameworkTestability6ObjectManagerGetManagedObjects::FrameworkTestability6ObjectManagerGetManagedObjects(const string &managedObjectClassName, const string &schemaName)
        : ManagementInterfaceMessage (FrameworkTestability6ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_6_GET_MANAGED_OBJECTS),
        m_managedObjectClassName   (managedObjectClassName),
        m_schemaName               (schemaName),
        m_numberOfManagedObjects   (0),
        m_useRawFormat             (false)
{
}

FrameworkTestability6ObjectManagerGetManagedObjects::~FrameworkTestability6ObjectManagerGetManagedObjects ()
{
}

void FrameworkTestability6ObjectManagerGetManagedObjects::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString       (&m_managedObjectClassName,    "managedObjectClassName"));
     addSerializableAttribute (new AttributeString       (&m_schemaName,                "schemaName"));
     addSerializableAttribute (new AttributeUI32         (&m_numberOfManagedObjects,    "numberOfManagedObjects"));
     addSerializableAttribute (new AttributeStringVector (&m_managedObjects,            "managedObjects"));
     addSerializableAttribute (new AttributeBool         (&m_useRawFormat,              "useRawFormat"));
}

string FrameworkTestability6ObjectManagerGetManagedObjects::getManagedObjectClassName () const
{
    return (m_managedObjectClassName);
}

void FrameworkTestability6ObjectManagerGetManagedObjects::setManagedObjectClassName (const string &managedObjectClassName)
{
    m_managedObjectClassName = managedObjectClassName;
}

string FrameworkTestability6ObjectManagerGetManagedObjects::getSchemaName () const
{
    return (m_schemaName);
}

void FrameworkTestability6ObjectManagerGetManagedObjects::setSchemaName (const string &schemaName)
{
    m_schemaName = schemaName;
}

UI32 FrameworkTestability6ObjectManagerGetManagedObjects::getNumberOfManagedObjects () const
{
    return (m_numberOfManagedObjects);
}

void FrameworkTestability6ObjectManagerGetManagedObjects::setNumberOfManagedObjects (const UI32 &numberOfManagedObjects)
{
    m_numberOfManagedObjects = numberOfManagedObjects;
}

vector<string> &FrameworkTestability6ObjectManagerGetManagedObjects::getManagedObjects ()
{
    return (m_managedObjects);
}

bool FrameworkTestability6ObjectManagerGetManagedObjects::getUseRawFormat() const
{
    return (m_useRawFormat);
}

void FrameworkTestability6ObjectManagerGetManagedObjects::setUseRawFormat(const bool& useRawFormat)
{
    m_useRawFormat = useRawFormat;
}

}
