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
 *   Author : Sai Srikanth
 **************************************************************************/
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Dot1XLocalTestMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    Dot1XLocalTestMessage::Dot1XLocalTestMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), DOT1X_LOCAL_TEST_ALL)
    {
    }

    Dot1XLocalTestMessage::Dot1XLocalTestMessage (const string &Interface, const string &ifName, const UI32 &cmdCode, bool isGwRequest)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), DOT1X_LOCAL_TEST_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
        m_cmdCode   (cmdCode),
        m_isGwRequest (isGwRequest)
    {
    }

    Dot1XLocalTestMessage::~Dot1XLocalTestMessage ()
    {
    }

    void  Dot1XLocalTestMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
    }

    void  Dot1XLocalTestMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  Dot1XLocalTestMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  Dot1XLocalTestMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  Dot1XLocalTestMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  Dot1XLocalTestMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  Dot1XLocalTestMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void Dot1XLocalTestMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool Dot1XLocalTestMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void Dot1XLocalTestMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void Dot1XLocalTestMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

    void Dot1XLocalTestMessage::transferOutputBuffer(Dot1XLocalTestMessage *&pMsg)
    {
        UI32 size = 0;
        UI32 bufferNum = pMsg->getCmdCode();
        void *pBuffer = NULL;

        trace(TRACE_LEVEL_DEVEL, string("Dot1XLocalTestMessage::transferOutputBuffer bufNum is ") + bufferNum);
        pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
        if (pBuffer == NULL) {
            trace(TRACE_LEVEL_DEVEL, "Dot1XLocalTestMessage::transferOutputBuffer received empty buffer");
            return;
        }
        addBuffer(bufferNum, size, pBuffer);
    }

}


