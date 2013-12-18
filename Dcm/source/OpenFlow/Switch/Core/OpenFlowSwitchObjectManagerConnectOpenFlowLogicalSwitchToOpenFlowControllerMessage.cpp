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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUri.h"
#include "Framework/Attributes/AttributeResourceId.h"

using namespace WaveNs;

namespace OpenFlowNs
{

OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage ()
    : ManagementInterfaceMessage                           (OpenFlowSwitchObjectManager::getServiceName (), OPENFLOW_OBJECT_MANAGER_CONNECT_OPENFLOW_LOGICAL_SWITCH_TO_OPENFLOW_CONTROLLER),
      m_openFlowControllerPort                             (0),
      m_openFlowControllerChannelConnectionDefaultProtocol (OPEN_FLOW_CHANNEL_CONNECTION_PROTOCOL_TCP)
{
}

OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::~OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage ()
{
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString     (         &m_openFlowLogicalSwitchName,                          "openFlowLogicalSwitchName"));
    addSerializableAttribute (new AttributeUri        (         &m_openFlowLogicalSwitchUri,                           "openFlowLogicalSwitchUri"));
    addSerializableAttribute (new AttributeString     (         &m_openFlowControllerIpAddress,                        "m_openFlowControllerIpAddress"));
    addSerializableAttribute (new AttributeUI32       (         &m_openFlowControllerPort,                             "openFlowControllerPort"));
    addSerializableAttribute (new AttributeResourceId ((UI32 *) &m_openFlowControllerChannelConnectionDefaultProtocol, "openFlowControllerChannelConnectionDefaultProtocol"));
}

string OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::getOpenFlowLogicalSwitchName () const
{
    return (m_openFlowLogicalSwitchName);
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::setOpenFlowLogicalSwitchName (const string &openFlowLogicalSwitchName)
{
    m_openFlowLogicalSwitchName = openFlowLogicalSwitchName;
}

Uri OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::getOpenFlowLogicalSwitchUri () const
{
    return (m_openFlowLogicalSwitchUri);
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::setOpenFlowLogicalSwitchUri (const Uri &openFlowLogicalSwitchUri)
{
    m_openFlowLogicalSwitchUri = openFlowLogicalSwitchUri;
}

string OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::getOpenFlowControllerIpAddress () const
{
    return (m_openFlowControllerIpAddress);
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::setOpenFlowControllerIpAddress (const string &OpenFlowControllerIpAddress)
{
    m_openFlowControllerIpAddress = OpenFlowControllerIpAddress;
}

UI32 OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::getOpenFlowControllerPort () const
{
    return (m_openFlowControllerPort);
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::setOpenFlowControllerPort (const UI32 &openFlowControllerPort)
{
    m_openFlowControllerPort = openFlowControllerPort;
}

OpenFlowChannelConnectionProtocol OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::getOpenFlowcontrollerChannelConnectionDefaultProtocol () const
{
    return (m_openFlowControllerChannelConnectionDefaultProtocol);
}

void OpenFlowSwitchObjectManagerConnectOpenFlowLogicalSwitchToOpenFlowControllerMessage::setOpenFlowcontrollerChannelConnectionDefaultProtocol (const OpenFlowChannelConnectionProtocol &openFlowControllerChannelConnectionDefaultProtocol)
{
    m_openFlowControllerChannelConnectionDefaultProtocol = openFlowControllerChannelConnectionDefaultProtocol;
}

}

