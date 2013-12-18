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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "SnmpServer/Global/SnmpServerSnmpConfigAddCommMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    SnmpServerSnmpConfigAddCommMessage::SnmpServerSnmpConfigAddCommMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGADDCOMM)
    {
    }

    SnmpServerSnmpConfigAddCommMessage::SnmpServerSnmpConfigAddCommMessage (const string &community,const Access &access)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGADDCOMM),
        m_community    (community),
        m_access    (access)
    {
    }

    SnmpServerSnmpConfigAddCommMessage::~SnmpServerSnmpConfigAddCommMessage ()
    {
    }

    void  SnmpServerSnmpConfigAddCommMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_community,"community"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_access, "access"));
    }

    void  SnmpServerSnmpConfigAddCommMessage::setCommunity(const string &community)
    {
        m_community  =  community;
    }

    string  SnmpServerSnmpConfigAddCommMessage::getCommunity() const
    {
        return (m_community);
    }

    void  SnmpServerSnmpConfigAddCommMessage::setAccess(const Access &access)
    {
        m_access  =  access;
    }

    Access  SnmpServerSnmpConfigAddCommMessage::getAccess() const
    {
        return (m_access);
    }

}
