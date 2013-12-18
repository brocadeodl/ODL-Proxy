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

#include "OpenFlow/Switch/Core/AddOpenFlowControllerToOpenFlowLogicalSwitchContext.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "OpenFlow/Switch/Core/OpenFlowControllerManagedObject.h"

namespace OpenFlowNs
{

AddOpenFlowControllerToOpenFlowLogicalSwitchContext::AddOpenFlowControllerToOpenFlowLogicalSwitchContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext                          (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_openFlowControllerPort                             (0),
      m_openFlowControllerChannelConnectionDefaultProtocol (OPEN_FLOW_CHANNEL_CONNECTION_PROTOCOL_TCP),
      m_pOpenFlowControllerManagedObject                   (NULL),
      m_pOpenFlowLogicalSwitchManagedObject                (NULL)
{
}

AddOpenFlowControllerToOpenFlowLogicalSwitchContext::AddOpenFlowControllerToOpenFlowLogicalSwitchContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext           (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_openFlowControllerPort                             (0),
      m_openFlowControllerChannelConnectionDefaultProtocol (OPEN_FLOW_CHANNEL_CONNECTION_PROTOCOL_TCP),
      m_pOpenFlowControllerManagedObject                   (NULL),
      m_pOpenFlowLogicalSwitchManagedObject                (NULL)
{
}

AddOpenFlowControllerToOpenFlowLogicalSwitchContext::~AddOpenFlowControllerToOpenFlowLogicalSwitchContext ()
{
}

string AddOpenFlowControllerToOpenFlowLogicalSwitchContext::getOpenFlowLogicalSwitchName () const
{
    return (m_openFlowLogicalSwitchName);
}

void AddOpenFlowControllerToOpenFlowLogicalSwitchContext::setOpenFlowLogicalSwitchName (const string &openFlowLogicalSwitchName)
{
    m_openFlowLogicalSwitchName = openFlowLogicalSwitchName;
}

Uri AddOpenFlowControllerToOpenFlowLogicalSwitchContext::getOpenFlowLogicalSwitchUri () const
{
    return (m_openFlowLogicalSwitchUri);
}

void AddOpenFlowControllerToOpenFlowLogicalSwitchContext::setOpenFlowLogicalSwitchUri (const Uri &openFlowLogicalSwitchUri)
{
    m_openFlowLogicalSwitchUri = openFlowLogicalSwitchUri;
}

string AddOpenFlowControllerToOpenFlowLogicalSwitchContext::getOpenFlowcontrollerIpAddress () const
{
    return (m_openFlowControllerIpAddress);
}

void AddOpenFlowControllerToOpenFlowLogicalSwitchContext::setOpenFlowControllerIpAddress (const string &OpenFlowControllerIpAddress)
{
    m_openFlowControllerIpAddress = OpenFlowControllerIpAddress;
}

UI32 AddOpenFlowControllerToOpenFlowLogicalSwitchContext::getOpenFlowControllerPort () const
{
    return (m_openFlowControllerPort);
}

void AddOpenFlowControllerToOpenFlowLogicalSwitchContext::setOpenFlowControllerPort (const UI32 &openFlowControllerPort)
{
    m_openFlowControllerPort = openFlowControllerPort;
}

OpenFlowControllerManagedObject *AddOpenFlowControllerToOpenFlowLogicalSwitchContext::getPOpenFlowControllerManagedObejct ()
{
    return (m_pOpenFlowControllerManagedObject);
}

void AddOpenFlowControllerToOpenFlowLogicalSwitchContext::setPOpenFlowControllerManagedObject (OpenFlowControllerManagedObject *pOpenFlowControllerManagedObject)
{
    m_pOpenFlowControllerManagedObject = pOpenFlowControllerManagedObject;
}

OpenFlowLogicalSwitchManagedObject *AddOpenFlowControllerToOpenFlowLogicalSwitchContext::getPOpenFlowLogicalSwitchManagedObject ()
{
    return (m_pOpenFlowLogicalSwitchManagedObject);
}

void AddOpenFlowControllerToOpenFlowLogicalSwitchContext::setPOpenFlowLogicalSwitchManagedObject (OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject)
{
    m_pOpenFlowLogicalSwitchManagedObject = pOpenFlowLogicalSwitchManagedObject;
}

OpenFlowChannelConnectionProtocol AddOpenFlowControllerToOpenFlowLogicalSwitchContext::getOpenFlowcontrollerChannelConnectionDefaultProtocol () const
{
    return (m_openFlowControllerChannelConnectionDefaultProtocol);
}

void AddOpenFlowControllerToOpenFlowLogicalSwitchContext::setOpenFlowcontrollerChannelConnectionDefaultProtocol (const OpenFlowChannelConnectionProtocol &openFlowControllerChannelConnectionDefaultProtocol)
{
    m_openFlowControllerChannelConnectionDefaultProtocol = openFlowControllerChannelConnectionDefaultProtocol;
}

}
