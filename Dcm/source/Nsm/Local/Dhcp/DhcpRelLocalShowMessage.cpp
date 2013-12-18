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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/
#include "ClientInterface/DceWaveClientMessage.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalShowMessage.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalObjectManager.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    DhcpRelLocalShowMessage::DhcpRelLocalShowMessage()
        : PrismMessage (DhcpRelLocalObjectManager::getPrismServiceId (), IPHELPSHOW),
        m_cmdCode       (0)
    {
        
    }

    DhcpRelLocalShowMessage::DhcpRelLocalShowMessage(const UI32 &cmdCode, const string &ifName, const UI32 &ifType, const UI32 &vlanValue)
        : PrismMessage (DhcpRelLocalObjectManager::getPrismServiceId (), IPHELPSHOW),
        m_cmdCode       (cmdCode),
        m_ifName       (ifName),
        m_ifType       (ifType),
        m_vlanValue    (vlanValue)
    {
    }

    DhcpRelLocalShowMessage::~DhcpRelLocalShowMessage ()
    {
    }

    void  DhcpRelLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeUI32(&m_vlanValue,"vlanValue"));
//        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
//        addSerializableAttribute(new AttributeBool(&m_isMgmtCluster, "isMgmtCluster"));
    }

    void  DhcpRelLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  DhcpRelLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  DhcpRelLocalShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32 DhcpRelLocalShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  DhcpRelLocalShowMessage::setVlanValue(const UI32 &vlanValue)
    {
        m_vlanValue  =  vlanValue;
    }

    UI32 DhcpRelLocalShowMessage::getVlanValue() const
    {
        return (m_vlanValue);
    }

    void  DhcpRelLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  DhcpRelLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }


    void DhcpRelLocalShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void DhcpRelLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {   
        this->copyBuffersFrom(prismMessage);
    }
      
    void DhcpRelLocalShowMessage::transferOutputBuffer(DhcpRelLocalShowMessage *&pMsg)
    {
		UI32 size = 0;
		UI32 bufferNum = pMsg->getCmdCode();
		void *pBuffer = NULL;

		trace(TRACE_LEVEL_DEVEL, string("DhcpRelLocalShowMessage::transferOutputBuffer bufNum is ") + bufferNum);
		pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
		if (pBuffer == NULL) {
			trace(TRACE_LEVEL_DEVEL, "DhcpRelLocalShowMessage::transferOutputBuffer received empty buffer");
			return;
		}
		addBuffer(bufferNum, size, pBuffer);
    }
}
