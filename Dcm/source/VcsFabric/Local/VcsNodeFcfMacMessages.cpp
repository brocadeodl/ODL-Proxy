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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeFcfMacMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

/************************************ 
* SET FCF MAC 
*************************************/

VcsNodeSetFcfMacMessage::VcsNodeSetFcfMacMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_FCFMAC)
{
}

VcsNodeSetFcfMacMessage::VcsNodeSetFcfMacMessage (const MacAddress &fcfMac)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_FCFMAC),
      m_fcfMac                 (fcfMac)
{
}

VcsNodeSetFcfMacMessage::~VcsNodeSetFcfMacMessage ()
{
}

void VcsNodeSetFcfMacMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeMacAddress (&m_fcfMac, "fcfMac"));
}

void VcsNodeSetFcfMacMessage::setFcfMac(const MacAddress &fcfMac)
{
    m_fcfMac = fcfMac;
}

MacAddress VcsNodeSetFcfMacMessage::getFcfMac() const
{
    return (m_fcfMac);
}



/************************************ 
* SET FCF MAC SERVICE INDEPENDENT
*************************************/

VcsNodeSetFcfMacMessageServiceIndependent::VcsNodeSetFcfMacMessageServiceIndependent ()
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_SET_FCF_MAC_ADDRESS_SERVICE_INDEPENDENT)
{
}

VcsNodeSetFcfMacMessageServiceIndependent::VcsNodeSetFcfMacMessageServiceIndependent (const MacAddress &fcfMac)
    : ManagementInterfaceServiceIndependentMessage (VcsFabricLocalObjectManager::getClassName (), VCS_SET_FCF_MAC_ADDRESS_SERVICE_INDEPENDENT),
      m_fcfMac                 (fcfMac)
{
}

VcsNodeSetFcfMacMessageServiceIndependent::~VcsNodeSetFcfMacMessageServiceIndependent ()
{
}

void VcsNodeSetFcfMacMessageServiceIndependent::setupAttributesForSerialization ()
{
    ManagementInterfaceServiceIndependentMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeMacAddress (&m_fcfMac, "fcfMac"));
}

void VcsNodeSetFcfMacMessageServiceIndependent::setFcfMac(const MacAddress &fcfMac)
{
    m_fcfMac = fcfMac;
}

MacAddress VcsNodeSetFcfMacMessageServiceIndependent::getFcfMac() const
{
    return (m_fcfMac);
}

}
