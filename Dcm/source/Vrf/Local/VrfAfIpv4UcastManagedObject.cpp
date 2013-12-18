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

#include "Vrf/Local/VrfAfIpv4UcastManagedObject.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "Vrf/Local/VrfLocalMessages.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "vcs.h"

namespace DcmNs
{

VrfAfIpv4UcastManagedObject::VrfAfIpv4UcastManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (VrfAfIpv4UcastManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
    m_ipv4PresenceFlag = true;
    m_maxRoutes = RTM_DEFAULT_MAX_ROUTES;
}

VrfAfIpv4UcastManagedObject::VrfAfIpv4UcastManagedObject (WaveObjectManager *pWaveObjectManager, VrfLocalAddVrfMessage  *pMessage )
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (VrfAfIpv4UcastManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
    m_vrfName = pMessage->getVrfName();
    if(pMessage->getCmdCode() == RTM_MSG_AF_IPV4_SET)
        m_ipv4PresenceFlag = pMessage->getVrfIpv4Flag();
    if(pMessage->getCmdCode() == RTM_MSG_AF_IPV4_SET || pMessage->getCmdCode() ==  RTM_MSG_CONFIG_MAX_ROUTES_SET)
        m_maxRoutes = pMessage->getMaxRoutes();
}

VrfAfIpv4UcastManagedObject::~VrfAfIpv4UcastManagedObject ()
{
}

void VrfAfIpv4UcastManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_vrfName, "vrfName"));
    addPersistableAttribute (new AttributeBool (&m_ipv4PresenceFlag, "vrfAfIpv4", vrf_ipv4));
    addPersistableAttribute (new AttributeUI32 (&m_maxRoutes, "maxRoutes", vrf_max_route));
    addPersistableAttribute(new AttributeManagedObjectVectorComposition<VrfRouteTargetAfManagedObject>(&m_routeTarget,"vrfRouteTarget", getClassName (), getObjectId (),VrfRouteTargetAfManagedObject::getClassName ()));


}

void VrfAfIpv4UcastManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate(new AttributeString (&m_vrfName, "vrfName"));
    addPersistableAttributeForCreate(new AttributeBool (&m_ipv4PresenceFlag, "vrfAfIpv4", vrf_ipv4));
    addPersistableAttributeForCreate(new AttributeUI32 (&m_maxRoutes, "maxRoutes", vrf_max_route));
    addPersistableAttributeForCreate(new AttributeManagedObjectVectorComposition<VrfRouteTargetAfManagedObject>(&m_routeTarget,"vrfRouteTarget", getClassName (), getObjectId (),VrfRouteTargetAfManagedObject::getClassName ()));
    vector<string > keyName;
    keyName.push_back ("maxRoutes");
    setUserDefinedKeyCombination (keyName);
}


void VrfAfIpv4UcastManagedObject::setRt(VrfRouteTargetAfManagedObject *mo)
{
    WaveManagedObjectPointer<VrfRouteTargetAfManagedObject> rt (mo);
    m_routeTarget.push_back(rt);
}

string VrfAfIpv4UcastManagedObject::getClassName ()
{
    return ("VrfAfIpv4UcastManagedObject");
}

}
