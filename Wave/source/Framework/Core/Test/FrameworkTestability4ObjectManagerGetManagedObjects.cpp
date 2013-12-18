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

#include "Framework/Core/Test/FrameworkTestability4ObjectManagerGetManagedObjects.h"
#include "Framework/Core/Test/FrameworkTestabilityTypes.h"
#include "Framework/Core/Test/FrameworkTestability4ObjectManager.h"

namespace WaveNs
{

FrameworkTestability4ObjectManagerGetManagedObjects::FrameworkTestability4ObjectManagerGetManagedObjects()
    : ManagementInterfaceMessage (FrameworkTestability4ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_4_GET_MANAGED_OBJECTS),
      m_managedObjectClassName   (""),
      m_schemaName               ("WaveCurrent"),
      m_numberOfManagedObjects   (0),
      m_useRawFormat             (false)
{
}

FrameworkTestability4ObjectManagerGetManagedObjects::FrameworkTestability4ObjectManagerGetManagedObjects(const string &managedObjectClassName, const string &schemaName)
    : ManagementInterfaceMessage (FrameworkTestability4ObjectManager::getPrismServiceName (), FRAMEWORK_TESTABILITY_4_GET_MANAGED_OBJECTS),
      m_managedObjectClassName   (managedObjectClassName),
      m_schemaName               (schemaName),
      m_numberOfManagedObjects   (0),
      m_useRawFormat             (false)
{
}

FrameworkTestability4ObjectManagerGetManagedObjects::~FrameworkTestability4ObjectManagerGetManagedObjects ()
{
}

void FrameworkTestability4ObjectManagerGetManagedObjects::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString       (&m_managedObjectClassName,    "managedObjectClassName"));
     addSerializableAttribute (new AttributeString       (&m_schemaName,                "schemaName"));
     addSerializableAttribute (new AttributeUI32         (&m_numberOfManagedObjects,    "numberOfManagedObjects"));
     addSerializableAttribute (new AttributeStringVector (&m_managedObjects,            "managedObjects"));
     addSerializableAttribute (new AttributeBool         (&m_useRawFormat,              "useRawFormat"));
}

string FrameworkTestability4ObjectManagerGetManagedObjects::getManagedObjectClassName () const
{
    return (m_managedObjectClassName);
}

void FrameworkTestability4ObjectManagerGetManagedObjects::setManagedObjectClassName (const string &managedObjectClassName)
{
    m_managedObjectClassName = managedObjectClassName;
}

string FrameworkTestability4ObjectManagerGetManagedObjects::getSchemaName () const
{
    return (m_schemaName);
}

void FrameworkTestability4ObjectManagerGetManagedObjects::setSchemaName (const string &schemaName)
{
    m_schemaName = schemaName;
}

UI32 FrameworkTestability4ObjectManagerGetManagedObjects::getNumberOfManagedObjects () const
{
    return (m_numberOfManagedObjects);
}

void FrameworkTestability4ObjectManagerGetManagedObjects::setNumberOfManagedObjects (const UI32 &numberOfManagedObjects)
{
    m_numberOfManagedObjects = numberOfManagedObjects;
}

vector<string> &FrameworkTestability4ObjectManagerGetManagedObjects::getManagedObjects ()
{
    return (m_managedObjects);
}

bool FrameworkTestability4ObjectManagerGetManagedObjects::getUseRawFormat() const
{
    return (m_useRawFormat);
}

void FrameworkTestability4ObjectManagerGetManagedObjects::setUseRawFormat(const bool& useRawFormat)
{
    m_useRawFormat = useRawFormat;
}

}
