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
#include "SnmpServer/Global/SnmpServerSnmpConfigChangeHostMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerSnmpConfigChangeHostMessage::SnmpServerSnmpConfigChangeHostMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGCHANGEHOST)
    {
    }

    SnmpServerSnmpConfigChangeHostMessage::SnmpServerSnmpConfigChangeHostMessage (const HostUC &ip,const Version &version,const UI32 &udpPort,const string &community,const SevLevel &sevLevel)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGCHANGEHOST),
        m_ip    (ip),
        m_version    (version),
        m_udpPort    (udpPort),
        m_community    (community),
		m_sevLevel    (sevLevel)
    {
    }

    SnmpServerSnmpConfigChangeHostMessage::~SnmpServerSnmpConfigChangeHostMessage ()
    {
    }

    void  SnmpServerSnmpConfigChangeHostMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"ip"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_version,"version"));
        addSerializableAttribute (new AttributeUI32(&m_udpPort,"udpPort"));
        addSerializableAttribute (new AttributeString(&m_community, "community"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel"));
    }

    void  SnmpServerSnmpConfigChangeHostMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC SnmpServerSnmpConfigChangeHostMessage::getHostUC()  const
    {
        return (m_ip);
    }

    void  SnmpServerSnmpConfigChangeHostMessage::setVersion(const Version &version)
    {
        m_version  =  version;
    }

    Version  SnmpServerSnmpConfigChangeHostMessage::getVersion() const
    {
        return (m_version);
    }

    void  SnmpServerSnmpConfigChangeHostMessage::setUdpPort(const UI32 &udpPort)
    {
        m_udpPort  =  udpPort;
    }

    UI32  SnmpServerSnmpConfigChangeHostMessage::getUdpPort() const
    {
        return (m_udpPort);
    }

    void  SnmpServerSnmpConfigChangeHostMessage::setCommunity(const string &community)
    {
        m_community  =  community;
    }

    string  SnmpServerSnmpConfigChangeHostMessage::getCommunity() const
    {
        return (m_community);
    }

    void  SnmpServerSnmpConfigChangeHostMessage::setSevLevel(const SevLevel &sevLevel)
    {
        m_sevLevel  =  sevLevel;
    }

    SevLevel SnmpServerSnmpConfigChangeHostMessage::getSevLevel() const
    {
        return (m_sevLevel);
    }
}
