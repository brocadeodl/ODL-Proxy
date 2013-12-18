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

#include "SnmpServer/Global/SystemGroupManagedObject.h"
#include "SnmpServer/Global/SnmpServerObjectManager.h"
#include "SnmpServer/Global/SnmpServerTypes.h"
//#include "snmp.h"
#include "vcs.h"
namespace DcmNs
{

    SystemGroupManagedObject::SystemGroupManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (SystemGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager)
    {
    }

    SystemGroupManagedObject::SystemGroupManagedObject (SnmpServerObjectManager *pSnmpServerObjectManager,const string &contact,const string &location,const string &sysDescr)
        : PrismElement  (pSnmpServerObjectManager),
        PrismPersistableObject (SystemGroupManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pSnmpServerObjectManager),
        DcmManagedObject (pSnmpServerObjectManager),
        m_contact    (contact),
        m_location    (location),
        m_sysDescr    (sysDescr)
    {
    }

    SystemGroupManagedObject::~SystemGroupManagedObject ()
    {
    }

    string  SystemGroupManagedObject::getClassName()
    {
        return ("SystemGroupManagedObject");
    }

    void  SystemGroupManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_contact,"contact", snmp_server_contact));
        addPersistableAttribute (new AttributeString(&m_location,"location", snmp_server_location));
        addPersistableAttribute (new AttributeString(&m_sysDescr,"sysDescr", snmp_server_sys_descr));
    }

    void  SystemGroupManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_contact,"contact", snmp_server_contact));
        addPersistableAttributeForCreate  (new AttributeString(&m_location,"location", snmp_server_location));
        addPersistableAttributeForCreate  (new AttributeString(&m_sysDescr,"sysDescr",snmp_server_sys_descr));
    }

    void  SystemGroupManagedObject::setContact(const string &contact)
    {
        m_contact  =  contact;
    }

    string  SystemGroupManagedObject::getContact() const
    {
        return (m_contact);
    }

    void  SystemGroupManagedObject::setLocation(const string &location)
    {
        m_location  =  location;
    }

    string  SystemGroupManagedObject::getLocation() const
    {
        return (m_location);
    }

    void  SystemGroupManagedObject::setSysDescr(const string &sysDescr)
    {
        m_sysDescr  =  sysDescr;
    }

    string  SystemGroupManagedObject::getSysDescr() const
    {
        return (m_sysDescr);
    }
}
