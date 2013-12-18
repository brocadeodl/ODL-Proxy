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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : MITHUN BS                                                    *
 ***************************************************************************/

#include "Framework/Core/Test/WaveTestManagedObject12.h"

namespace WaveNs
{

WaveTestManagedObject12::WaveTestManagedObject12 (WaveObjectManager *pWaveObjectManager)
        : PrismElement (pWaveObjectManager),
        PrismPersistableObject (WaveTestManagedObject12::getClassName (), WaveManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        m_integer12        (0)
{
    setEmptyNeededOnPersistentBoot (true);
}

WaveTestManagedObject12::~WaveTestManagedObject12 ()
{
}

string WaveTestManagedObject12::getClassName ()
{
    return ("WaveTestManagedObject12");
}

void WaveTestManagedObject12::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32 (&m_integer12,"integer12"));
    addPersistableAttribute (new AttributeString (&m_message12,"message12"));
}

void WaveTestManagedObject12::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32 (&m_integer12,"integer12"));
    addPersistableAttributeForCreate (new AttributeString (&m_message12,"message12"));

    vector<string > keyName;
    keyName.push_back ("integer12");
    setUserDefinedKeyCombination (keyName);
}

void WaveTestManagedObject12::setInteger12 (const UI32 &integer12)
{
    m_integer12 = integer12;
}

void WaveTestManagedObject12::setMessage12 (const string &message12)
{
    m_message12 = message12;
}


UI32 WaveTestManagedObject12::getInteger12 () const
{
    return (m_integer12);
}

string WaveTestManagedObject12::getMessage12 () const
{
    return (m_message12);
}

}
