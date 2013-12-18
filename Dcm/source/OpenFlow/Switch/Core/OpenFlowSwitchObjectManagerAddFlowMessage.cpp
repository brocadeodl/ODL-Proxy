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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Kausik Sridharabalan                                         *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddFlowMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"
#include "Framework/Attributes/AttributeUri.h"


namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerAddFlowMessage::OpenFlowSwitchObjectManagerAddFlowMessage ()
    : ManagementInterfaceMessage (OpenFlowSwitchObjectManager::getServiceName (), OPEN_FLOW_OBJECT_MANAGER_ADD_FLOW_MESSAGE)
{
}

OpenFlowSwitchObjectManagerAddFlowMessage::~OpenFlowSwitchObjectManagerAddFlowMessage ()
{
}

void OpenFlowSwitchObjectManagerAddFlowMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32        (&m_openFlowIngressIfIndex, "openFlowIngressIfIndex"         ));
    addSerializableAttribute (new AttributeUI32        (&m_openFlowEgressIfIndex, "openFlowEgressIfIndex"           ));
    addSerializableAttribute (new AttributeUI32 (&m_openFlowSrcIpV4Address, "openFlowSrcIpV4Address"         ));
    addSerializableAttribute (new AttributeUI32 (&m_openFlowSrcIpV4AddressMask, "openFlowSrcIpV4AddressMask" ));
    addSerializableAttribute (new AttributeUI32 (&m_openFlowDstIpV4Address, "openFlowDstIpV4Address"         ));
    addSerializableAttribute (new AttributeUI32 (&m_openFlowDstIpV4AddressMask, "openFlowDstIpV4AddressMask" ));
    addSerializableAttribute (new AttributeString      (&m_openFlowAppType, "openFlowAppType"                       ));
    addSerializableAttribute (new AttributeUI16        (&m_openFlowSrcPort, "openFlowSrcPort"                       ));
    addSerializableAttribute (new AttributeUI16        (&m_openFlowDstPort, "openFlowDstPort"                       ));
}

// getters and setters

UI32 OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowIngressIfIndex () const
{
    return (m_openFlowIngressIfIndex);
}

void OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowIngressIfIndex (const UI32 &OpenFlowIngressIfIndex)
{
    m_openFlowIngressIfIndex = OpenFlowIngressIfIndex;
}

UI32 OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowEgressIfIndex () const
{
    return (m_openFlowEgressIfIndex);
}

void OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowEgressIfIndex (const UI32 &OpenFlowEgressIfIndex)
{
    m_openFlowEgressIfIndex = OpenFlowEgressIfIndex;
}

UI32 OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowSrcIpV4Address () const
{
    return (m_openFlowSrcIpV4Address);
}

void  OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowSrcIpV4Address (const UI32 &OpenFlowSrcIpV4Address)
{
    m_openFlowSrcIpV4Address = OpenFlowSrcIpV4Address;
}

UI32 OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowSrcIpV4AddressMask () const
{
    return (m_openFlowSrcIpV4AddressMask);
}

void  OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowSrcIpV4AddressMask (const UI32 &OpenFlowSrcIpV4AddressMask)
{
    m_openFlowSrcIpV4AddressMask = OpenFlowSrcIpV4AddressMask;
}

UI32 OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowDstIpV4Address () const
{
    return (m_openFlowDstIpV4Address);
}

void  OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowDstIpV4Address (const UI32 &OpenFlowDstIpV4Address)
{
    m_openFlowDstIpV4Address = OpenFlowDstIpV4Address;
}

UI32 OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowDstIpV4AddressMask () const
{
    return (m_openFlowDstIpV4AddressMask);
}

void  OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowDstIpV4AddressMask (const UI32 &OpenFlowDstIpV4AddressMask)
{
    m_openFlowDstIpV4AddressMask = OpenFlowDstIpV4AddressMask;
}

string  OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowAppType () const
{
    return (m_openFlowAppType);
}

void OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowAppType (const string &OpenFlowAppType)
{
    m_openFlowAppType = OpenFlowAppType;
}

UI16 OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowSrcPort () const
{
    return (m_openFlowSrcPort);
}

void OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowSrcPort (const UI16 &OpenFlowSrcPort)
{
    m_openFlowSrcPort = OpenFlowSrcPort;
}

UI16 OpenFlowSwitchObjectManagerAddFlowMessage::getOpenFlowDstPort () const
{
    return (m_openFlowDstPort);
}

void OpenFlowSwitchObjectManagerAddFlowMessage::setOpenFlowDstPort (const UI16 &OpenFlowDstPort)
{
    m_openFlowDstPort = OpenFlowDstPort;
}

}
