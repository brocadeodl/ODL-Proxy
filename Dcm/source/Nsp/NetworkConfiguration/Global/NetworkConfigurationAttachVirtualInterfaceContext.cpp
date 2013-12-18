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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationAttachVirtualInterfaceContext.h"
#include "Nsp/NetworkConfiguration/Global/VnicManagedObject.h"

namespace NspNs
{

NetworkConfigurationAttachVirtualInterfaceContext::NetworkConfigurationAttachVirtualInterfaceContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pVnicManagedObject        (NULL)
{
}

NetworkConfigurationAttachVirtualInterfaceContext::~NetworkConfigurationAttachVirtualInterfaceContext ()
{
}

VnicManagedObject *NetworkConfigurationAttachVirtualInterfaceContext::getPVnicManagedObject ()
{
    return (m_pVnicManagedObject);
}

void NetworkConfigurationAttachVirtualInterfaceContext::setPVnicManagedObject (VnicManagedObject *pVnicManagedObject)
{
    m_pVnicManagedObject = pVnicManagedObject;
}

string NetworkConfigurationAttachVirtualInterfaceContext::getNetworkName () const
{
    return (m_networkName);
}

void NetworkConfigurationAttachVirtualInterfaceContext::setNetworkName (const string &networkName)
{
    m_networkName = networkName;
}

UI32 NetworkConfigurationAttachVirtualInterfaceContext::getNetworkId () const
{
    return (m_networkId);
}

void NetworkConfigurationAttachVirtualInterfaceContext::setNetworkId (const UI32 &networkId)
{
    m_networkId = networkId;
}

}
