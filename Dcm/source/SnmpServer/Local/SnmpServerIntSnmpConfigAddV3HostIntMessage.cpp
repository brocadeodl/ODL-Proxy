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
#include "SnmpServer/Local/SnmpServerIntSnmpConfigAddV3HostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigAddV3HostIntMessage::SnmpServerIntSnmpConfigAddV3HostIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGADDV3HOSTINT)
    {
    }

    SnmpServerIntSnmpConfigAddV3HostIntMessage::SnmpServerIntSnmpConfigAddV3HostIntMessage (const HostUC &ip,const string &username,const NotifyType &notifyType,const UI32 &udpPort,const string &engineId,const SevLevel &sevLevel)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGADDV3HOSTINT),
        m_ip    (ip),
        m_notify    (notifyType),
        m_udpPort    (udpPort),
        m_username    (username),
        m_engineid    (engineId),
		m_sevLevel    (sevLevel)
    {
    }

    SnmpServerIntSnmpConfigAddV3HostIntMessage::~SnmpServerIntSnmpConfigAddV3HostIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigAddV3HostIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"hostip"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_notify,"notify"));
        addSerializableAttribute (new AttributeUI32(&m_udpPort,"udpPort"));
        addSerializableAttribute (new AttributeString(&m_username,"username"));
        addSerializableAttribute (new AttributeString(&m_engineid,"engineid"));
		addSerializableAttribute (new AttributeEnum((UI32 *)&m_sevLevel,"sevLevel"));
    }

    void  SnmpServerIntSnmpConfigAddV3HostIntMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC  SnmpServerIntSnmpConfigAddV3HostIntMessage::getHostUC() const
    {
        return (m_ip);
    }

    void  SnmpServerIntSnmpConfigAddV3HostIntMessage::setNotify(const NotifyType &notify)
    {
        m_notify  =  notify;
    }

    NotifyType  SnmpServerIntSnmpConfigAddV3HostIntMessage::getNotify() const
    {
        return (m_notify);
    }

    void  SnmpServerIntSnmpConfigAddV3HostIntMessage::setUdpPort(const UI32 &udpPort)
    {
        m_udpPort  =  udpPort;
    }

    UI32  SnmpServerIntSnmpConfigAddV3HostIntMessage::getUdpPort() const
    {
        return (m_udpPort);
    }

    void  SnmpServerIntSnmpConfigAddV3HostIntMessage::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpServerIntSnmpConfigAddV3HostIntMessage::getUsername() const
    {
        return (m_username);
    }
    void  SnmpServerIntSnmpConfigAddV3HostIntMessage::setEngineid(const string &engineid)
    {
        m_engineid  =  engineid;
    }

    string  SnmpServerIntSnmpConfigAddV3HostIntMessage::getEngineid() const
    {
        return (m_engineid);
    }

    void  SnmpServerIntSnmpConfigAddV3HostIntMessage::setSevLevel(const SevLevel &sevLevel)
    {
        m_sevLevel  =  sevLevel;
    }

    SevLevel SnmpServerIntSnmpConfigAddV3HostIntMessage::getSevLevel() const
    {
        return (m_sevLevel);
    }
}
