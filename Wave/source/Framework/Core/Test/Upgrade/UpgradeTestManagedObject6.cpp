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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject6.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"

namespace WaveNs
{

UpgradeTestManagedObject6::UpgradeTestManagedObject6 (WaveObjectManager *pWaveObjectManager)
        : PrismElement (pWaveObjectManager),
        PrismPersistableObject (UpgradeTestManagedObject6::getClassName (), WaveManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        m_integer6        (0)
{
}

UpgradeTestManagedObject6::~UpgradeTestManagedObject6 ()
{
}

string UpgradeTestManagedObject6::getClassName ()
{
    return ("UpgradeTestManagedObject6");
}

void UpgradeTestManagedObject6::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32                                              (&m_integer6,     "integer6"));
    addPersistableAttribute (new AttributeString                                            (&m_message6,     "message6"));
    addPersistableAttribute (new AttributeObjectId                                          (&m_objectId6,    "objectId6"));
    addPersistableAttribute (new AttributeManagedObjectComposition<UpgradeTestManagedObject1> (&m_composition6, "composition6", UpgradeTestManagedObject1::getClassName ()));
}

void UpgradeTestManagedObject6::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32                                              (&m_integer6,     "integer6"));
    addPersistableAttributeForCreate (new AttributeString                                            (&m_message6,     "message6"));
    addPersistableAttributeForCreate (new AttributeObjectId                                          (&m_objectId6,    "objectId6"));
    addPersistableAttributeForCreate (new AttributeManagedObjectComposition<UpgradeTestManagedObject1> (&m_composition6, "composition6", UpgradeTestManagedObject1::getClassName ()));
}

void UpgradeTestManagedObject6::setInteger6 (const UI32 &integer6)
{
    m_integer6 = integer6;
}

void UpgradeTestManagedObject6::setMessage6 (const string &message6)
{
    m_message6 = message6;
}

void UpgradeTestManagedObject6::setObjectId6 (const ObjectId &objectId6)
{
    m_objectId6 = objectId6;
}

void UpgradeTestManagedObject6::setComposition6 (const WaveManagedObjectPointer<UpgradeTestManagedObject1> &composition6)
{
    m_composition6 = composition6;
}

UI32 UpgradeTestManagedObject6::getInteger6 () const
{
    return (m_integer6);
}

string UpgradeTestManagedObject6::getMessage6 () const
{
    return (m_message6);
}

ObjectId UpgradeTestManagedObject6::getObjectId6 () const
{
    return (m_objectId6);
}

WaveManagedObjectPointer<UpgradeTestManagedObject1> UpgradeTestManagedObject6::getComposition6 () const
{
    return (m_composition6);
}

bool UpgradeTestManagedObject6::operator == (const UpgradeTestManagedObject6 &rhs) const
{
    if ((rhs.getObjectId ()) != ((getObjectId ())))
    {
        return (false);
    }

    if (rhs.m_integer6 != m_integer6)
    {
        return (false);
    }

    if (rhs.m_message6 != m_message6)
    {
        return (false);
    }

    if (rhs.m_objectId6 != m_objectId6)
    {
        return (false);
    }

    if (m_composition6 != rhs.m_composition6)
    {
        return (false);
    }

    return (true);
}

bool UpgradeTestManagedObject6::operator != (const UpgradeTestManagedObject6 &rhs) const
{
    return (!(operator == (rhs)));
}

}
