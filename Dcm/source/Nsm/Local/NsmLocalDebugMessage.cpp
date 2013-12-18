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
 *   .Author : bvreddy	                                                   *
 **************************************************************************/

#include "Nsm/Local/NsmLocalDebugMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    NsmLocalDebugMessage::NsmLocalDebugMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALDEBUGCONFIG)
    {
		m_DebugType = 0;
		m_cmdCode = 0;
		m_isGwRequest = true;
    }

    NsmLocalDebugMessage::NsmLocalDebugMessage (const UI32 &DebugType, const UI32 &cmdCode,const bool isGwRequest)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALDEBUGCONFIG),
        m_DebugType   (DebugType),
        m_cmdCode   (cmdCode),
		m_isGwRequest  (isGwRequest)
    {
    }

    NsmLocalDebugMessage::~NsmLocalDebugMessage ()
    {
    }

    void  NsmLocalDebugMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_DebugType,"DebugType"));
		addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
    }


    void  NsmLocalDebugMessage::setDebugType(const UI32 &DebugType)
    {
        m_DebugType  =  DebugType;
    }

    UI32  NsmLocalDebugMessage::getDebugType() const
    {
        return (m_DebugType);
    }

	
    void  NsmLocalDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void NsmLocalDebugMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool NsmLocalDebugMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void NsmLocalDebugMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void NsmLocalDebugMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

}
