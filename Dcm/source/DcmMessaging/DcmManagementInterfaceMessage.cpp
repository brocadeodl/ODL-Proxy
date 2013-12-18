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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

namespace DcmNs
{

DcmManagementInterfaceMessage::DcmManagementInterfaceMessage (const UI32 &operationId)
    : ManagementInterfaceMessage (WaveUserInterfaceObjectManager::getPrismServiceName (), operationId),
    m_clientStatus (-1)
{
}

DcmManagementInterfaceMessage::~DcmManagementInterfaceMessage ()
{
}

void DcmManagementInterfaceMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeSI32 (&m_clientStatus, "clientStatus"));
}

SI32 DcmManagementInterfaceMessage::getClientStatus () const
{
    return (m_clientStatus);
}

void DcmManagementInterfaceMessage::setClientStatus (const SI32 &clientStatus)
{
    m_clientStatus = clientStatus;
}

void DcmManagementInterfaceMessage::updateForCompletionStatusDuringSurrogacy ()
{
    PrismMessage::updateForCompletionStatusDuringSurrogacy ();
    m_clientStatus = WAVE_MESSAGE_SUCCESS;
}
}
