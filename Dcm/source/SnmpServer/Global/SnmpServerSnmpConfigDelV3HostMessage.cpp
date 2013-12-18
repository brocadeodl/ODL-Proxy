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
#include "SnmpServer/Global/SnmpServerSnmpConfigDelV3HostMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeHostUC.h"

namespace DcmNs
{

    SnmpServerSnmpConfigDelV3HostMessage::SnmpServerSnmpConfigDelV3HostMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGDELV3HOST)
    {
    }

    SnmpServerSnmpConfigDelV3HostMessage::SnmpServerSnmpConfigDelV3HostMessage (const HostUC &ip,const string &username)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGDELV3HOST),
        m_ip    (ip),
        m_username    (username)
    {
    }

    SnmpServerSnmpConfigDelV3HostMessage::~SnmpServerSnmpConfigDelV3HostMessage ()
    {
    }

    void  SnmpServerSnmpConfigDelV3HostMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeHostUC(&m_ip,false,"hostip"));
        addSerializableAttribute (new AttributeString(&m_username,"username"));
    }

    void  SnmpServerSnmpConfigDelV3HostMessage::setHostUC(const HostUC &ip)
    {
        m_ip  =  ip;
    }

    HostUC  SnmpServerSnmpConfigDelV3HostMessage::getHostUC() const
    {
        return (m_ip);
    }

    void  SnmpServerSnmpConfigDelV3HostMessage::setUsername(const string &username)
    {
        m_username  =  username;
    }

    string  SnmpServerSnmpConfigDelV3HostMessage::getUsername() const
    {
        return (m_username);
    }

}
