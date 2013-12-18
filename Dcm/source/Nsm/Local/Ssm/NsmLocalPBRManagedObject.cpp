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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Debojyoti                                                    *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "vcs.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "Nsm/Local/Ssm/NsmLocalPBRManagedObject.h"


namespace DcmNs
{

    NsmLocalPBRManagedObject::NsmLocalPBRManagedObject
        (WaveObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (NsmLocalPBRManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    NsmLocalPBRManagedObject::NsmLocalPBRManagedObject
        (WaveObjectManager *pNsmLocalObjectManager, const string &routeMapName)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (NsmLocalPBRManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_routeMapName    (routeMapName)
    {
    }

    NsmLocalPBRManagedObject::~NsmLocalPBRManagedObject()
    {
    }

    string  NsmLocalPBRManagedObject::getClassName()
    {
        return ("NsmLocalPBRManagedObject");
    }

    void  NsmLocalPBRManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence();
        //addPersistableAttribute (new AttributeObjectIdAssociation(&m_routeMapName, "routeMapName", RouteMapLocalManagedObject::getClassName(), true));
		addPersistableAttribute (new AttributeString(&m_routeMapName, "routeMapName")); 
        setUserTagForAttribute ("routeMapName", ip_policy_route_map_name);
    }

    void  NsmLocalPBRManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        //addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_routeMapName, "routeMapName", RouteMapLocalManagedObject::getClassName(), true));
		addPersistableAttributeForCreate (new AttributeString(&m_routeMapName, "routeMapName"));
    }

	/*
    void  NsmLocalPBRManagedObject::setupKeys() {
        vector<string> keyName;
        keyName.push_back("routeMapName");
        setUserDefinedKeyCombination (keyName);
    } */

    string  NsmLocalPBRManagedObject::getRouteMapName() const
    {
        return (m_routeMapName);
    }

    void  NsmLocalPBRManagedObject::setRouteMapName(const string &routeMap)
    {
        m_routeMapName = routeMap;
    }


} // DcmNs

