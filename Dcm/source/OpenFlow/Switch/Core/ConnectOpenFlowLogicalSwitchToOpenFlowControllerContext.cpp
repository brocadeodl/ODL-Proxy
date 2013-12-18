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

#include "OpenFlow/Switch/Core/ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "OpenFlow/Switch/Core/OpenFlowControllerManagedObject.h"

namespace OpenFlowNs
{

ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext                          (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_openFlowControllerPort                             (0),
      m_openFlowControllerChannelConnectionDefaultProtocol (OPEN_FLOW_CHANNEL_CONNECTION_PROTOCOL_TCP),
      m_pOpenFlowControllerManagedObject                   (NULL),
      m_pOpenFlowLogicalSwitchManagedObject                (NULL)
{
}

ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext           (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_openFlowControllerPort                             (0),
      m_openFlowControllerChannelConnectionDefaultProtocol (OPEN_FLOW_CHANNEL_CONNECTION_PROTOCOL_TCP),
      m_pOpenFlowControllerManagedObject                   (NULL),
      m_pOpenFlowLogicalSwitchManagedObject                (NULL)
{
}

ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::~ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext ()
{
}

string ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::getOpenFlowLogicalSwitchName () const
{
    return (m_openFlowLogicalSwitchName);
}

void ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::setOpenFlowLogicalSwitchName (const string &openFlowLogicalSwitchName)
{
    m_openFlowLogicalSwitchName = openFlowLogicalSwitchName;
}

Uri ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::getOpenFlowLogicalSwitchUri () const
{
    return (m_openFlowLogicalSwitchUri);
}

void ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::setOpenFlowLogicalSwitchUri (const Uri &openFlowLogicalSwitchUri)
{
    m_openFlowLogicalSwitchUri = openFlowLogicalSwitchUri;
}

string ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::getOpenFlowcontrollerIpAddress () const
{
    return (m_openFlowControllerIpAddress);
}

void ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::setOpenFlowControllerIpAddress (const string &OpenFlowControllerIpAddress)
{
    m_openFlowControllerIpAddress = OpenFlowControllerIpAddress;
}

UI32 ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::getOpenFlowControllerPort () const
{
    return (m_openFlowControllerPort);
}

void ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::setOpenFlowControllerPort (const UI32 &openFlowControllerPort)
{
    m_openFlowControllerPort = openFlowControllerPort;
}

OpenFlowControllerManagedObject *ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::getPOpenFlowControllerManagedObejct ()
{
    return (m_pOpenFlowControllerManagedObject);
}

void ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::setPOpenFlowControllerManagedObject (OpenFlowControllerManagedObject *pOpenFlowControllerManagedObject)
{
    m_pOpenFlowControllerManagedObject = pOpenFlowControllerManagedObject;
}

OpenFlowLogicalSwitchManagedObject *ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::getPOpenFlowLogicalSwitchManagedObject ()
{
    return (m_pOpenFlowLogicalSwitchManagedObject);
}

void ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::setPOpenFlowLogicalSwitchManagedObject (OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject)
{
    m_pOpenFlowLogicalSwitchManagedObject = pOpenFlowLogicalSwitchManagedObject;
}

OpenFlowChannelConnectionProtocol ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::getOpenFlowcontrollerChannelConnectionDefaultProtocol () const
{
    return (m_openFlowControllerChannelConnectionDefaultProtocol);
}

void ConnectOpenFlowLogicalSwitchToOpenFlowControllerContext::setOpenFlowcontrollerChannelConnectionDefaultProtocol (const OpenFlowChannelConnectionProtocol &openFlowControllerChannelConnectionDefaultProtocol)
{
    m_openFlowControllerChannelConnectionDefaultProtocol = openFlowControllerChannelConnectionDefaultProtocol;
}

}
