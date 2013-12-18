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
#include "SnmpServer/Global/SnmpServerSnmpConfigChangeV3HostMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeHostUC.h"
//#include "snmp.h"
#include "vcs.h"
namespace DcmNs
{

    SnmpServerSnmpConfigChangeV3HostMessage::SnmpServerSnmpConfigChangeV3HostMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGCHANGEV3HOST)
    {
    }

    SnmpServerSnmpConfigChangeV3HostMessage::SnmpServerSnmpConfigChangeV3HostMessage (const HostUC &ip,const string &username, const NotifyType &notify,const UI32 &udpPort,const string &engineId, const SevLevel &sevLevel)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGCHANGEV3HOST),
        m_ip    (ip),
        m_nType    (notify),
        m_udpPort    (udpPort),
        m_username    (username),
        m_engineId (engineId),
        m_sevLevel (sevLevel)
    {
    }

    SnmpServerSnmpConfigChangeV3HostMessage::~SnmpServerSnmpConfigChangeV3HostMessage ()
    {
    }

    void  SnmpServerSnmpConfigChangeV3HostMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"hostip"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_nType,"notify", snmp_server_notifytype));
        addSerializableAttribute (new AttributeUI32(&m_udpPort,"udpPort", snmp_server_udp_port));
        addSerializableAttribute (new AttributeString(&m_username, "username", snmp_server_username));
        addSerializableAttribute (new AttributeString(&m_engineId, "engineId", snmp_server_engineid));
		addSerializableAttribute (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel", snmp_server_severity_level));
    }

    void  SnmpServerSnmpConfigChangeV3HostMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC  SnmpServerSnmpConfigChangeV3HostMessage::getHostUC() const
    {
        return (m_ip);
    }

    void  SnmpServerSnmpConfigChangeV3HostMessage::setNotify(const NotifyType &notify)
    {
        m_nType  =  notify;
    }

    NotifyType  SnmpServerSnmpConfigChangeV3HostMessage::getNotify() const
    {
        return (m_nType);
    }

    void  SnmpServerSnmpConfigChangeV3HostMessage::setUdpPort(const UI32 &udpPort)
    {
        m_udpPort  =  udpPort;
    }

    UI32  SnmpServerSnmpConfigChangeV3HostMessage::getUdpPort() const
    {
        return (m_udpPort);
    }

    void  SnmpServerSnmpConfigChangeV3HostMessage::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpServerSnmpConfigChangeV3HostMessage::getUsername() const
    {
        return (m_username);
    }
    void  SnmpServerSnmpConfigChangeV3HostMessage::setEngineid(const string &engineId)
    {
        m_engineId  =  engineId;
    }

    string  SnmpServerSnmpConfigChangeV3HostMessage::getEngineid() const
    {
        return (m_engineId);
    }

    void  SnmpServerSnmpConfigChangeV3HostMessage::setSevLevel(const SevLevel &sevLevel)
    {
        m_sevLevel  =  sevLevel;
    }

    SevLevel SnmpServerSnmpConfigChangeV3HostMessage::getSevLevel() const
    {
        return (m_sevLevel);
    }


}
