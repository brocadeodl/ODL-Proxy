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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject3.h"
#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject2.h"

namespace WaveNs
{

UpgradeTestManagedObject3::UpgradeTestManagedObject3 (WaveObjectManager *pWaveObjectManager)
        : PrismElement (pWaveObjectManager),
        PrismPersistableObject (UpgradeTestManagedObject3::getClassName (), UpgradeTestManagedObject1::getClassName ()),
        UpgradeTestManagedObject1 (pWaveObjectManager),
        m_integer3              (0)
{
}

UpgradeTestManagedObject3::~UpgradeTestManagedObject3 ()
{
}

string UpgradeTestManagedObject3::getClassName ()
{
    return ("UpgradeTestManagedObject3");
}

void UpgradeTestManagedObject3::setupAttributesForPersistence ()
{
    UpgradeTestManagedObject1::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32                (&m_integer3,     "integer3"));
    addPersistableAttribute (new AttributeString              (&m_message3,     "message3"));
    addPersistableAttribute (new AttributeObjectId            (&m_objectId3,    "objectId3"));
    addPersistableAttribute (new AttributeObjectIdAssociation (&m_association3, "association3", UpgradeTestManagedObject1::getClassName ()));
}

void UpgradeTestManagedObject3::setupAttributesForCreate ()
{
    addPersistableAttributeForCreate (new AttributeUI32                (&m_integer3,     "integer3"));
    addPersistableAttributeForCreate (new AttributeString              (&m_message3,     "message3"));
    addPersistableAttributeForCreate (new AttributeObjectId            (&m_objectId3,    "objectId3"));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation (&m_association3, "association3", UpgradeTestManagedObject1::getClassName ()));
}

void UpgradeTestManagedObject3::setInteger3 (const UI32 &integer3)
{
    m_integer3 = integer3;
}

void UpgradeTestManagedObject3::setMessage3 (const string &message3)
{
    m_message3 = message3;
}

void UpgradeTestManagedObject3::setObjectId3 (const ObjectId &objectId3)
{
    m_objectId3 = objectId3;
}

void UpgradeTestManagedObject3::setAssociation3 (const ObjectId &association3)
{
    m_association3 = association3;
}

UI32 UpgradeTestManagedObject3::getInteger3 () const
{
    return (m_integer3);
}

string UpgradeTestManagedObject3::getMessage3 () const
{
    return (m_message3);
}

ObjectId UpgradeTestManagedObject3::getObjectId3 () const
{
    return (m_objectId3);
}

ObjectId UpgradeTestManagedObject3::getAssociation3 () const
{
    return (m_association3);
}

bool UpgradeTestManagedObject3::operator == (const UpgradeTestManagedObject3 &rhs) const
{
    bool isEqual = UpgradeTestManagedObject1::operator == (rhs);

    if (false == isEqual)
    {
        return (false);
    }

    if (rhs.m_integer3 != m_integer3)
    {
        return (false);
    }

    if (rhs.m_message3 != m_message3)
    {
        return (false);
    }

    if (rhs.m_objectId3 != m_objectId3)
    {
        return (false);
    }

    if (rhs.m_association3 != m_association3)
    {
        return (false);
    }

    return (true);
}

bool UpgradeTestManagedObject3::operator != (const UpgradeTestManagedObject3 &rhs) const
{
    return (!(operator == (rhs)));
}

}
