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

#include "SnmpServer/Global/SnmpServerSnmpConfigSetLocationMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"

namespace DcmNs
{

    SnmpServerSnmpConfigSetLocationMessage::SnmpServerSnmpConfigSetLocationMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGSETLOCATION)
    {
    }

    SnmpServerSnmpConfigSetLocationMessage::SnmpServerSnmpConfigSetLocationMessage (const string &location)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGSETLOCATION),
        m_location    (location)
    {
    }

    SnmpServerSnmpConfigSetLocationMessage::~SnmpServerSnmpConfigSetLocationMessage ()
    {
    }

    void  SnmpServerSnmpConfigSetLocationMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_location,"location"));
    }

    void  SnmpServerSnmpConfigSetLocationMessage::setLocation(const string &location)
    {
        m_location  =  location;
    }

    string  SnmpServerSnmpConfigSetLocationMessage::getLocation() const
    {
        return (m_location);
    }

}
