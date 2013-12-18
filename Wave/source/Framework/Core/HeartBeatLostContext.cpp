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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/HeartBeatLostContext.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

HeartBeatLostContext::HeartBeatLostContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps)
{
}

HeartBeatLostContext::~HeartBeatLostContext ()
{
}

void HeartBeatLostContext::addNodeIpAddressAndPort (const string &nodeIpAddress, const UI32 &nodePort)
{
    m_nodeIpAddresses.push_back (nodeIpAddress);
    m_nodePorts.push_back (nodePort);
}

UI32 HeartBeatLostContext::getNumberOfNodes () const
{
    UI32 numberOfNodeIpAddresses = m_nodeIpAddresses.size ();
    UI32 numberOfNodePorts       = m_nodePorts.size ();

    prismAssert (numberOfNodeIpAddresses == numberOfNodePorts, __FILE__, __LINE__);

    return (numberOfNodeIpAddresses);
}

string HeartBeatLostContext::getNodeIpAddressAtIndex (const UI32 &index) const
{
    prismAssert (index < (m_nodeIpAddresses.size ()), __FILE__, __LINE__);

    return (m_nodeIpAddresses[index]);
}

UI32 HeartBeatLostContext::getNodePortAtIndex (const UI32 &index) const
{
    prismAssert (index < (m_nodePorts.size ()), __FILE__, __LINE__);

    return (m_nodePorts[index]);
}

}
