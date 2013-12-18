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
 *   Author : Brunda                                                       *
 **************************************************************************/

#include "SnmpServer/Global/ContextMappingManagedObject.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "SnmpServer/Global/enumdatatype.h"
#include "vcs.h"
namespace DcmNs
{

    ContextMappingManagedObject::ContextMappingManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (ContextMappingManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager)
    {
    }

    ContextMappingManagedObject::ContextMappingManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const string &contextName,const string &vrfName)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (ContextMappingManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager),
        m_contextName    (contextName),
        m_vrfName    (vrfName)
    {
    }

    ContextMappingManagedObject::~ContextMappingManagedObject ()
    {
    }

    string  ContextMappingManagedObject::getClassName()
    {
        return ("ContextMappingManagedObject");
    }

    void  ContextMappingManagedObject::setupAttributesForPersistence()
    {
		DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_contextName,"contextName"));
        addPersistableAttribute (new AttributeString(&m_vrfName,"vrfName",snmp_server_vrf_name));
        setUserTagForAttribute ("contextName", snmp_server_context_name);
    }

    void  ContextMappingManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_contextName,"contextName"));
        addPersistableAttributeForCreate  (new AttributeString(&m_vrfName,"vrfName", snmp_server_vrf_name));
        vector<string > keyList;
        keyList.push_back ("contextName");
        setUserDefinedKeyCombination (keyList);
    }

    void ContextMappingManagedObject:: setContextName(const string &contextName)
    {
         m_contextName = contextName;
    }

    string ContextMappingManagedObject:: getContextName()  const
    {
         return (m_contextName);
    }

    void ContextMappingManagedObject::setVrfName(const string &vrfName)
    {
         m_vrfName = vrfName;
    }

    string ContextMappingManagedObject::getVrfName()  const
    {
         return(m_vrfName);
    }

}
