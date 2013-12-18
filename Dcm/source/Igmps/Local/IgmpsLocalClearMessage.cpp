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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#include "Igmps/Local/IgmpsLocalClearMessage.h"
#include "Igmps/Local/IgmpsLocalObjectManager.h"
#include "Igmps/Local/IgmpsTypes.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"

namespace DcmNs
{

    IgmpsLocalClearMessage::IgmpsLocalClearMessage()
        : PrismMessage (IgmpsLocalObjectManager::getPrismServiceId (), IGMPSLOCALCLEAR),
        m_cmdCode       (IGMPS_DEFAULT_CMD_CODE),
        m_isGwRequest   (false)
    {
    }

    IgmpsLocalClearMessage::IgmpsLocalClearMessage(const string &grpName, const string &ifName, const UI32 &cmdCode, bool isGwRequest)
        : PrismMessage (IgmpsLocalObjectManager::getPrismServiceId (), IGMPSLOCALCLEAR),
        m_grpName      (grpName),
        m_ifName       (ifName),
        m_cmdCode       (cmdCode),
        m_isGwRequest (isGwRequest)
    {
    }

    IgmpsLocalClearMessage::~IgmpsLocalClearMessage ()
    {
    }

    void  IgmpsLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_grpName,"grpName"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
    }

    void  IgmpsLocalClearMessage::setGrpName(const string &grpName)
    {
        m_grpName  =  grpName;
    }

    string  IgmpsLocalClearMessage::getGrpName() const
    {
        return (m_grpName);
    }
    
    void  IgmpsLocalClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsLocalClearMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  IgmpsLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  IgmpsLocalClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void IgmpsLocalClearMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool IgmpsLocalClearMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void IgmpsLocalClearMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void IgmpsLocalClearMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {   
        this->copyBuffersFrom(prismMessage);
    }
}
