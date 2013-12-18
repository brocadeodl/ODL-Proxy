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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Lldp/Local/LldpTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Lldp/Local/LldpLocalClearMessage.h"
#include "Utils/DcmMemUtil.h"
namespace DcmNs
{

    LldpLocalClearMessage::LldpLocalClearMessage ()
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),
                LLDPLOCALCLEAR)
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALCLEARMESSAGE,sizeof(LldpLocalClearMessage),DCMMEMSTATINCREMENT);
		m_interface = "tengigabitethernet";
	}

    LldpLocalClearMessage::LldpLocalClearMessage (const string &ifName, 
            const UI32 &cmdCode, bool isGwRequest)
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),
                LLDPLOCALCLEAR),
        m_ifName    (ifName),
        m_cmdCode    (cmdCode), 
        m_isGwRequest (isGwRequest),
		m_interface ("tengigabitethernet")
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALCLEARMESSAGE,sizeof(LldpLocalClearMessage),DCMMEMSTATINCREMENT);
    }

    LldpLocalClearMessage::~LldpLocalClearMessage ()
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALCLEARMESSAGE,sizeof(LldpLocalClearMessage),DCMMEMSTATDECREMENT);
    }

    void  LldpLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, 
                                "isGwRequest"));
        addSerializableAttribute(new AttributeString(&m_interface,"interface"));
    }

    void  LldpLocalClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  LldpLocalClearMessage::getIfName() const
    {
        return (m_ifName);
    }
    void  LldpLocalClearMessage::setInterface(const string &interface)
    {
        m_interface  =  interface;
    }

    string  LldpLocalClearMessage::getInterface() const
    {
        return (m_interface);
    }

    void  LldpLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LldpLocalClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void LldpLocalClearMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool LldpLocalClearMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void LldpLocalClearMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void LldpLocalClearMessage::copyAllBuffers (const PrismMessage &prismMessage) 
    {
        this->copyBuffersFrom(prismMessage);
        
    }

}
