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

#include "Framework/Core/Test/WaveTestManagedObject6.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"

namespace WaveNs
{

WaveTestManagedObject6::WaveTestManagedObject6 (WaveObjectManager *pWaveObjectManager)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject (WaveTestManagedObject6::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject (pWaveObjectManager),
      m_integer6        (0)
{
}

WaveTestManagedObject6::~WaveTestManagedObject6 ()
{
}

string WaveTestManagedObject6::getClassName ()
{
    return ("WaveTestManagedObject6");
}

void WaveTestManagedObject6::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32                                              (&m_integer6,     "integer6"));
    addPersistableAttribute (new AttributeString                                            (&m_message6,     "message6"));
    addPersistableAttribute (new AttributeObjectId                                          (&m_objectId6,    "objectId6"));
    addPersistableAttribute (new AttributeManagedObjectComposition<PrismTestManagedObject1> (&m_composition6, "composition6", PrismTestManagedObject1::getClassName ()));
}

void WaveTestManagedObject6::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32                                              (&m_integer6,     "integer6"));
    addPersistableAttributeForCreate (new AttributeString                                            (&m_message6,     "message6"));
    addPersistableAttributeForCreate (new AttributeObjectId                                          (&m_objectId6,    "objectId6"));
    addPersistableAttributeForCreate (new AttributeManagedObjectComposition<PrismTestManagedObject1> (&m_composition6, "composition6", PrismTestManagedObject1::getClassName ()));
}

void WaveTestManagedObject6::setInteger6 (const UI32 &integer6)
{
    m_integer6 = integer6;
}

void WaveTestManagedObject6::setMessage6 (const string &message6)
{
    m_message6 = message6;
}

void WaveTestManagedObject6::setObjectId6 (const ObjectId &objectId6)
{
    m_objectId6 = objectId6;
}

void WaveTestManagedObject6::setComposition6 (const WaveManagedObjectPointer<PrismTestManagedObject1> &composition6)
{
    m_composition6 = composition6;
}

UI32 WaveTestManagedObject6::getInteger6 () const
{
    return (m_integer6);
}

string WaveTestManagedObject6::getMessage6 () const
{
    return (m_message6);
}

ObjectId WaveTestManagedObject6::getObjectId6 () const
{
    return (m_objectId6);
}

WaveManagedObjectPointer<PrismTestManagedObject1> WaveTestManagedObject6::getComposition6 () const
{
    return (m_composition6);
}

bool WaveTestManagedObject6::operator == (const WaveTestManagedObject6 &rhs) const
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

bool WaveTestManagedObject6::operator != (const WaveTestManagedObject6 &rhs) const
{
    return (!(operator == (rhs)));
}

}
