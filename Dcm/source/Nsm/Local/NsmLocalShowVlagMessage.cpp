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
 *   Author : Amulya Makam                                                 *
 **************************************************************************/

#include "Nsm/Local/NsmLocalShowVlagMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    NsmLocalShowVlagMessage::NsmLocalShowVlagMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALSHOWVLAG)
        {
            m_cmdCode = 0;
            m_isGwRequest = true;
        }

    NsmLocalShowVlagMessage::NsmLocalShowVlagMessage (const UI32 &cmdCode,const UI32 &poId,const bool &isGwRequest) : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALSHOWVLAG),
        m_cmdCode   (cmdCode),
        m_poId (poId),
        m_isGwRequest(isGwRequest)
        {
        }

    NsmLocalShowVlagMessage::~NsmLocalShowVlagMessage ()
    {
    }

    void  NsmLocalShowVlagMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_poId,"poId"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
    }

    void  NsmLocalShowVlagMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }
    UI32  NsmLocalShowVlagMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmLocalShowVlagMessage::setPoId(const UI32 &poId)
    {
        m_poId  =  poId;
    }

    UI32  NsmLocalShowVlagMessage::getPoId() const
    {
        return (m_poId);
    }

    void NsmLocalShowVlagMessage::setIsGwRequest(const bool &isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool NsmLocalShowVlagMessage::getIsGwRequest() const
    {
        return (m_isGwRequest);
    }

    void NsmLocalShowVlagMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void NsmLocalShowVlagMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }
}

