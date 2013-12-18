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

#include "Framework/Core/Test/PrismTestManagedObject1.h"

namespace WaveNs
{

PrismTestManagedObject1::PrismTestManagedObject1 (WaveObjectManager *pWaveObjectManager)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject (PrismTestManagedObject1::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject (pWaveObjectManager),
      m_integer1        (0)
{
}

PrismTestManagedObject1::~PrismTestManagedObject1 ()
{
}

string PrismTestManagedObject1::getClassName ()
{
    return ("PrismTestManagedObject1");
}

void PrismTestManagedObject1::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32     (&m_integer1,  "integer1"));
    addPersistableAttribute (new AttributeString   (&m_message1,  "message1"));
    addPersistableAttribute (new AttributeObjectId (&m_objectId1, "objectId1"));
}

void PrismTestManagedObject1::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32     (&m_integer1,  "integer1"));
    addPersistableAttributeForCreate (new AttributeString   (&m_message1,  "message1"));
    addPersistableAttributeForCreate (new AttributeObjectId (&m_objectId1, "objectId1"));
}

void PrismTestManagedObject1::setInteger1 (const UI32 &integer1)
{
    m_integer1 = integer1;
}

void PrismTestManagedObject1::setMessage1 (const string &message1)
{
    m_message1 = message1;
}

void PrismTestManagedObject1::setObjectId1 (const ObjectId &objectId1)
{
    m_objectId1 = objectId1;
}

UI32 PrismTestManagedObject1::getInteger1 () const
{
    return (m_integer1);
}

string PrismTestManagedObject1::getMessage1 () const
{
    return (m_message1);
}

ObjectId PrismTestManagedObject1::getObjectId1 () const
{
    return (m_objectId1);
}

bool PrismTestManagedObject1::operator == (const PrismTestManagedObject1 &rhs) const
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

bool PrismTestManagedObject1::operator != (const PrismTestManagedObject1 &rhs) const
{
    return (!(operator == (rhs)));
}

}
