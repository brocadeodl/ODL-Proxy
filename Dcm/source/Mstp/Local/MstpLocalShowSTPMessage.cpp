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
 *   Author : Sudheendra                                                   *
 **************************************************************************/

#include "Mstp/Local/MstpLocalShowSTPMessage.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "Mstp/Local/MstpTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    MstpLocalShowSTPMessage::MstpLocalShowSTPMessage ()
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPLOCALSHOWSTP)
    {
        m_cmdCode       = 0; //STP_SHOW_UNKNOWN;
        m_ifType        = IF_TYPE_INVALID;
        m_instId        = 0;
        m_reqType       = REQ_TYPE_GET;
        m_lastInstId    = 0;
        m_isGwRequest   = false;
        m_locationId    = 0;
    }

    MstpLocalShowSTPMessage::MstpLocalShowSTPMessage (MstpLocalShowSTPMessage *other)
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPLOCALSHOWSTP)
        , m_ifName      ( other->m_ifName )
    {
        m_cmdCode       = other->m_cmdCode;
        m_ifType        = other->m_ifType;
        m_instId        = other->m_instId;
        m_reqType       = other->m_reqType;
        m_lastInstId    = other->m_lastInstId;
        m_isGwRequest   = other->m_isGwRequest;
        m_locationId    = other->m_locationId;
    }

    MstpLocalShowSTPMessage::~MstpLocalShowSTPMessage ()
    {
    }

    void  MstpLocalShowSTPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization();

        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
        addSerializableAttribute(new AttributeUI32(&m_locationId, "locationId"));

        addSerializableAttribute(new AttributeUI32(&m_reqType, "reqType"));
        addSerializableAttribute(new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute(new AttributeEnum((UI32*)(&m_ifType), "ifType"));
        addSerializableAttribute(new AttributeString(&m_ifName, "ifName"));
        addSerializableAttribute(new AttributeUI32(&m_instId, "instId"));

        addSerializableAttribute(new AttributeUI32(&m_lastInstId, "lastInstId"));
    }


    void  MstpLocalShowSTPMessage::setInterfaceInfo (InterfaceType ifType, const string &ifName)
    {
        m_ifType  =  ifType;
        m_ifName  =  ifName;
    }

    string  MstpLocalShowSTPMessage::getIfName() const
    {
        return (m_ifName);
    }

    InterfaceType MstpLocalShowSTPMessage::getIfType () const
    {
        return m_ifType;
    }

    void  MstpLocalShowSTPMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpLocalShowSTPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void MstpLocalShowSTPMessage::setLastIndex (UI32 lastInstId)
    {
        m_lastInstId = lastInstId;
    }

    void MstpLocalShowSTPMessage::setLocationId(LocationId locationId)
    {
        m_locationId = locationId;
    }

    LocationId MstpLocalShowSTPMessage::getLocationId()  const
    {
        return m_locationId;
    }

    void MstpLocalShowSTPMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool MstpLocalShowSTPMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void MstpLocalShowSTPMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }


}
