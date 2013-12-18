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
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "IpPolicy/Framework/PBRShowMessage.h"
#include "IpPolicy/User/IpPolicyLocalWorkers.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"

namespace DcmNs
{

    PBRShowMessage::PBRShowMessage () : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId(), PBR_SHOW_ROUTEMAP)
    {
    }

    PBRShowMessage::PBRShowMessage(const UI32 &opCode, const string &routeMapName, const string &ifName,
            const string &rbridgeId, const UI32 &ifType, bool isGwRequest)
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), PBR_SHOW_ROUTEMAP),
            m_opCode        (opCode),
            m_routeMapName  (routeMapName),
            m_ifName        (ifName),
            m_rbridgeId     (rbridgeId),
            m_ifType        (ifType),
            m_isGwRequest   (isGwRequest),
            m_lastStanza	(0)
    {
    }

    PBRShowMessage::~PBRShowMessage ()
    {
    }

    void  PBRShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute(new AttributeString(&m_routeMapName,"routeMapName"));
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeString(&m_rbridgeId,"rbridgeId"));
        addSerializableAttribute(new AttributeUI8(&m_ifType,"ifType"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest,"isGwRequest"));
        addSerializableAttribute(new AttributeUI16(&m_lastStanza,"lastStanza"));
    }

    void  PBRShowMessage::setRouteMapName(const string &routeMapName)
    {
        m_routeMapName  =  routeMapName;
    }

    string  PBRShowMessage::getRouteMapName() const
    {
        return (m_routeMapName);
    }

    void  PBRShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  PBRShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  PBRShowMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  PBRShowMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void PBRShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool PBRShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    UI8  PBRShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void PBRShowMessage::setIfType(const UI8 &ifType)
    {
        m_ifType = ifType;
    }

    void PBRShowMessage::setRbridgeId(const string &rbridgeId)
    {
        m_rbridgeId = rbridgeId;
    }

    string PBRShowMessage::getRbridgeId() const
    {
        return m_rbridgeId;
    }

    void PBRShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void PBRShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

    void PBRShowMessage::setLastStanza(const UI16 &lastStannza) {
    	m_lastStanza = lastStannza;
    }

    UI16 PBRShowMessage::getLastStanza() const {
    	return m_lastStanza;
    }

}

