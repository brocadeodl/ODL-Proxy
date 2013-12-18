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

/**************************************************************************
*   Copyright (C) 2005-2014 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : Himanshu Varshney                                            *
***************************************************************************/

#include "Cluster/Local/ClusterLocalSetControllerDetailsMessage.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace WaveNs
{

ClusterLocalSetControllerDetailsMessage::ClusterLocalSetControllerDetailsMessage ()
    : ManagementInterfaceMessage (ClusterLocalObjectManager::getPrismServiceName (), CLUSTER_LOCAL_SET_CONTROLLER_DETAILS_MESSAGE)
{
    m_controllerPort = 0;
}

ClusterLocalSetControllerDetailsMessage::~ClusterLocalSetControllerDetailsMessage ()
{
}

void ClusterLocalSetControllerDetailsMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeIpVxAddress (&m_controllerIpAddress, "controllerIpAddress"));
    addSerializableAttribute (new AttributeSI32        (&m_controllerPort,      "controllerPort"));
}

void ClusterLocalSetControllerDetailsMessage::setControllerIpAddress (const IpVxAddress &ipAddress)
{
    m_controllerIpAddress = ipAddress;
}

IpVxAddress ClusterLocalSetControllerDetailsMessage::getControllerIpAddress ()
{
    return (m_controllerIpAddress);
}

SI32 ClusterLocalSetControllerDetailsMessage::getControllerPort ()
{
    return (m_controllerPort);
}

void ClusterLocalSetControllerDetailsMessage::setControllerPort (const SI32 &port)
{
    m_controllerPort = port;
}

}


