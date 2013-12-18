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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Nsm/Local/LacpLocalClearMessage.h"
#include "Utils/DcmMemUtil.h"
namespace DcmNs
{

    LacpLocalClearMessage::LacpLocalClearMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),
                LACPLOCALCLEAR)
    {
	}

    LacpLocalClearMessage::LacpLocalClearMessage (const UI32 &portchannelNumber, 
            const UI32 &cmdCode, bool isGwRequest)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),
                LACPLOCALCLEAR),
        m_portchannelNumber    (portchannelNumber),
        m_cmdCode    (cmdCode), 
        m_isGwRequest (isGwRequest)
    {
    }

    LacpLocalClearMessage::~LacpLocalClearMessage ()
    {
    }

    void  LacpLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeUI32(&m_portchannelNumber,"portchannelNumber"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, 
                                "isGwRequest"));
    }

    void  LacpLocalClearMessage::setPortchannelNumber(const UI32 &portchannelNumber)
    {
        m_portchannelNumber  =  portchannelNumber;
    }

    UI32  LacpLocalClearMessage::getPortchannelNumber() const
    {
        return (m_portchannelNumber);
    }

    void  LacpLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LacpLocalClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void LacpLocalClearMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool LacpLocalClearMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void LacpLocalClearMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void LacpLocalClearMessage::copyAllBuffers (const PrismMessage &prismMessage) 
    {
        this->copyBuffersFrom(prismMessage);
        
    }

}
