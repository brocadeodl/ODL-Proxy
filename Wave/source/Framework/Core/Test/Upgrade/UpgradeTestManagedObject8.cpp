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

#include "Framework/Core/Test/Upgrade/UpgradeTestManagedObject8.h"

namespace WaveNs
{

UpgradeTestManagedObject8::UpgradeTestManagedObject8 (WaveObjectManager *pWaveObjectManager)
        : PrismElement (pWaveObjectManager),
        PrismPersistableObject (UpgradeTestManagedObject8::getClassName (), WaveManagedObject::getClassName ()),
        WaveManagedObject (pWaveObjectManager),
        m_integer8        (0)
{
}

UpgradeTestManagedObject8::~UpgradeTestManagedObject8 ()
{
}

string UpgradeTestManagedObject8::getClassName ()
{
    return ("UpgradeTestManagedObject8");
}

void UpgradeTestManagedObject8::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32 (&m_integer8,"integer8"));
    addPersistableAttribute (new AttributeString (&m_message8,"message8"));
}

void UpgradeTestManagedObject8::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32 (&m_integer8,"integer8"));
    addPersistableAttributeForCreate (new AttributeString (&m_message8,"message8"));
}

void UpgradeTestManagedObject8::setInteger8 (const UI32 &integer8)
{
    m_integer8 = integer8;
}

void UpgradeTestManagedObject8::setMessage8 (const string &message8)
{
    m_message8 = message8;
}


UI32 UpgradeTestManagedObject8::getInteger8 () const
{
    return (m_integer8);
}

string UpgradeTestManagedObject8::getMessage8 () const
{
    return (m_message8);
}

}
