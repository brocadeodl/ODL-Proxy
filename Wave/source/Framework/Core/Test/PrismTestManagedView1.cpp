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

#include "Framework/Core/Test/PrismTestManagedView1.h"

namespace WaveNs
{

PrismTestManagedView1::PrismTestManagedView1 (WaveObjectManager *pWaveObjectManager)
    : PrismElement           (pWaveObjectManager),
      PrismPersistableObject (PrismTestManagedView1::getClassName ()),
      WaveManagedObject      (pWaveObjectManager),
      WaveManagedView        (pWaveObjectManager)
{   
}

PrismTestManagedView1::~PrismTestManagedView1 ()
{
}

string PrismTestManagedView1::getClassName ()
{
    return ("PrismTestManagedView1");
}

void PrismTestManagedView1::setupAttributesForPersistence ()
{
    addPersistableAttribute (new AttributeUI32     (&m_integer1,  "integer1"));
    addPersistableAttribute (new AttributeString   (&m_message1,  "message1"));
}

void PrismTestManagedView1::setupAttributesForCreate ()
{
    addPersistableAttributeForCreate (new AttributeUI32     (&m_integer1,  "integer1"));
    addPersistableAttributeForCreate (new AttributeString   (&m_message1,  "message1"));
}

string PrismTestManagedView1::getSqlForCreateView ()
{
    string sqlForCreateView = "CREATE OR REPLACE VIEW WaveCurrent.PrismTestManagedView1 AS SELECT integer1, message1 from  wavecurrent.PrismTestManagedObject1 UNION SELECT integer2, message2 from wavecurrent.PrismTestManagedObject2;";
    return sqlForCreateView;
}

void PrismTestManagedView1::setInteger1 (const UI32 &integer1)
{
    m_integer1 = integer1;
}

void PrismTestManagedView1::setMessage1 (const string &message1)
{
    m_message1 = message1;
}

UI32 PrismTestManagedView1::getInteger1 () const
{   
    return (m_integer1);
}

string PrismTestManagedView1::getMessage1 () const
{   
    return (m_message1);
}

}
