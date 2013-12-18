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
 *   Author : Sanjeev Joshi                                                *
 **************************************************************************/

#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Lldp/Local/LldpTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Lldp/Local/LldpLocalShowMessage.h"
#include "Utils/DcmMemUtil.h"
namespace DcmNs
{

    LldpLocalShowMessage::LldpLocalShowMessage ()
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),
                LLDPLOCALSHOW)
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALSHOWMESSAGE,sizeof(LldpLocalShowMessage),DCMMEMSTATINCREMENT);
	}

    LldpLocalShowMessage::LldpLocalShowMessage (const string &ifName, 
            const UI32 &cmdCode, const UI32 &lastIfIndex, const UI32 &numRecords, bool isGwRequest)
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),
                LLDPLOCALSHOW),
        m_ifName    (ifName),
        m_cmdCode    (cmdCode), 
        m_lastIfIndex (lastIfIndex),
        m_numRecords  (numRecords),
        m_isGwRequest (isGwRequest)
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALSHOWMESSAGE,sizeof(LldpLocalShowMessage),DCMMEMSTATINCREMENT);
    }

    LldpLocalShowMessage::~LldpLocalShowMessage ()
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALSHOWMESSAGE,sizeof(LldpLocalShowMessage),DCMMEMSTATDECREMENT);
    }

    void  LldpLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_lastIfIndex,"lastIfIndex"));
        addSerializableAttribute(new AttributeUI32(&m_numRecords,"numRecords"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, 
                                "isGwRequest"));
    }

    void  LldpLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  LldpLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  LldpLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LldpLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void LldpLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool LldpLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void LldpLocalShowMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void LldpLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage) 
    {
        this->copyBuffersFrom(prismMessage);
        
    }

    UI32 LldpLocalShowMessage::getLastIfIndex() const
    {
        return m_lastIfIndex;
    }

    UI32 LldpLocalShowMessage::getNumRecords() const
    {
        return m_numRecords;
    }

    void LldpLocalShowMessage::setLastIfIndex(const UI32 lastIfIndex)
    {
        m_lastIfIndex = lastIfIndex;
    }

    void LldpLocalShowMessage::setNumRecords(const UI32 numRecords)
    {
        m_numRecords = numRecords;
    }
}
