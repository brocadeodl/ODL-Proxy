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
 *   Author : Debojyoti                                                     *
 **************************************************************************/

#include "IpPolicy/Framework/IpPolicyGetPBRObjectMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"

namespace DcmNs
{

    IpPolicyGetPBRObjectMessage::IpPolicyGetPBRObjectMessage ()
        : ManagementInterfaceMessage (IpPolicyLocalObjectManager::getClassName (),IPPOLICYGETPBR)
    {
    }

    IpPolicyGetPBRObjectMessage::IpPolicyGetPBRObjectMessage (const string &routeMapName)
        : ManagementInterfaceMessage (IpPolicyLocalObjectManager::getClassName (),IPPOLICYGETPBR),
        m_routeMapName    (routeMapName)
    {
    }

    IpPolicyGetPBRObjectMessage::~IpPolicyGetPBRObjectMessage ()
    {
    }

    void  IpPolicyGetPBRObjectMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_routeMapName,"routeMapName"));
        addSerializableAttribute (new AttributeObjectId(&m_routeMapObjId,"routeMapObjId"));
    }

    void  IpPolicyGetPBRObjectMessage::setPBRRouteMapName(const string &routeMapName)
    {
        m_routeMapName =  routeMapName;
    }

    string  IpPolicyGetPBRObjectMessage::getPBRRouteMapName() const
    {
        return (m_routeMapName);
    }

    void  IpPolicyGetPBRObjectMessage::setPBRRouteMapObjId(const ObjectId &routeMapId)
    {
        m_routeMapObjId  =  routeMapId;
    }

    ObjectId  IpPolicyGetPBRObjectMessage::getPBRRouteMapObjId() const
    {
        return (m_routeMapObjId);
    }

}

