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
#include "SnmpServer/Local/SnmpServerIntSnmpConfigDelHostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigDelHostIntMessage::SnmpServerIntSnmpConfigDelHostIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGDELHOSTINT)
    {
    }

		SnmpServerIntSnmpConfigDelHostIntMessage::SnmpServerIntSnmpConfigDelHostIntMessage (const HostUC &ip,const string &community)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGDELHOSTINT),
        m_ip    (ip),
        m_community    (community)
    {
    }

    SnmpServerIntSnmpConfigDelHostIntMessage::~SnmpServerIntSnmpConfigDelHostIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigDelHostIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"ip"));
        addSerializableAttribute (new AttributeString(&m_community,"community"));
    }

    void  SnmpServerIntSnmpConfigDelHostIntMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC SnmpServerIntSnmpConfigDelHostIntMessage::getHostUC()  const
    {
        return (m_ip);
    }

    void  SnmpServerIntSnmpConfigDelHostIntMessage::setCommunity(const string &community)
    {
        m_community  =  community;
    }

    string  SnmpServerIntSnmpConfigDelHostIntMessage::getCommunity() const
    {
        return (m_community);
    }

}
