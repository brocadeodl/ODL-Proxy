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

#include "SnmpServer/Global/SnmpServerSnmpConfigRemoveCommMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
namespace DcmNs
{

    SnmpServerSnmpConfigRemoveCommMessage::SnmpServerSnmpConfigRemoveCommMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGREMOVECOMM)
    {
    }

    SnmpServerSnmpConfigRemoveCommMessage::SnmpServerSnmpConfigRemoveCommMessage (const string &community)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGREMOVECOMM),
        m_community    (community)
    {
    }

    SnmpServerSnmpConfigRemoveCommMessage::~SnmpServerSnmpConfigRemoveCommMessage ()
    {
    }

    void  SnmpServerSnmpConfigRemoveCommMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_community,"community"));
		addSerializableAttribute (new AttributeEnum((UI32 *)&m_access, "access"));
    }

    void  SnmpServerSnmpConfigRemoveCommMessage::setCommunity(const string &community)
    {
        m_community  =  community;
    }

    string  SnmpServerSnmpConfigRemoveCommMessage::getCommunity() const
    {
        return (m_community);
    }

    void  SnmpServerSnmpConfigRemoveCommMessage::setAccess(const Access &access)
    {
        m_access  =  access;
    }

    Access  SnmpServerSnmpConfigRemoveCommMessage::getAccess() const
    {
        return (m_access);
    }

}
