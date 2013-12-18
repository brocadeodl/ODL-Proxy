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
 *   Author : Brunda                                                      *
 **************************************************************************/

#include "SnmpServer/Local/SnmpServerIntLocalObjectManager.h"
#include "SnmpServer/Local/SnmpServerIntSnmpConfigRemoveContextMappingIntMessage.h"
#include "SnmpServer/Local/SnmpServerIntTypes.h"

namespace DcmNs
{

    SnmpServerIntSnmpConfigRemoveContextMappingIntMessage::SnmpServerIntSnmpConfigRemoveContextMappingIntMessage ()
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGREMOVECONTEXTMAPPINGINT)
    {
    }

    SnmpServerIntSnmpConfigRemoveContextMappingIntMessage::SnmpServerIntSnmpConfigRemoveContextMappingIntMessage (const string &contextName,const string &delFieldName)
        : PrismMessage (SnmpServerIntLocalObjectManager::getPrismServiceId (), SNMPSERVERINTSNMPCONFIGREMOVECONTEXTMAPPINGINT),
        m_contextName    (contextName),
        m_delFieldName(delFieldName)
    {
    }

    SnmpServerIntSnmpConfigRemoveContextMappingIntMessage::~SnmpServerIntSnmpConfigRemoveContextMappingIntMessage ()
    {
    }

    void  SnmpServerIntSnmpConfigRemoveContextMappingIntMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_contextName,"contextName"));
        addSerializableAttribute (new AttributeString(&m_delFieldName,"delFieldName"));
    }

    void SnmpServerIntSnmpConfigRemoveContextMappingIntMessage:: setContextName(const string &contextName)
    {
        m_contextName = contextName;
    }

    string SnmpServerIntSnmpConfigRemoveContextMappingIntMessage:: getContextName()  const
    {
        return (m_contextName);

    }

    void SnmpServerIntSnmpConfigRemoveContextMappingIntMessage::setDelFieldName(const string &delFieldName)
    {
         m_delFieldName = delFieldName;

    }

    string SnmpServerIntSnmpConfigRemoveContextMappingIntMessage::getDelFieldName()  const
    {
         return(m_delFieldName);
    }
}
