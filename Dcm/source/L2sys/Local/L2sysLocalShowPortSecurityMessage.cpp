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
 *   Author : skrastog                                                     *
 **************************************************************************/

#include "L2sys/Local/L2sysLocalObjectManager.h"
#include "L2sys/Local/L2sysTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include  "L2sys/Local/L2sysLocalShowPortSecurityMessage.h"
#include  "ClientInterface/L2sys/L2sysMessageDef.h"
#include  "Framework/Utils/TraceUtils.h"
namespace DcmNs
{

    L2sysLocalShowPortSecurityMessage::L2sysLocalShowPortSecurityMessage ()
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSSHOWPORTSECURITY),
        m_cmdCode    (0),
        m_rbridgeId    (0),
        m_ifType    (0),
        m_isGwRequest(0),
        m_lastRbridgeId(0),
        m_lastIfName(""),
        m_lastMac(""),
        m_lastType(""),
        m_lastVlanId(0),
        m_isLast(0),
        m_isFirstRequest(0)
    {
    }

    L2sysLocalShowPortSecurityMessage::L2sysLocalShowPortSecurityMessage (const UI32 &cmdCode, const string &ifName, const UI32 &ifType)
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSSHOWPORTSECURITY),
        m_cmdCode    (cmdCode),
        m_ifName    (ifName),
        m_ifType    (ifType)
    {
        m_lastRbridgeId = 0;
        m_lastIfName = "";
        m_lastMac = "";
        m_lastType = "";
        m_lastVlanId = 0;
        m_isLast = 0;
        m_isFirstRequest = 0;
		m_rbridgeId = 0;
		m_isGwRequest = 0;
    }

    L2sysLocalShowPortSecurityMessage::~L2sysLocalShowPortSecurityMessage ()
    {
    }

    void  L2sysLocalShowPortSecurityMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_rbridgeId,"rbridgeId"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));

        // Last Mac Record information
        addSerializableAttribute (new AttributeUI32(&m_lastRbridgeId, "lastRbridgeId"));
        addSerializableAttribute (new AttributeString(&m_lastIfName,"lastIfName"));
        addSerializableAttribute (new AttributeString(&m_lastMac,"lastMac"));
        addSerializableAttribute (new AttributeString(&m_lastType,"lastType"));
        addSerializableAttribute (new AttributeUI32(&m_lastVlanId, "lastVlanId"));
        addSerializableAttribute (new AttributeUI32(&m_isLast, "isLast"));
        addSerializableAttribute (new AttributeUI32(&m_isFirstRequest, "isFirstRequest"));
    }

    void  L2sysLocalShowPortSecurityMessage::setRbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    UI32  L2sysLocalShowPortSecurityMessage::getRbridgeId() const
    {
        return (m_rbridgeId);
    }

    void  L2sysLocalShowPortSecurityMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  L2sysLocalShowPortSecurityMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  L2sysLocalShowPortSecurityMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  L2sysLocalShowPortSecurityMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  L2sysLocalShowPortSecurityMessage::setCmdCode(const UI32 &cmdCode)
    {

        m_cmdCode  =  cmdCode;
    }

    UI32  L2sysLocalShowPortSecurityMessage::getCmdCode() const
    {
        return (m_cmdCode);
   	}

	void L2sysLocalShowPortSecurityMessage::setIsGwRequest(bool isGwRequest)
    {
		m_isGwRequest = isGwRequest;
    }

	bool L2sysLocalShowPortSecurityMessage::getIsGwRequest() const
	{
		return (m_isGwRequest);
	}

	void  L2sysLocalShowPortSecurityMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
    	pBuffer = findBuffer(bufferNum, size);
    }

    void L2sysLocalShowPortSecurityMessage::transferOutputBuffer(L2sysPortSecShowMessage *&pMsg)
    {
    	UI32 size = 0;
        UI32 bufferNum = pMsg->getCmdCode();
        void *pBuffer = NULL;

        pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
        if (pBuffer == NULL) {
        	trace(TRACE_LEVEL_INFO, "L2sysLocalShowPortSecurityMessage::transferOutputBuffer received empty buffer");
            return;
        }
        addBuffer(bufferNum, size, pBuffer);
    }

	void L2sysLocalShowPortSecurityMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
	}

    void L2sysLocalShowPortSecurityMessage::setLastPortSecurityRecord(const l2sys_show_mac_output_msg_t *lastMacRecord)
    {
        m_lastRbridgeId = lastMacRecord->rbridge_id;
        m_lastIfName = lastMacRecord->if_name;
        m_lastMac = lastMacRecord->Mac;
        m_lastType = lastMacRecord->type;
        m_lastVlanId = lastMacRecord->vlan_id;
        m_isLast = lastMacRecord->is_last;
    }

    void L2sysLocalShowPortSecurityMessage::getLastPortSecurityRecord(l2sys_show_mac_output_msg_t *lastMacRecord)
    {
        lastMacRecord->rbridge_id = m_lastRbridgeId;
        strncpy(lastMacRecord->if_name, m_lastIfName.c_str(), 256);
        strncpy(lastMacRecord->Mac, m_lastMac.c_str(), 16);
        strncpy(lastMacRecord->type, m_lastType.c_str(), 64);
        lastMacRecord->vlan_id = m_lastVlanId;
        lastMacRecord->is_last = m_isLast;
    }

    void L2sysLocalShowPortSecurityMessage::setIsFirstRequest(const UI32 &isFirstRequest)
    {
        m_isFirstRequest = isFirstRequest;
    }

    UI32 L2sysLocalShowPortSecurityMessage::getIsFirstRequest() const
    {
        return (m_isFirstRequest);
    }

}
