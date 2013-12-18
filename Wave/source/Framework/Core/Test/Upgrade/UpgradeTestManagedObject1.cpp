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

#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject1.h"

namespace WaveNs
{

UpgradeTestManagedObject1::UpgradeTestManagedObject1 (WaveObjectManager *pWaveObjectManager)
        : PrismElement (pWaveObjectManager),
        PrismPersistableObject (UpgradeTestManagedObject1::getClassName (), WaveManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        m_integer1        (0)
{
}

UpgradeTestManagedObject1::~UpgradeTestManagedObject1 ()
{
}

string UpgradeTestManagedObject1::getClassName ()
{
    return ("UpgradeTestManagedObject1");
}

void UpgradeTestManagedObject1::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32     (&m_integer1,  "integer1"));
    addPersistableAttribute (new AttributeString   (&m_message1,  "message1"));
    addPersistableAttribute (new AttributeObjectId (&m_objectId1, "objectId1"));
}

void UpgradeTestManagedObject1::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32     (&m_integer1,  "integer1"));
    addPersistableAttributeForCreate (new AttributeString   (&m_message1,  "message1"));
    addPersistableAttributeForCreate (new AttributeObjectId (&m_objectId1, "objectId1"));
}

void UpgradeTestManagedObject1::setInteger1 (const UI32 &integer1)
{
    m_integer1 = integer1;
}

void UpgradeTestManagedObject1::setMessage1 (const string &message1)
{
    m_message1 = message1;
}

void UpgradeTestManagedObject1::setObjectId1 (const ObjectId &objectId1)
{
    m_objectId1 = objectId1;
}

UI32 UpgradeTestManagedObject1::getInteger1 () const
{
    return (m_integer1);
}

string UpgradeTestManagedObject1::getMessage1 () const
{
    return (m_message1);
}

ObjectId UpgradeTestManagedObject1::getObjectId1 () const
{
    return (m_objectId1);
}

bool UpgradeTestManagedObject1::operator == (const UpgradeTestManagedObject1 &rhs) const
{
    if ((rhs.getObjectId ()) != ((getObjectId ())))
    {
        return (false);
    }

    if (rhs.m_integer1 != m_integer1)
    {
        return (false);
    }

    if (rhs.m_message1 != m_message1)
    {
        return (false);
    }

    if (rhs.m_objectId1 != m_objectId1)
    {
        return (false);
    }

    return (true);
}

bool UpgradeTestManagedObject1::operator != (const UpgradeTestManagedObject1 &rhs) const
{
    return (!(operator == (rhs)));
}

}
