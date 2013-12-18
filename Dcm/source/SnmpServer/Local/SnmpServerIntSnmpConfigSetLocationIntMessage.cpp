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

#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetLocationIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigSetLocationIntMessage::SnmpServerIntSnmpConfigSetLocationIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGSETLOCATIONINT)
    {
    }

    SnmpServerIntSnmpConfigSetLocationIntMessage::SnmpServerIntSnmpConfigSetLocationIntMessage (const string &location)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGSETLOCATIONINT),
        m_location    (location)
    {
    }

    SnmpServerIntSnmpConfigSetLocationIntMessage::~SnmpServerIntSnmpConfigSetLocationIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigSetLocationIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_location,"location"));
    }

    void  SnmpServerIntSnmpConfigSetLocationIntMessage::setLocation(const string &location)
    {
        m_location  =  location;
    }

    string  SnmpServerIntSnmpConfigSetLocationIntMessage::getLocation() const
    {
        return (m_location);
    }

}
