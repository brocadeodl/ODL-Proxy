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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/Test/PrismTestManagedObject4.h"

namespace WaveNs
{

PrismTestManagedObject4::PrismTestManagedObject4 (WaveObjectManager *pWaveObjectManager)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject (PrismTestManagedObject4::getClassName (), PrismTestManagedObject1::getClassName ()),
      PrismTestManagedObject1 (pWaveObjectManager),
      m_integer4              (0)
{
}

PrismTestManagedObject4::~PrismTestManagedObject4 ()
{
}

string PrismTestManagedObject4::getClassName ()
{
    return ("PrismTestManagedObject4");
}

void PrismTestManagedObject4::setupAttributesForPersistence ()
{
    PrismTestManagedObject1::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32                      (&m_integer4,           "integer4"));
    addPersistableAttribute (new AttributeString                    (&m_message4,           "message4"));
    addPersistableAttribute (new AttributeObjectId                  (&m_objectId4,          "objectId4"));
    addPersistableAttribute (new AttributeObjectIdAssociation       (&m_association4,       "association4", PrismTestManagedObject1::getClassName ()));
    addPersistableAttribute (new AttributeObjectIdVector            (&m_objectIdVector4,    "objectIdVector4"));
    addPersistableAttribute (new AttributeObjectIdVectorAssociation (&m_associationVector4, "associationVector4", getClassName (), getObjectId (), PrismTestManagedObject1::getClassName ()));
}

void PrismTestManagedObject4::setupAttributesForCreate ()
{
    addPersistableAttributeForCreate (new AttributeUI32                      (&m_integer4,           "integer4"));
    addPersistableAttributeForCreate (new AttributeString                    (&m_message4,           "message4"));
    addPersistableAttributeForCreate (new AttributeObjectId                  (&m_objectId4,          "objectId4"));
    addPersistableAttributeForCreate (new AttributeObjectIdAssociation       (&m_association4,       "association4", PrismTestManagedObject1::getClassName ()));
    addPersistableAttributeForCreate (new AttributeObjectIdVector            (&m_objectIdVector4,    "objectIdVector4"));
    addPersistableAttributeForCreate (new AttributeObjectIdVectorAssociation (&m_associationVector4, "associationVector4", getClassName (), getObjectId (), PrismTestManagedObject1::getClassName ()));
}

void PrismTestManagedObject4::setInteger4 (const UI32 &integer4)
{
    m_integer4 = integer4;
}

void PrismTestManagedObject4::setMessage4 (const string &message4)
{
    m_message4 = message4;
}

void PrismTestManagedObject4::setObjectId4 (const ObjectId &objectId4)
{
    m_objectId4 = objectId4;
}

void PrismTestManagedObject4::setAssociation4 (const ObjectId &association4)
{
    m_association4 = association4;
}

void PrismTestManagedObject4::setObjectIdVector4 (const vector<ObjectId> &objectVector4)
{
    m_objectIdVector4 = objectVector4;
}

void PrismTestManagedObject4::setAssociationVector4 (const vector<ObjectId> &objectVector4)
{
    m_associationVector4 = objectVector4;
}

UI32 PrismTestManagedObject4::getInteger4 () const
{
    return (m_integer4);
}

string PrismTestManagedObject4::getMessage4 () const
{
    return (m_message4);
}

ObjectId PrismTestManagedObject4::getObjectId4 () const
{
    return (m_objectId4);
}

ObjectId PrismTestManagedObject4::getAssociation4 () const
{
    return (m_association4);
}

vector<ObjectId> PrismTestManagedObject4::getObjectIdVector4 () const
{
    return (m_objectIdVector4);
}

vector<ObjectId> PrismTestManagedObject4::getAssociationVector4 () const
{
    return (m_associationVector4);
}

bool PrismTestManagedObject4::operator == (const PrismTestManagedObject4 &rhs) const
{
    bool isEqual = PrismTestManagedObject1::operator == (rhs);

    if (false == isEqual)
    {
        return (false);
    }

    if (rhs.m_integer4 != m_integer4)
    {
        return (false);
    }

    if (rhs.m_message4 != m_message4)
    {
        return (false);
    }

    if (rhs.m_objectId4 != m_objectId4)
    {
        return (false);
    }

    if (rhs.m_association4 != m_association4)
    {
        return (false);
    }

    if (rhs.m_objectIdVector4 != m_objectIdVector4)
    {
        return (false);
    }

    if (rhs.m_associationVector4 != m_associationVector4)
    {
        return (false);
    }

    return (true);
}

bool PrismTestManagedObject4::operator != (const PrismTestManagedObject4 &rhs) const
{
    return (!(operator == (rhs)));
}

}
