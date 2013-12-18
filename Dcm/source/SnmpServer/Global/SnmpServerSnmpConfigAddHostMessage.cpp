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
#include "SnmpServer/Global/SnmpServerSnmpConfigAddHostMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerSnmpConfigAddHostMessage::SnmpServerSnmpConfigAddHostMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGADDHOST)
    {
    }

    SnmpServerSnmpConfigAddHostMessage::SnmpServerSnmpConfigAddHostMessage (const HostUC &ip,const Version &version,const UI32 &udpPort,const string &community,const SevLevel &sevLevel)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGADDHOST),
        m_ip    (ip),
        m_version    (version),
        m_udpPort    (udpPort),
        m_community    (community),
		m_sevLevel    (sevLevel)
    {
    }

    SnmpServerSnmpConfigAddHostMessage::~SnmpServerSnmpConfigAddHostMessage ()
    {
    }

    void  SnmpServerSnmpConfigAddHostMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"ip"));
        addSerializableAttribute (new AttributeEnum((UI32 * )&m_version,"version"));
        addSerializableAttribute (new AttributeUI32(&m_udpPort,"udpPort"));
        addSerializableAttribute (new AttributeString(&m_community, "community"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel"));
    }

    void  SnmpServerSnmpConfigAddHostMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC SnmpServerSnmpConfigAddHostMessage::getHostUC() const
    {
        return (m_ip);
    }

    void  SnmpServerSnmpConfigAddHostMessage::setVersion(const Version &version)
    {
        m_version  =  version;
    }

    Version  SnmpServerSnmpConfigAddHostMessage::getVersion() const
    {
        return (m_version);
    }

    void  SnmpServerSnmpConfigAddHostMessage::setUdpPort(const UI32 &udpPort)
    {
        m_udpPort  =  udpPort;
    }

    UI32  SnmpServerSnmpConfigAddHostMessage::getUdpPort() const
    {
        return (m_udpPort);
    }

    void  SnmpServerSnmpConfigAddHostMessage::setCommunity(const string &community)
    {
        m_community  =  community;
    }

    string  SnmpServerSnmpConfigAddHostMessage::getCommunity() const
    {
        return (m_community);
    }

    void  SnmpServerSnmpConfigAddHostMessage::setSevLevel(const SevLevel &sevLevel)
    {
        m_sevLevel  =  sevLevel;
    }

    SevLevel SnmpServerSnmpConfigAddHostMessage::getSevLevel() const
    {
        return (m_sevLevel);
    }
}
