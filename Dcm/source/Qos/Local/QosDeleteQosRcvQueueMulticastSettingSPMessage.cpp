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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosDeleteQosRcvQueueMulticastSettingSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosDeleteQosRcvQueueMulticastSettingSPMessage::QosDeleteQosRcvQueueMulticastSettingSPMessage ()
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(),QOSDELETEQOSRCVQUEUEMULTICASTSETTINGSP)
    {
    }

    QosDeleteQosRcvQueueMulticastSettingSPMessage::QosDeleteQosRcvQueueMulticastSettingSPMessage (const SI32 &opCode)
        : PrismMessage (QosLocalObjectManager::getPrismServiceId(),QOSDELETEQOSRCVQUEUEMULTICASTSETTINGSP),
        m_opCode    (opCode)
    {
    }

    QosDeleteQosRcvQueueMulticastSettingSPMessage::~QosDeleteQosRcvQueueMulticastSettingSPMessage ()
    {
    }

    void  QosDeleteQosRcvQueueMulticastSettingSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_opCode,"opCode"));
    }

    void  QosDeleteQosRcvQueueMulticastSettingSPMessage::setOpCode(const SI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    SI32  QosDeleteQosRcvQueueMulticastSettingSPMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
