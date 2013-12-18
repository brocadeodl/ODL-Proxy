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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "Zone/Global/EffectiveZoneManagedObject.h"
#include "Zone/Global/ZoneObjectManager.h"
#include "Zone/Global/EffectiveZoneMemberManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Zone/Global/ZoneTypes.h"

#include "vcs.h"
/*#include "brocade-zone.h"*/

namespace DcmNs
{

    EffectiveZoneManagedObject::EffectiveZoneManagedObject (ZoneObjectManager *pZoneObjectManager)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (EffectiveZoneManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager)
    {
    }

    EffectiveZoneManagedObject::EffectiveZoneManagedObject (ZoneObjectManager *pZoneObjectManager,const string &effectiveZoneName,const vector<WaveManagedObjectPointer<EffectiveZoneMemberManagedObject> > &effectiveZoneMembers)
        : PrismElement  (pZoneObjectManager),
        PrismPersistableObject (EffectiveZoneManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pZoneObjectManager),
        DcmManagedObject (pZoneObjectManager),
        m_effectiveZoneName    (effectiveZoneName),
        m_effectiveZoneMembers    (effectiveZoneMembers)
    {
    }

    EffectiveZoneManagedObject::~EffectiveZoneManagedObject ()
    {
    }

    string  EffectiveZoneManagedObject::getClassName()
    {
        return ("EffectiveZoneManagedObject");
    }

    void  EffectiveZoneManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_effectiveZoneName,"effectiveZoneName"));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<EffectiveZoneMemberManagedObject>(&m_effectiveZoneMembers,"effectiveZoneMembers", getClassName (), getObjectId (), EffectiveZoneMemberManagedObject::getClassName ()));

	    setUserTagForAttribute ("effectiveZoneName", brocade_zone_zone_name);
    }

    void  EffectiveZoneManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_effectiveZoneName,"effectiveZoneName"));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<EffectiveZoneMemberManagedObject>(&m_effectiveZoneMembers,"effectiveZoneMembers", getClassName (), getObjectId (), EffectiveZoneMemberManagedObject::getClassName ()));

	    setDisableValidations ("effectiveZoneMembers", true);
	    vector<string> keyName;
	    keyName.push_back ("effectiveZoneName");
	    setUserDefinedKeyCombination (keyName);
    }

    void  EffectiveZoneManagedObject::setEffectiveZoneName(const string &effectiveZoneName)
    {
        m_effectiveZoneName  =  effectiveZoneName;
    }

    string  EffectiveZoneManagedObject::getEffectiveZoneName() const
    {
        return (m_effectiveZoneName);
    }

    void  EffectiveZoneManagedObject::setEffectiveZoneMembers(const vector<WaveManagedObjectPointer<EffectiveZoneMemberManagedObject> > &effectiveZoneMembers)
    {
        m_effectiveZoneMembers  =  effectiveZoneMembers;
    }

    vector<WaveManagedObjectPointer<EffectiveZoneMemberManagedObject> >  EffectiveZoneManagedObject::getEffectiveZoneMembers() const
    {
        return (m_effectiveZoneMembers);
    }

}
