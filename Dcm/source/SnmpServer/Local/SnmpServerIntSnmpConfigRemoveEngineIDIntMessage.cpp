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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skumaras                                                     *
 **************************************************************************/

#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveEngineIDIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigRemoveEngineIDIntMessage::SnmpServerIntSnmpConfigRemoveEngineIDIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGREMOVEENGINEIDINT)
    {
    }

    SnmpServerIntSnmpConfigRemoveEngineIDIntMessage::SnmpServerIntSnmpConfigRemoveEngineIDIntMessage (const string &EngineID)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGREMOVEENGINEIDINT),
        m_EngineID    (EngineID)
    {
    }

    SnmpServerIntSnmpConfigRemoveEngineIDIntMessage::~SnmpServerIntSnmpConfigRemoveEngineIDIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigRemoveEngineIDIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_EngineID,"EngineID"));
    }

    void  SnmpServerIntSnmpConfigRemoveEngineIDIntMessage::setEngineID(const string &EngineID)
    {
        m_EngineID  =  EngineID;
    }

    string  SnmpServerIntSnmpConfigRemoveEngineIDIntMessage::getEngineID() const
    {
        return (m_EngineID);
    }

}