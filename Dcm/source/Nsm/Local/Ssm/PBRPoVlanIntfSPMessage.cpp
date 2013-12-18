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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author Debojyoti                                                      *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/Ssm/PBRPoVlanIntfSPMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    PBRPoVlanIntfSPMessage::PBRPoVlanIntfSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSMLOCALPBRPOINTF)
    {
        m_cmdCode = 0;
        m_ifId = 0;
        m_ifType = 0;
    }

    PBRPoVlanIntfSPMessage::PBRPoVlanIntfSPMessage (const UI32 &ifId, const UI8 &ifType, const string &routeMapName, const UI32 &cmdCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSMLOCALPBRPOINTF),
        m_ifId      (ifId),
        m_ifType    (ifType),
        m_routeMapName (routeMapName),
        m_cmdCode   (cmdCode)
    {
    }

    PBRPoVlanIntfSPMessage::~PBRPoVlanIntfSPMessage ()
    {
    }

    void  PBRPoVlanIntfSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ifId,"ifId"));
        addSerializableAttribute (new AttributeUI8(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_routeMapName,"routeMapName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  PBRPoVlanIntfSPMessage::setRouteMapName(const string &routeMapName)
    {
        m_routeMapName  =  routeMapName;
    }

    string  PBRPoVlanIntfSPMessage::getRouteMapName() const
    {
        return (m_routeMapName);
    }

    void  PBRPoVlanIntfSPMessage::setIfId(const UI32 &ifId)
    {
        m_ifId  =  ifId;
    }

    UI32  PBRPoVlanIntfSPMessage::getIfId() const
    {
        return (m_ifId);
    }

    UI8  PBRPoVlanIntfSPMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  PBRPoVlanIntfSPMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PBRPoVlanIntfSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}

