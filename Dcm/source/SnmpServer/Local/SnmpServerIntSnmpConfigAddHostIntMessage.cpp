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
#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddHostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigAddHostIntMessage::SnmpServerIntSnmpConfigAddHostIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGADDHOSTINT)
    {
    }

    SnmpServerIntSnmpConfigAddHostIntMessage::SnmpServerIntSnmpConfigAddHostIntMessage (const HostUC &ip,const Version &version,const UI32 &udpPort,const string &community,const SevLevel &sevLevel)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGADDHOSTINT),
        m_ip    (ip),
        m_version    (version),
        m_udpPort    (udpPort),
        m_community    (community),
        m_sevLevel    (sevLevel)
    {
    }

    SnmpServerIntSnmpConfigAddHostIntMessage::~SnmpServerIntSnmpConfigAddHostIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigAddHostIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"ip"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_version,"version"));
        addSerializableAttribute (new AttributeUI32(&m_udpPort,"udpPort"));
        addSerializableAttribute (new AttributeString(&m_community,"community"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel"));
    }

    void  SnmpServerIntSnmpConfigAddHostIntMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC SnmpServerIntSnmpConfigAddHostIntMessage::getHostUC() const
    {
        return (m_ip);
    }

    void  SnmpServerIntSnmpConfigAddHostIntMessage::setVersion(const Version &version)
    {
        m_version  =  version;
    }

    Version  SnmpServerIntSnmpConfigAddHostIntMessage::getVersion() const
    {
        return (m_version);
    }

    void  SnmpServerIntSnmpConfigAddHostIntMessage::setUdpPort(const UI32 &udpPort)
    {
        m_udpPort  =  udpPort;
    }

    UI32  SnmpServerIntSnmpConfigAddHostIntMessage::getUdpPort() const
    {
        return (m_udpPort);
    }

    void  SnmpServerIntSnmpConfigAddHostIntMessage::setCommunity(const string &community)
    {
        m_community  =  community;
    }

    string  SnmpServerIntSnmpConfigAddHostIntMessage::getCommunity() const
    {
        return (m_community);
    }

    void  SnmpServerIntSnmpConfigAddHostIntMessage::setSevLevel(const SevLevel &sevLevel)
    {
        m_sevLevel  =  sevLevel;
    }

    SevLevel SnmpServerIntSnmpConfigAddHostIntMessage::getSevLevel() const
    {
        return (m_sevLevel);
    }


    SnmpServerIntConfigureDefaultsMessage::SnmpServerIntConfigureDefaultsMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTCONFIGUREDEFAULTS)
    {
    }

    SnmpServerIntConfigureDefaultsMessage::~SnmpServerIntConfigureDefaultsMessage ()
    {
    }

    void  SnmpServerIntConfigureDefaultsMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
    }
}
