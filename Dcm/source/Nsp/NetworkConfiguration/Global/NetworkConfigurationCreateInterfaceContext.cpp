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

#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateInterfaceContext.h"
#include "Nsp/NetworkConfiguration/Global/NetworkSegmentManagedObject.h"

namespace NspNs
{

NetworkConfigurationCreateInterfaceContext::NetworkConfigurationCreateInterfaceContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pNetworkSegmentManagedObject (NULL)
{
}

NetworkConfigurationCreateInterfaceContext::~NetworkConfigurationCreateInterfaceContext ()
{
}

ObjectId NetworkConfigurationCreateInterfaceContext::getNetworkObjectId () const
{
    return (m_networkObjectId);
}

void NetworkConfigurationCreateInterfaceContext::setNetworkObjectId (const ObjectId &networkObjectId)
{
    m_networkObjectId = networkObjectId;
}

ObjectId NetworkConfigurationCreateInterfaceContext::getNetworkSegmentObjectId () const
{
    return (m_networkSegmentObjectId);
}

void NetworkConfigurationCreateInterfaceContext::setNetworkSegmentObjectId (const ObjectId &networkSegmentObjectId)
{
    m_networkSegmentObjectId = networkSegmentObjectId;
}

NetworkSegmentManagedObject *NetworkConfigurationCreateInterfaceContext::getPNetworkSegmentManagedObject ()
{
    return (m_pNetworkSegmentManagedObject);
}

void NetworkConfigurationCreateInterfaceContext::setPNetworkSegmentManagedObject (NetworkSegmentManagedObject *pNetworkSegmentManagedObject)
{
    m_pNetworkSegmentManagedObject = pNetworkSegmentManagedObject;
}

}
