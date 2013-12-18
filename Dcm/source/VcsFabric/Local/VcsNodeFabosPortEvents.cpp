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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Shawn Green 	                                               *
 *   Adapted from VcsNodeFabosDomainEvents.cpp                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosPortEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"

namespace DcmNs
{

/*************************************************************************
* Port Disable Event
*************************************************************************/
VcsNodeFabosSwitchPortDisableEvent::VcsNodeFabosSwitchPortDisableEvent ()
    : PrismEvent (VcsNodeFabosInterfaceObjectManager::getPrismServiceId (), VCS_NODE_FABOS_PORT_DISABLE)
{
    m_portNumber = 0;
    m_reasonCode = 0;
}

VcsNodeFabosSwitchPortDisableEvent::~VcsNodeFabosSwitchPortDisableEvent ()
{
}

void VcsNodeFabosSwitchPortDisableEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_portNumber, "portNumber"));
     addSerializableAttribute (new AttributeUI32   (&m_reasonCode, "reasonCode"));
}

UI32 VcsNodeFabosSwitchPortDisableEvent::getPortNumber () const
{
    return (m_portNumber);
}

void VcsNodeFabosSwitchPortDisableEvent::setPortNumber (const UI32 &portNumber)
{
    m_portNumber = portNumber;
}

UI32 VcsNodeFabosSwitchPortDisableEvent::getDisableReasonCode () const
{
    return (m_reasonCode);
}

void VcsNodeFabosSwitchPortDisableEvent::setDisableReasonCode (const UI32 &reasonCode)
{
    m_reasonCode = reasonCode;
}

}
