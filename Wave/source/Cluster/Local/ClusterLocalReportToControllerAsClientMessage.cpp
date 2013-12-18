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

#include "Cluster/Local/ClusterLocalReportToControllerAsClientMessage.h"
#include "Cluster/Local/ClusterLocalObjectManager.h"
#include "Cluster/Local/ClusterLocalTypes.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

namespace WaveNs
{   

ClusterLocalReportToControllerAsClientMessage::ClusterLocalReportToControllerAsClientMessage ()
    : ManagementInterfaceMessage (ClusterLocalObjectManager::getPrismServiceName (), CLUSTER_LOCAL_REPORT_AS_CLIENT_MESSAGE)
{
    m_controllerPort = 0;
    m_controllerClientPort     = 0;
}
    
ClusterLocalReportToControllerAsClientMessage::~ClusterLocalReportToControllerAsClientMessage ()
{
}

void ClusterLocalReportToControllerAsClientMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeIpVxAddress (&m_controllerIpAddress, "controllerIpAddress"));
    addSerializableAttribute (new AttributeIpVxAddress (&m_controllerClientIpAddress,     "controllerClientIpAddress"));
    addSerializableAttribute (new AttributeSI32        (&m_controllerPort,      "controllerPort"));
    addSerializableAttribute (new AttributeSI32        (&m_controllerClientPort,          "controllerClientPort"));
}

void ClusterLocalReportToControllerAsClientMessage::setControllerIpAddress (const IpVxAddress &ipAddress)
{
    m_controllerIpAddress = ipAddress;
}

IpVxAddress ClusterLocalReportToControllerAsClientMessage::getControllerIpAddress ()
{
    return (m_controllerIpAddress);
}

void ClusterLocalReportToControllerAsClientMessage::setControllerClientIpAddress (const IpVxAddress &ipAddress)
{
    m_controllerClientIpAddress = ipAddress;
}

IpVxAddress ClusterLocalReportToControllerAsClientMessage::getControllerClientIpAddress ()
{
    return (m_controllerClientIpAddress);
}

SI32 ClusterLocalReportToControllerAsClientMessage::getControllerPort ()
{
    return (m_controllerPort);
}

void ClusterLocalReportToControllerAsClientMessage::setControllerPort (const SI32 &port)
{
    m_controllerPort = port;
}


SI32 ClusterLocalReportToControllerAsClientMessage::getControllerClientPort ()
{
    return (m_controllerClientPort);
}

void ClusterLocalReportToControllerAsClientMessage::setControllerClientPort (const SI32 &port)
{
    m_controllerClientPort = port;
}

}
 
