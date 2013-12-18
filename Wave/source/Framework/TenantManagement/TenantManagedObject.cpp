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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/TenantManagement/TenantManagedObject.h"

namespace WaveNs
{

TenantManagedObject::TenantManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement           (pWaveObjectManager),
      PrismPersistableObject (TenantManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject      (pWaveObjectManager)
{
}

TenantManagedObject::~TenantManagedObject ()
{
}

string TenantManagedObject::getClassName ()
{
    return ("TenantManagedObject");
}

void TenantManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32 (&m_id, "id"));
}

void TenantManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32 (&m_id, "id"));
}

UI32 TenantManagedObject::getId () const
{
    return (m_id);
}

void TenantManagedObject::setId (const UI32 &id)
{
    m_id = id;
}

}
