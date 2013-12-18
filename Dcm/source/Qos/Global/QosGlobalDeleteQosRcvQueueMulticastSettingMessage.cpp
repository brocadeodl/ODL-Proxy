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

#include "Qos/Global/QosGlobalDeleteQosRcvQueueMulticastSettingMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalDeleteQosRcvQueueMulticastSettingMessage::QosGlobalDeleteQosRcvQueueMulticastSettingMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALDELETEQOSRCVQUEUEMULTICASTSETTING)
    {
    }

    QosGlobalDeleteQosRcvQueueMulticastSettingMessage::QosGlobalDeleteQosRcvQueueMulticastSettingMessage (const SI32 &opCode)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALDELETEQOSRCVQUEUEMULTICASTSETTING),
        m_opCode    (opCode)
    {
    }

    QosGlobalDeleteQosRcvQueueMulticastSettingMessage::~QosGlobalDeleteQosRcvQueueMulticastSettingMessage ()
    {
    }

    void  QosGlobalDeleteQosRcvQueueMulticastSettingMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_opCode,"opCode"));
    }

    void  QosGlobalDeleteQosRcvQueueMulticastSettingMessage::setOpCode(const SI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    SI32  QosGlobalDeleteQosRcvQueueMulticastSettingMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
