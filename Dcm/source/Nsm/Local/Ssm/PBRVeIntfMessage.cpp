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
 *   Author Debojyoti													   *
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/Ssm/PBRVeIntfMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    PBRVeIntfMessage::PBRVeIntfMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (), NSMLOCALPBRVEINTF)
    {
        m_cmdCode = 0;
        m_ifId = 0;
        m_ifType = 0;
    }

    PBRVeIntfMessage::PBRVeIntfMessage (const UI32 &ifId, const UI8 &ifType, const string &routeMapName, const UI32 &cmdCode)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMLOCALPBRVEINTF),
        m_ifId      (ifId),
        m_ifType    (ifType),
        m_routeMapName (routeMapName),
        m_cmdCode   (cmdCode)
    {
    }

    PBRVeIntfMessage::~PBRVeIntfMessage ()
    {
    }

    void  PBRVeIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_ifId,"ifId"));
        addSerializableAttribute (new AttributeUI8(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_routeMapName,"routeMapName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  PBRVeIntfMessage::setRouteMapName(const string &routeMapName)
    {
        m_routeMapName  =  routeMapName;
    }

    string  PBRVeIntfMessage::getRouteMapName() const
    {
        return (m_routeMapName);
    }

    void  PBRVeIntfMessage::setIfId(const UI32 &ifId)
    {
        m_ifId  =  ifId;
    }

    UI32  PBRVeIntfMessage::getIfId() const
    {
        return (m_ifId);
    }

    void  PBRVeIntfMessage::setIfType(const UI8 &iftype)
    {
        m_ifType  =  iftype;
    }

    UI8  PBRVeIntfMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  PBRVeIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PBRVeIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}

