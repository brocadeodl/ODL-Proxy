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
 *   Author : agidwani                                                     *
 **************************************************************************/

#include "Nsm/Local/Igmp/IgmpSgIntfConfManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

IgmpSgIntfConfManagedObject::IgmpSgIntfConfManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
    : PrismElement  (pNsmLocalObjectManager),
    PrismPersistableObject (IgmpSgIntfConfManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pNsmLocalObjectManager),
    DcmManagedObject (pNsmLocalObjectManager),
    WaveLocalManagedObjectBase(this),
    DcmLocalManagedObjectBase(this),
    DcmLocalManagedObject(pNsmLocalObjectManager)
{

}

IgmpSgIntfConfManagedObject::IgmpSgIntfConfManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const IpV4Address &groupAddress,const UI32 &interface,const UI32 &ifType,const string &ifName)
    : PrismElement  (pNsmLocalObjectManager),
    PrismPersistableObject (IgmpSgIntfConfManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pNsmLocalObjectManager),
    DcmManagedObject (pNsmLocalObjectManager),
    WaveLocalManagedObjectBase(this),
    DcmLocalManagedObjectBase(this),
    DcmLocalManagedObject(pNsmLocalObjectManager),
    m_groupAddress    (groupAddress),
    m_interface    (interface),
    m_ifType    (ifType),
    m_ifName    (ifName)
{
}

IgmpSgIntfConfManagedObject::~IgmpSgIntfConfManagedObject ()
{
}

string  IgmpSgIntfConfManagedObject::getClassName()
{
    return ("IgmpSgIntfConfManagedObject");
}

void  IgmpSgIntfConfManagedObject::setupAttributesForPersistence()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();
    addPersistableAttribute (new AttributeIpV4Address(&m_groupAddress,"groupAddress"));
    addPersistableAttribute (new AttributeUI32(&m_interface,"interface"));
    addPersistableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    addPersistableAttribute (new AttributeString(&m_ifName,"ifName"));
    
    setUserTagForAttribute("groupAddress", igmp_sg_addr);
}

void  IgmpSgIntfConfManagedObject::setupAttributesForCreate()
{
    DcmLocalManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_groupAddress,"groupAddress"));
    addPersistableAttributeForCreate  (new AttributeUI32(&m_interface,"interface"));
    addPersistableAttributeForCreate  (new AttributeUI32(&m_ifType,"ifType"));
    addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName"));
}

void IgmpSgIntfConfManagedObject::setupKeys()
{
    vector<string> keyName;
    keyName.push_back ("groupAddress");
    setUserDefinedKeyCombination (keyName);
}

void  IgmpSgIntfConfManagedObject::setGroupAddress(const IpV4Address &groupAddress)
{
    m_groupAddress  =  groupAddress;
}

IpV4Address  IgmpSgIntfConfManagedObject::getGroupAddress() const
{
    return (m_groupAddress);
}

void  IgmpSgIntfConfManagedObject::setInterface(const UI32 &interface)
{
    m_interface  =  interface;
}

UI32  IgmpSgIntfConfManagedObject::getInterface() const
{
    return (m_interface);
}

void  IgmpSgIntfConfManagedObject::setIfType(const UI32 &ifType)
{
    m_ifType  =  ifType;
}

UI32  IgmpSgIntfConfManagedObject::getIfType() const
{
    return (m_ifType);
}

void  IgmpSgIntfConfManagedObject::setIfName(const string &ifName)
{
    m_ifName  =  ifName;
}

string  IgmpSgIntfConfManagedObject::getIfName() const
{
    return (m_ifName);
}

}
