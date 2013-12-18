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

#include "Igmps/Local/IgmpsLocalDebugMessage.h"
#include "Igmps/Local/IgmpsLocalObjectManager.h"
#include "Igmps/Local/IgmpsTypes.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"

namespace DcmNs
{

    IgmpsLocalDebugMessage::IgmpsLocalDebugMessage()
        : PrismMessage (IgmpsLocalObjectManager::getPrismServiceId (), IGMPSLOCALDEBUG),
        m_cmdCode       (IGMPS_DEFAULT_CMD_CODE),
        m_isNegation    (false)
    {
    }

    IgmpsLocalDebugMessage::IgmpsLocalDebugMessage(const string &grpName, const string &ifName, const UI32 &cmdCode, bool isNegation)
        : PrismMessage (IgmpsLocalObjectManager::getPrismServiceId (), IGMPSLOCALDEBUG),
        m_grpName      (grpName),
        m_ifName       (ifName),
        m_cmdCode       (cmdCode),
        m_isNegation (isNegation)
    {
    }

    IgmpsLocalDebugMessage::~IgmpsLocalDebugMessage ()
    {
    }

    void  IgmpsLocalDebugMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_grpName,"grpName"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeBool(&m_isNegation, "isNegation"));
    }

    void  IgmpsLocalDebugMessage::setGrpName(const string &grpName)
    {
        m_grpName  =  grpName;
    }

    string  IgmpsLocalDebugMessage::getGrpName() const
    {
        return (m_grpName);
    }
    
    void  IgmpsLocalDebugMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsLocalDebugMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  IgmpsLocalDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  IgmpsLocalDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void IgmpsLocalDebugMessage::setNegation (bool isNegation)
    {
        m_isNegation = isNegation;
    }

    bool IgmpsLocalDebugMessage::getNegation () const
    {
        return (m_isNegation);
    }

    void IgmpsLocalDebugMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void IgmpsLocalDebugMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {   
        this->copyBuffersFrom(prismMessage);
    }
      
}
