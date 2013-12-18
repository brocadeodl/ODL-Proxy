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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brunda                                                     *
 **************************************************************************/

#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigSetContextMappingIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigSetContextMappingIntMessage::SnmpServerIntSnmpConfigSetContextMappingIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGSETCONTEXTMAPPINGINT)
    {
    }

    SnmpServerIntSnmpConfigSetContextMappingIntMessage::SnmpServerIntSnmpConfigSetContextMappingIntMessage (const string &contextName,const string &vrfName)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGSETCONTEXTMAPPINGINT),
           m_contextName    (contextName),
        m_vrfName    (vrfName)
    {
    }

    SnmpServerIntSnmpConfigSetContextMappingIntMessage::~SnmpServerIntSnmpConfigSetContextMappingIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigSetContextMappingIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_contextName,"contextName"));
        addSerializableAttribute (new AttributeString(&m_vrfName,"vrfName"));
    }
    void  SnmpServerIntSnmpConfigSetContextMappingIntMessage:: setContextName(const string &contextName)
    {
         m_contextName = contextName;
    }

    string  SnmpServerIntSnmpConfigSetContextMappingIntMessage:: getContextName()  const
    {
         return (m_contextName);

    }

    void  SnmpServerIntSnmpConfigSetContextMappingIntMessage::setVrfName(const string &vrfName)
    {
         m_vrfName = vrfName;

    }

    string  SnmpServerIntSnmpConfigSetContextMappingIntMessage::getVrfName()  const
    {
         return(m_vrfName);
    }

}
