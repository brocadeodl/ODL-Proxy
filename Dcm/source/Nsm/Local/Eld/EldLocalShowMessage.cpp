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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Amitayu Das                                              	   *
 **************************************************************************/

#include "Nsm/Local/Eld/EldLocalShowMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/Eld/EldLocalShowMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    EldLocalShowMessage::EldLocalShowMessage()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), ELDLOCALSHOW)
    {
		trace (TRACE_LEVEL_INFO, "CONSTRUCTOR::1");
		m_cmdCode = 0;
		m_isGwRequest = true;
		m_poNum = 0;
		m_vlanId = 0;
		m_rbridgeId = 0;
		m_mappedId = 0;
		m_is_distribution_required = false;
    }

    EldLocalShowMessage::EldLocalShowMessage(const string &ifName, const UI32 &poNum, const UI32 &cmdCode, bool isGwRequest)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), ELDLOCALSHOW),

        m_ifName       (ifName),
        m_poNum         (poNum),
        m_cmdCode       (cmdCode),
        m_isGwRequest (isGwRequest)
    {
    }
    EldLocalShowMessage::EldLocalShowMessage(EldLocalShowMessage *pMsg)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), ELDLOCALSHOW)
    {
      m_ifName = pMsg->getIfName();
      m_poNum = pMsg->getPoNum();
      m_cmdCode = pMsg->getCmdCode();
      m_isGwRequest = pMsg->getIsGwRequest ();
      m_vlanId = pMsg->getVlanId();
      m_rbridgeId = pMsg->getRbridgeId();
      m_mappedId = pMsg->getMappedId();
      m_is_distribution_required = getisDistributionRequired();

    }

    EldLocalShowMessage::~EldLocalShowMessage ()
    {
    }

    void  EldLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_poNum,"poNum"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
        addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedid"));
        addSerializableAttribute (new AttributeUI32(&m_rbridgeId,"rbridge"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlan_id"));
        addSerializableAttribute (new AttributeBool(&m_is_distribution_required, "is_distribution_required"));  
    }

    void  EldLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  EldLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void EldLocalShowMessage::setPoNum (const UI32 &poNum)
    {
        m_poNum = poNum;
    }

    UI32 EldLocalShowMessage::getPoNum () const
    {
        return (m_poNum);
    }


    void  EldLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  EldLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    
    void EldLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool EldLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }


    void EldLocalShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void EldLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {   
        this->copyBuffersFrom(prismMessage);
    }
    #if 0  
    void EldLocalShowMessage::transferOutputBuffer(EldLocalShowMessage *&pMsg)
    {
		UI32 size = 0;
		UI32 bufferNum = pMsg->getCmdCode();
		void *pBuffer = NULL;

		trace(TRACE_LEVEL_DEVEL, string("EldLocalShowMessage::transferOutputBuffer bufNum is ") + bufferNum);
		pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
		if (pBuffer == NULL) {
			trace(TRACE_LEVEL_DEVEL, "EldLocalShowMessage::transferOutputBuffer received empty buffer");
			return;
		}
		addBuffer(bufferNum, size, pBuffer);
    }
#endif

   UI32  EldLocalShowMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  EldLocalShowMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  EldLocalShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void  EldLocalShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    void  EldLocalShowMessage::setRbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    UI32  EldLocalShowMessage::getRbridgeId() const
    {
        return (m_rbridgeId);
    }


}
