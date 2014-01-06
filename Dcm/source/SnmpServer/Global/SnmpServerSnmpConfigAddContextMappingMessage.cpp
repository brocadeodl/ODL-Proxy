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

/******************************************************************************* 
*copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : brunda                                                     *
 **************************************************************************/

#include "SnmpServer/Global/SnmpServerSnmpConfigAddContextMappingMessage.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    SnmpServerSnmpConfigAddContextMappingMessage::SnmpServerSnmpConfigAddContextMappingMessage ()
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGADDCONTEXTMAPPING)
    {
    }

    SnmpServerSnmpConfigAddContextMappingMessage::SnmpServerSnmpConfigAddContextMappingMessage (const string &contextName,const string &vrfName)
        : PrismMessage (SnmpServerObjectManager::getPrismServiceId (), SNMPSERVERSNMPCONFIGADDCONTEXTMAPPING),
        m_contextName    (contextName),
        m_vrfName    (vrfName)
    {
    }

    SnmpServerSnmpConfigAddContextMappingMessage::~SnmpServerSnmpConfigAddContextMappingMessage ()
    {
    }

    void  SnmpServerSnmpConfigAddContextMappingMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_contextName,"contextName"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
    }

    void SnmpServerSnmpConfigAddContextMappingMessage:: setContextName(const string &contextName)
    {
        m_contextName = contextName;
    }
 
    string SnmpServerSnmpConfigAddContextMappingMessage:: getContextName()  const
    {
        return (m_contextName);
    }

    void SnmpServerSnmpConfigAddContextMappingMessage::setVrfName(const string &vrfName)
    {
         m_vrfName = vrfName;
    }

    string SnmpServerSnmpConfigAddContextMappingMessage::getVrfName()  const
    {
        return(m_vrfName);
    }

}
