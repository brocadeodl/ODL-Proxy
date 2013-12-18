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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#include "Vrf/Local/VrfLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"

namespace DcmNs
{

VrfLocalManagedObject::VrfLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (VrfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
    m_rd="";
}

VrfLocalManagedObject::~VrfLocalManagedObject ()
{
}

void VrfLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_vrfName, "vrfName", vrf_vrf_name));
    addPersistableAttribute (new AttributeString(&m_rd,true,"","rd", vrf_route_distiniguisher));
    IpV4Address routerID;
    addPersistableAttribute (new AttributeIpV4AddressUC(&m_routerId,true,false,routerID,"routerId", vrf_vrf_router_id));
    addPersistableAttribute (new AttributeManagedObjectComposition<VrfAfIpv4UcastManagedObject>(&m_vrfAfIpv4, "vrfAfIpv4", VrfAfIpv4UcastManagedObject::getClassName(), true));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition<VrfRouteTargetManagedObject>(&m_routeTarget,"vrfRouteTarget", getClassName (), getObjectId (),VrfRouteTargetManagedObject::getClassName ()));

}

void VrfLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString (&m_vrfName, "vrfName"));
    addPersistableAttributeForCreate(new AttributeString (&m_rd,true,"","rd", vrf_route_distiniguisher));
    IpV4Address routerID;
    addPersistableAttributeForCreate(new AttributeIpV4AddressUC(&m_routerId,true,false,routerID,"routerId", vrf_vrf_router_id));
    addPersistableAttributeForCreate(new AttributeManagedObjectComposition<VrfAfIpv4UcastManagedObject>(&m_vrfAfIpv4, "vrfAfIpv4", VrfAfIpv4UcastManagedObject::getClassName(), true));
    addPersistableAttributeForCreate(new AttributeManagedObjectVectorComposition<VrfRouteTargetManagedObject>(&m_routeTarget,"vrfRouteTarget", getClassName (), getObjectId (),VrfRouteTargetManagedObject::getClassName ()));

    vector<string> userDefinedKeyCombination;
    userDefinedKeyCombination.push_back ("vrfName");
    setUserDefinedKeyCombination (userDefinedKeyCombination);
}

string VrfLocalManagedObject::getClassName ()
{
    return ("VrfLocalManagedObject");
}

void VrfLocalManagedObject::setVrfName (const string &vrfName)
{
    m_vrfName = vrfName;
}

string VrfLocalManagedObject::getVrfName (void) const
{
    return m_vrfName;
}

void VrfLocalManagedObject::setRd (const string &rd, bool userCfg)
{
    m_rd= rd;
}

string VrfLocalManagedObject::getRd (void) const
{
    return m_rd;
}
#if 0
WaveManagedObjectPointer<VrfAfIpv4UcastManagedObject> VrfLocalManagedObject::getVrfAfIpv4() const 
{
    return m_vrfAfIpv4;
}
#endif

void VrfLocalManagedObject::setVrfAfIpv4(const WaveManagedObjectPointer<VrfAfIpv4UcastManagedObject> &vrfAfIpv4Mo)
{
    m_vrfAfIpv4 = vrfAfIpv4Mo; 
}

//this funtion also update vrfMo
void VrfLocalManagedObject::setVrfAfIpv4(VrfAfIpv4UcastManagedObject *mo)
{
    setVrfAfIpv4(WaveManagedObjectPointer<VrfAfIpv4UcastManagedObject> (mo));
   // updateWaveManagedObject(this);
}

void VrfLocalManagedObject::setRt(VrfRouteTargetManagedObject *mo)
{
    WaveManagedObjectPointer<VrfRouteTargetManagedObject> rt (mo);
    m_routeTarget.push_back(rt);
}
VrfAfIpv4UcastManagedObject *  VrfLocalManagedObject::getVrfAfIpv4(void) 
{
    return m_vrfAfIpv4.operator->();
}
}
