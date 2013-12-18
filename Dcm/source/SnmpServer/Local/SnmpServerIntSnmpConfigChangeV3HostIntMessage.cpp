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
#include "SnmpServer/Local/SnmpServerIntSnmpConfigChangeV3HostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigChangeV3HostIntMessage::SnmpServerIntSnmpConfigChangeV3HostIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGCHANGEV3HOSTINT)
    {
    }

    SnmpServerIntSnmpConfigChangeV3HostIntMessage::SnmpServerIntSnmpConfigChangeV3HostIntMessage (const HostUC &ip,const string &username,const NotifyType &notifyType,const UI32 &udpPort,const string &engineId,const SevLevel &sevLevel)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGCHANGEV3HOSTINT),
        m_ip    (ip),
        m_notify    (notifyType),
        m_udpPort    (udpPort),
        m_username    (username),
        m_engineid    (engineId),
		m_sevLevel    (sevLevel)
    {
    }

    SnmpServerIntSnmpConfigChangeV3HostIntMessage::~SnmpServerIntSnmpConfigChangeV3HostIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigChangeV3HostIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"hostip"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_notify,"notify"));
        addSerializableAttribute (new AttributeUI32(&m_udpPort,"udpPort"));
        addSerializableAttribute (new AttributeString(&m_username,"username"));
        addSerializableAttribute (new AttributeString(&m_engineid,"engineid"));
		addSerializableAttribute (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel"));
    }

    void  SnmpServerIntSnmpConfigChangeV3HostIntMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC  SnmpServerIntSnmpConfigChangeV3HostIntMessage::getHostUC() const
    {
        return (m_ip);
    }

    void  SnmpServerIntSnmpConfigChangeV3HostIntMessage::setNotify(const NotifyType &notify)
    {
        m_notify  =  notify;
    }

    NotifyType  SnmpServerIntSnmpConfigChangeV3HostIntMessage::getNotify() const
    {
        return (m_notify);
    }

    void  SnmpServerIntSnmpConfigChangeV3HostIntMessage::setUdpPort(const UI32 &udpPort)
    {
        m_udpPort  =  udpPort;
    }

    UI32  SnmpServerIntSnmpConfigChangeV3HostIntMessage::getUdpPort() const
    {
        return (m_udpPort);
    }

    void  SnmpServerIntSnmpConfigChangeV3HostIntMessage::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpServerIntSnmpConfigChangeV3HostIntMessage::getUsername() const
    {
        return (m_username);
    }
    void  SnmpServerIntSnmpConfigChangeV3HostIntMessage::setEngineid(const string &engineid)
    {
        m_engineid  =  engineid;
    }

    string  SnmpServerIntSnmpConfigChangeV3HostIntMessage::getEngineid() const
    {
        return (m_engineid);
    }

    void  SnmpServerIntSnmpConfigChangeV3HostIntMessage::setSevLevel(const SevLevel &sevLevel)
    {
        m_sevLevel  =  sevLevel;
    }

    SevLevel SnmpServerIntSnmpConfigChangeV3HostIntMessage::getSevLevel() const
    {
        return (m_sevLevel);
    }



}
