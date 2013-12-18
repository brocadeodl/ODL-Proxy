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
 *   Author : Sudha Balodia
 **************************************************************************/
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Dot1XLocalClearMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    Dot1XLocalClearMessage::Dot1XLocalClearMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), DOT1X_LOCAL_CLEAR_ALL)
    {
    }

    Dot1XLocalClearMessage::Dot1XLocalClearMessage (const string &Interface, const string &ifName, const UI32 &cmdCode, bool isGwRequest)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), DOT1X_LOCAL_CLEAR_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
        m_cmdCode   (cmdCode),
        m_isGwRequest (isGwRequest)
    {
    }

    Dot1XLocalClearMessage::~Dot1XLocalClearMessage ()
    {
    }

    void  Dot1XLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
    }

    void  Dot1XLocalClearMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  Dot1XLocalClearMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  Dot1XLocalClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  Dot1XLocalClearMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  Dot1XLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  Dot1XLocalClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void Dot1XLocalClearMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool Dot1XLocalClearMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void Dot1XLocalClearMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void Dot1XLocalClearMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

    void Dot1XLocalClearMessage::transferOutputBuffer(Dot1XLocalClearMessage *&pMsg)
    {
        UI32 size = 0;
        UI32 bufferNum = pMsg->getCmdCode();
        void *pBuffer = NULL;

        trace(TRACE_LEVEL_DEVEL, string("Dot1XLocalClearMessage::transferOutputBuffer bufNum is ") + bufferNum);
        pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
        if (pBuffer == NULL) {
            trace(TRACE_LEVEL_DEVEL, "Dot1XLocalClearMessage::transferOutputBuffer received empty buffer");
            return;
        }
        addBuffer(bufferNum, size, pBuffer);
    }

}
