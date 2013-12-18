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

#include "Qos/Local/QosLocalNasShowMessage.h"
#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosLocalNasShowMessage::QosLocalNasShowMessage ()
        : PrismMessage (QosLocalObjectManager::getPrismServiceId(), QOSNASSHOW)
    {
    }

	QosLocalNasShowMessage::QosLocalNasShowMessage (const UI32 opCode, const UI32 cmdCode, const UI32 cos, const UI32 dscp,
                const IpV4AddressNetworkMask serverIpAddressPrefix, const string vrfName, const UI32 vlanNumber)
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSNASSHOW),
        QosNasBaseMessage(opCode, cmdCode, cos, dscp, serverIpAddressPrefix, vrfName, vlanNumber)
    {
    }


    QosLocalNasShowMessage::~QosLocalNasShowMessage ()
    {
    }

    void  QosLocalNasShowMessage::setupAttributesForSerialization()
    {
		PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_cos,"cos"));
        addSerializableAttribute (new AttributeUI32(&m_dscp,"dscp"));
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_serverIpAddressPrefix,"serverIpAddressPrefix"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
        addSerializableAttribute (new AttributeUI32(&m_vlanNumber,"vlanNumber"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
    }
    void QosLocalNasShowMessage::setIsGwRequest (bool &isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool QosLocalNasShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }
    void QosLocalNasShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void QosLocalNasShowMessage::copyAllBuffers (const PrismMessage &prismMessage) 
    {
        this->copyBuffersFrom(prismMessage);
    }


}
