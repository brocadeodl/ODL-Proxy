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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Arun kumar viswanathan                                       *
 ***************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"

#include "Ipadm/Global/IpadmGlobalManagedObject.h"
#include "Ipadm/Local/IpadmLocalManagedObject.h"
#include "Ipadm/Local/IpadmTypes.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"
//#include "ipadm.h"
#include "vcs.h"

namespace DcmNs
{
/*
 * Domain name managed object
 */
IpadmGlobalDNManagedObject::IpadmGlobalDNManagedObject (IpadmGlobalObjectManager *pIpadmGlobalObjectManager)
    : PrismElement           (pIpadmGlobalObjectManager),
      PrismPersistableObject (IpadmGlobalDNManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pIpadmGlobalObjectManager),
      DcmManagedObject       (pIpadmGlobalObjectManager)
{
}

IpadmGlobalDNManagedObject::~IpadmGlobalDNManagedObject ()
{
}

string IpadmGlobalDNManagedObject::getClassName ()
{
    return ("IpadmGlobalDNManagedObject");
}

void IpadmGlobalDNManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
    addPersistableAttribute (new AttributeString(&m_domain_name,"domain_name", ip_administration_domain_name));
}

void IpadmGlobalDNManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate (new AttributeString(&m_domain_name,"domain_name", ip_administration_domain_name));
}

string IpadmGlobalDNManagedObject::getDomainName () const
{
    return (m_domain_name);
}

void IpadmGlobalDNManagedObject::setDomainName (const string &domain_name)
{
    m_domain_name = domain_name;
}

// Name Server Ip MO
IpadmGlobalNSManagedObject::IpadmGlobalNSManagedObject (IpadmGlobalObjectManager *pIpadmGlobalObjectManager)
    : PrismElement           (pIpadmGlobalObjectManager),
      PrismPersistableObject (IpadmGlobalNSManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject      (pIpadmGlobalObjectManager),
      DcmManagedObject       (pIpadmGlobalObjectManager)
{   
}
IpadmGlobalNSManagedObject::~IpadmGlobalNSManagedObject ()
{
}

string IpadmGlobalNSManagedObject::getClassName ()
{   
    return ("IpadmGlobalNSManagedObject");
}     
      
void IpadmGlobalNSManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
    addPersistableAttribute (new AttributeUI32(&m_index,"index"));
    addPersistableAttribute (new AttributeIpVxAddress(&m_nameSrvIp,"ipaddress", ip_administration_name_server_ip));
}     
      
void IpadmGlobalNSManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate  (new AttributeUI32(&m_index,"index"));
    addPersistableAttributeForCreate (new AttributeIpVxAddress(&m_nameSrvIp,"ipaddress", ip_administration_name_server_ip));
}

void IpadmGlobalNSManagedObject::setupKeys()
{
        vector<string > keyName;
        keyName.push_back ("ipaddress");
        setUserDefinedKeyCombination (keyName);
}

void  IpadmGlobalNSManagedObject::setIndex(const UI32 &index)
{
        m_index  =  index;
}       

UI32  IpadmGlobalNSManagedObject::getIndex() const
{
        return (m_index);
}

IpVxAddress IpadmGlobalNSManagedObject::getNameSrvIp () const
{
    return (m_nameSrvIp);
}

void IpadmGlobalNSManagedObject::setNameSrvIp (const IpVxAddress &nameSrvIp)
{
    m_nameSrvIp = nameSrvIp;
}
}
