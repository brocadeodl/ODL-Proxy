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

#include "Vrf/Local/VrfRouteTargetManagedObject.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "Vrf/Local/VrfLocalMessages.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeEnumUC.h"
#include "Framework/Attributes/Attributes.h"
#include "vcs.h"

namespace DcmNs
{

VrfRouteTargetManagedObject::VrfRouteTargetManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (VrfRouteTargetManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
    m_vrfName = "";
    m_rt = "";
    m_rtType =  RTM_MSG_CONFIG_RT_TYPE_UNKNOWN;
}

VrfRouteTargetManagedObject::VrfRouteTargetManagedObject (WaveObjectManager *pWaveObjectManager, VrfLocalAddVrfMessage  *pMessage )
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (VrfRouteTargetManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
    m_vrfName = pMessage->getVrfName();
    if(pMessage->getCmdCode() == RTM_MSG_CONFIG_VRF_MODE_RT_SET)
    {
        m_rt = pMessage->getRt();
        m_rtType = pMessage->getRtType();
    }
}


VrfRouteTargetManagedObject::~VrfRouteTargetManagedObject ()
{
}

void VrfRouteTargetManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_vrfName, "vrfName"));
    addPersistableAttribute (new AttributeEnum   ((UI32*)&m_rtType, "vrfRtType", vrf_action));
    addPersistableAttribute (new AttributeString (&m_rt, "vrfRouteTarget", vrf_target_community));

}

void VrfRouteTargetManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate(new AttributeString (&m_vrfName, "vrfName"));
    addPersistableAttributeForCreate(new AttributeEnum   ((UI32*)&m_rtType, "vrfRtType", vrf_ipv4));
    addPersistableAttributeForCreate(new AttributeString (&m_rt, "vrfRouteTarget", vrf_max_route));

    vector<string> userDefinedKeyCombination;
    userDefinedKeyCombination.push_back ("vrfRtType");
    userDefinedKeyCombination.push_back ("vrfRouteTarget");
    setUserDefinedKeyCombination (userDefinedKeyCombination);
}


string VrfRouteTargetManagedObject:: getClassName ()              
{ 
    return "VrfRouteTarget";
}

VrfRouteTargetAfManagedObject::VrfRouteTargetAfManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (VrfRouteTargetAfManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
    m_vrfName = "";
    m_rt = "";
    m_rtType =  RTM_MSG_CONFIG_RT_TYPE_UNKNOWN;
}

VrfRouteTargetAfManagedObject::VrfRouteTargetAfManagedObject (WaveObjectManager *pWaveObjectManager, VrfLocalAddVrfMessage  *pMessage )
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (VrfRouteTargetAfManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
    m_vrfName = pMessage->getVrfName();
    if(pMessage->getCmdCode() == RTM_MSG_CONFIG_VRF_MODE_RT_SET)
    {
        m_rt = pMessage->getRt();
        m_rtType = pMessage->getRtType();
    }
}

VrfRouteTargetAfManagedObject::~VrfRouteTargetAfManagedObject ()
{
}

void VrfRouteTargetAfManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString (&m_vrfName, "vrfName"));
    addPersistableAttribute (new AttributeEnum   ((UI32*)&m_rtType, "vrfRtType", vrf_action));
    addPersistableAttribute (new AttributeString (&m_rt, "vrfRouteTarget", vrf_target_community));

}

void VrfRouteTargetAfManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate(new AttributeString (&m_vrfName, "vrfName"));
    addPersistableAttributeForCreate(new AttributeEnum   ((UI32*)&m_rtType, "vrfRtType", vrf_ipv4));
    addPersistableAttributeForCreate(new AttributeString (&m_rt, "vrfRouteTarget", vrf_max_route));

    vector<string> userDefinedKeyCombination;
    userDefinedKeyCombination.push_back ("vrfRtType");
    userDefinedKeyCombination.push_back ("vrfRouteTarget");
    setUserDefinedKeyCombination (userDefinedKeyCombination);
}

string VrfRouteTargetAfManagedObject:: getClassName ()              
{ 
    return "VrfRouteTargetAf";
}
}
