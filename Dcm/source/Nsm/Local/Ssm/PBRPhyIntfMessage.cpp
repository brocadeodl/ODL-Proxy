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
#include "Nsm/Local/Ssm/PBRPhyIntfMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    PBRPhyIntfMessage::PBRPhyIntfMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (), NSMLOCALPBRPHYINTF)
    {
        m_cmdCode = 0;
    }

    PBRPhyIntfMessage::PBRPhyIntfMessage (const string &ifName, const UI8 &ifType, const string &routeMapName, const UI32 &cmdCode)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (), NSMLOCALPBRPHYINTF),
        m_ifName    (ifName),
        m_ifType    (ifType),
        m_routeMapName (routeMapName),
        m_cmdCode   (cmdCode)
    {
    }

    PBRPhyIntfMessage::~PBRPhyIntfMessage ()
    {
    }

    void  PBRPhyIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI8(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_routeMapName,"routeMapName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  PBRPhyIntfMessage::setRouteMapName(const string &routeMapName)
    {
        m_routeMapName  =  routeMapName;
    }

    string  PBRPhyIntfMessage::getRouteMapName() const
    {
        return (m_routeMapName);
    }

    void  PBRPhyIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  PBRPhyIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  PBRPhyIntfMessage::setIfType(const UI8 &iftype)
    {
        m_ifType  =  iftype;
    }

    UI8  PBRPhyIntfMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  PBRPhyIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PBRPhyIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}

