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

#include "Framework/Types/HostUC.h"
#include "SnmpServer/Global/SnmpServerSnmpConfigDelHostMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerSnmpConfigDelHostMessage::SnmpServerSnmpConfigDelHostMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGDELHOST)
    {
    }

    SnmpServerSnmpConfigDelHostMessage::SnmpServerSnmpConfigDelHostMessage (const HostUC &ip,const string &community)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGDELHOST),
        m_ip    (ip),
        m_community    (community)
    {
    }

    SnmpServerSnmpConfigDelHostMessage::~SnmpServerSnmpConfigDelHostMessage ()
    {
    }

    void  SnmpServerSnmpConfigDelHostMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"ip"));
        addSerializableAttribute (new AttributeString(&m_community,"community"));
    }

    void  SnmpServerSnmpConfigDelHostMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC SnmpServerSnmpConfigDelHostMessage::getHostUC()  const
    {
        return (m_ip);
    }

    void  SnmpServerSnmpConfigDelHostMessage::setCommunity(const string &community)
    {
        m_community  =  community;
    }

    string  SnmpServerSnmpConfigDelHostMessage::getCommunity() const
    {
        return (m_community);
    }

}
