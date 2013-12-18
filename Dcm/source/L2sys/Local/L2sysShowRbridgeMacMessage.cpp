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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Eugene Gee
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "L2sys/Local/L2sysLocalObjectManager.h"
#include "L2sys/Local/L2sysTypes.h"
#include "L2sys/Local/L2sysShowRbridgeMacMessage.h"

namespace DcmNs
{
    L2sysShowRbridgeMacMessage::L2sysShowRbridgeMacMessage ()
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSSHOWRBRIDGEMAC)
    {
        m_cmdCode = 0;
        m_ifindex = 0;
        m_isGwRequest = true;
        m_from_rbridge_id = 0;
        m_for_rbridge_id = 0;
        m_type = 0;
    }

    L2sysShowRbridgeMacMessage::L2sysShowRbridgeMacMessage (const string &ifName,const UI32 &cmdCode,
        const UI32 &ifindex, const UI32 &type, const bool isGwRequest,
        const UI32 &from_rbridge_id, const UI32 &for_rbridge_id)
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSSHOWRBRIDGEMAC),
       m_ifName    (ifName),
        m_cmdCode   (cmdCode),
        m_ifindex   (ifindex),
        m_type   (type),
        m_isGwRequest  (isGwRequest),
        m_from_rbridge_id (from_rbridge_id),
        m_for_rbridge_id (for_rbridge_id)
    {
    }

    L2sysShowRbridgeMacMessage::~L2sysShowRbridgeMacMessage ()
    {
    }

    void  L2sysShowRbridgeMacMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifindex,"ifindex"));
        addSerializableAttribute (new AttributeUI32(&m_type,"type"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeBool(&m_isGwRequest,"isGwRequest"));
        addSerializableAttribute (new AttributeUI32(&m_from_rbridge_id,"from_rbridge_id"));
        addSerializableAttribute (new AttributeUI32(&m_for_rbridge_id,"for_rbridge_id"));
    }

    void  L2sysShowRbridgeMacMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  L2sysShowRbridgeMacMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  L2sysShowRbridgeMacMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  L2sysShowRbridgeMacMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  L2sysShowRbridgeMacMessage::setIfindex(const UI32 &ifindex)
    {
        m_ifindex  =  ifindex;
    }

    UI32  L2sysShowRbridgeMacMessage::getIfindex() const
    {
        return (m_ifindex);
    }

    void  L2sysShowRbridgeMacMessage::setType(const UI32 &type)
    {
        m_type  =  type;
    }

    UI32  L2sysShowRbridgeMacMessage::getType() const
    {
        return (m_type);
    }

    void L2sysShowRbridgeMacMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool L2sysShowRbridgeMacMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void  L2sysShowRbridgeMacMessage::setFromRbridgeId(const UI32 &from_rbridge_id)
    {
        m_from_rbridge_id = from_rbridge_id;
    }

    UI32  L2sysShowRbridgeMacMessage::getFromRbridgeId() const
    {
        return (m_from_rbridge_id);
    }

    void  L2sysShowRbridgeMacMessage::setForRbridgeId(const UI32 &for_rbridge_id)
    {
        m_for_rbridge_id = for_rbridge_id;
    }

    UI32  L2sysShowRbridgeMacMessage::getForRbridgeId() const
    {
        return (m_for_rbridge_id);
    }

    void L2sysShowRbridgeMacMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void L2sysShowRbridgeMacMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }
}
