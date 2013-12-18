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
#include "SnmpServer/Local/SnmpServerIntSnmpConfigDelV3HostIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigDelV3HostIntMessage::SnmpServerIntSnmpConfigDelV3HostIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGDELV3HOSTINT)
    {
    }

    SnmpServerIntSnmpConfigDelV3HostIntMessage::SnmpServerIntSnmpConfigDelV3HostIntMessage (const HostUC &ip,const string &username)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGDELV3HOSTINT),
        m_ip    (ip),
        m_username    (username)
    {
    }

    SnmpServerIntSnmpConfigDelV3HostIntMessage::~SnmpServerIntSnmpConfigDelV3HostIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigDelV3HostIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"hostip"));
        addSerializableAttribute (new AttributeString(&m_username,"username"));
    }

    void  SnmpServerIntSnmpConfigDelV3HostIntMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC  SnmpServerIntSnmpConfigDelV3HostIntMessage::getHostUC() const
    {
        return (m_ip);
    }

    void  SnmpServerIntSnmpConfigDelV3HostIntMessage::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpServerIntSnmpConfigDelV3HostIntMessage::getUsername() const
    {
        return (m_username);
    }

}
