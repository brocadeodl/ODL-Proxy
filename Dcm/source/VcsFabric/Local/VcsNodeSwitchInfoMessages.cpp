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

#include "VcsFabric/Local/VcsNodeSwitchInfoMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeUpdateSwitchInfoMessage::VcsNodeUpdateSwitchInfoMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_UPDATE_SWITCH_INFO),
      m_domainId                      (0),
      m_SWBDModel                     (0)
{
}

    
VcsNodeUpdateSwitchInfoMessage::VcsNodeUpdateSwitchInfoMessage (const string &chassisString, const MacAddress &switchMac, const SI32 domainId, const WorldWideName &switchWWN, const SI32 SWBDModel, const string &fwVersion, const string &swmodelName)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_UPDATE_SWITCH_INFO),
      m_chassisString                 (chassisString),
      m_switchMac                     (switchMac),
      m_domainId                      (domainId),
      m_switchWWN                     (switchWWN),
      m_SWBDModel                     (SWBDModel),
      m_fwVersion                     (fwVersion),
      m_swmodelName                   (swmodelName)
{
}

VcsNodeUpdateSwitchInfoMessage::~VcsNodeUpdateSwitchInfoMessage ()
{
}

void VcsNodeUpdateSwitchInfoMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString          (&m_chassisString,  "chassisString"));
     addSerializableAttribute (new AttributeMacAddress      (&m_switchMac,      "switchMac"));
     addSerializableAttribute (new AttributeSI32            (&m_domainId,       "domainId"));
     addSerializableAttribute (new AttributeWorldWideName   (&m_switchWWN,      "switchWWN"));
     addSerializableAttribute (new AttributeSI32            (&m_SWBDModel,      "SWBDModel"));
     addSerializableAttribute (new AttributeString          (&m_fwVersion,      "fwVersion"));
     addSerializableAttribute (new AttributeString          (&m_swmodelName,    "swmodelName"));
}

string VcsNodeUpdateSwitchInfoMessage::getChassisString ()
{
    return (m_chassisString);
}

MacAddress VcsNodeUpdateSwitchInfoMessage::getSwitchMac ()
{
    return (m_switchMac);
}

SI32 VcsNodeUpdateSwitchInfoMessage::getDomainId ()
{
    return (m_domainId);
}

WorldWideName VcsNodeUpdateSwitchInfoMessage::getSwitchWWN ()
{
    return (m_switchWWN);
}

SI32 VcsNodeUpdateSwitchInfoMessage::getSWBDModel ()
{
    return (m_SWBDModel);
}

string VcsNodeUpdateSwitchInfoMessage::getFwVersion ()
{
    return (m_fwVersion);
}

string VcsNodeUpdateSwitchInfoMessage::getSwmodelName () const
{
    return (m_swmodelName);
}

}



