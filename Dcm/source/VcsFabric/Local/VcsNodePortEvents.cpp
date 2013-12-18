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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodePortEvents.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

/*************************************************************************
* Port Disable Event
*************************************************************************/
VcsNodeSwitchPortDisableEvent::VcsNodeSwitchPortDisableEvent ()
    : PrismEvent (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_PORT_DISABLE)
{
    m_portNumber = 0;
    m_reasonCode = 0;
}

VcsNodeSwitchPortDisableEvent::~VcsNodeSwitchPortDisableEvent ()
{
}

void VcsNodeSwitchPortDisableEvent::setupAttributesForSerialization ()
{
    PrismEvent::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32   (&m_portNumber, "portNumber"));
     addSerializableAttribute (new AttributeUI32   (&m_reasonCode, "reasonCode"));
}

UI32 VcsNodeSwitchPortDisableEvent::getPortNumber () const
{
    return (m_portNumber);
}

void VcsNodeSwitchPortDisableEvent::setPortNumber (const UI32 &portNumber)
{
    m_portNumber = portNumber;
}

UI32 VcsNodeSwitchPortDisableEvent::getDisableReasonCode () const
{
    return (m_reasonCode);
}

void VcsNodeSwitchPortDisableEvent::setDisableReasonCode (const UI32 &reasonCode)
{
    m_reasonCode = reasonCode;
}

}
