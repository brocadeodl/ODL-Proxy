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
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "LockManagement/LockManagedObject.h"
#include "Framework/Attributes/Attribute.h"

namespace WaveNs
{

LockManagedObject::LockManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject (LockManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject (pWaveObjectManager),
      m_locationId (0),
      m_serviceId (0)
{
}

LockManagedObject::LockManagedObject (WaveObjectManager *pWaveObjectManager, const LocationId &locationId, const string &serviceString, const PrismServiceId &serviceId)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject (LockManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject (pWaveObjectManager),
      m_locationId (locationId),
      m_serviceString (serviceString),
      m_serviceId (serviceId)    
{
}

LockManagedObject::~LockManagedObject ()
{
}

string LockManagedObject::getClassName ()
{
    return ("LockManagedObject");
}

void LockManagedObject::setupAttributesForPersistence ()
{
    WaveManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeUI32 (&m_locationId,   "locationId"));
    addPersistableAttribute (new AttributeString (&m_serviceString,   "serviceString"));
    addPersistableAttribute (new AttributeUI32 (&m_serviceId, "serviceId"));
}

void LockManagedObject::setupAttributesForCreate ()
{
    WaveManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeUI32 (&m_locationId,   "locationId"));
    addPersistableAttributeForCreate (new AttributeString (&m_serviceString,   "serviceString"));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_serviceId, "serviceId"));
}

LocationId LockManagedObject::getLocationId () const
{
    return (m_locationId);
}

const string& LockManagedObject::getServiceString () const
{
    return (m_serviceString);
}

PrismServiceId LockManagedObject::getServiceId () const
{
    return (m_serviceId);
}

void LockManagedObject::setLocationId (const LocationId locationId)
{
    m_locationId = locationId;
}

void LockManagedObject::setServiceString (const string &serviceString)
{
    m_serviceString = serviceString;
}

void LockManagedObject::setServiceId (const PrismServiceId serviceId)
{
    m_serviceId = serviceId;
}

void LockManagedObject::resetLockManagedObject ()
{
    m_locationId = 0;
    m_serviceString.clear ();
    m_serviceId = 0;
}

LockManagedObject &LockManagedObject::operator= (const LockManagedObject &lockManagedObject)
{
    m_locationId = lockManagedObject.m_locationId;
    m_serviceString = lockManagedObject.m_serviceString;
    m_serviceId = lockManagedObject.m_serviceId;

    return (*this);
}

}
