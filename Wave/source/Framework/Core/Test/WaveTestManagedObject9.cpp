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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Framework/Core/Test/WaveTestManagedObject9.h"

namespace WaveNs
{

WaveTestManagedObject9::WaveTestManagedObject9 (WaveObjectManager *pWaveObjectManager)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject (WaveTestManagedObject9::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject (pWaveObjectManager),
      m_integer9        (0)
{
}

WaveTestManagedObject9::~WaveTestManagedObject9 ()
{
}

string WaveTestManagedObject9::getClassName ()
{
    return ("WaveTestManagedObject9");
}

void WaveTestManagedObject9::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32 (&m_integer9,"integer9"));
    addPersistableAttribute (new AttributeUI32 (&m_integer9_1,"integer9_1")); // This is a dummy attribute not used in BIST.  This is used to force a schema change.
    addPersistableAttribute (new AttributeString (&m_message9,"message9"));
}

void WaveTestManagedObject9::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32 (&m_integer9,"integer9"));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_integer9_1,"integer9_1"));
    addPersistableAttributeForCreate (new AttributeString (&m_message9,"message9"));
}

void WaveTestManagedObject9::setInteger9 (const UI32 &integer9)
{
    m_integer9 = integer9;
}

void WaveTestManagedObject9::setMessage9 (const string &message9)
{
    m_message9 = message9;
}


UI32 WaveTestManagedObject9::getInteger9 () const
{
    return (m_integer9);
}

string WaveTestManagedObject9::getMessage9 () const
{
    return (m_message9);
}

}
