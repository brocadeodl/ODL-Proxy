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
 *   Author : Chaitanya Gangwar                                            *
 ***************************************************************************/

#include "ManagementInterface/ClientInterface/ManagementInterfaceClientListMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "ManagementInterface/ManagementInterfaceTypes.h"
#include "ManagementInterface/ManagementInterfaceObjectManager.h"

namespace WaveNs
{

ManagementInterfaceClientListMessage::ManagementInterfaceClientListMessage ()
    : ManagementInterfaceMessage (ManagementInterfaceObjectManager::getServiceName (), MANAGEMENTINTERFACE_CLIENT_LIST)
{
}

ManagementInterfaceClientListMessage::~ManagementInterfaceClientListMessage ()
{
}

void ManagementInterfaceClientListMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeStringVector (&m_connectedClients, "connectedClients"));
}

void ManagementInterfaceClientListMessage::setConnectedClients (vector<string> connectedClients)
{
    m_connectedClients = connectedClients;
}

vector<string> ManagementInterfaceClientListMessage::getConnectedClients ()
{
    return (m_connectedClients);
}

}
