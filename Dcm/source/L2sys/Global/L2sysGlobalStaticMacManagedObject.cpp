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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "L2sys/Global/L2sysGlobalStaticMacManagedObject.h"
#include "L2sys/Global/L2sysGlobalObjectManager.h"
//#include "L2sys/Global/L2sysGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    L2sysGlobalStaticMacManagedObject::L2sysGlobalStaticMacManagedObject (L2sysGlobalObjectManager *pL2sysGlobalObjectManager)
        : PrismElement  (pL2sysGlobalObjectManager),
        PrismPersistableObject (L2sysGlobalStaticMacManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pL2sysGlobalObjectManager),
        DcmManagedObject (pL2sysGlobalObjectManager),
        m_forward    (0),
        m_interface_type    (0),
        m_vlan    (0),
        m_vlanid    (0),
        m_slotid    (0)
    {
    }

    L2sysGlobalStaticMacManagedObject::L2sysGlobalStaticMacManagedObject (L2sysGlobalObjectManager *pL2sysGlobalObjectManager,const string &Mac,const UI32 &forward,const UI32 &interface_type,const string &ifname,const UI32 &vlan,const UI32 &vlanid,const UI32 &slotid)
        : PrismElement  (pL2sysGlobalObjectManager),
        PrismPersistableObject (L2sysGlobalStaticMacManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pL2sysGlobalObjectManager),
        DcmManagedObject (pL2sysGlobalObjectManager),
        m_Mac    (Mac),
        m_forward    (forward),
        m_interface_type    (interface_type),
        m_ifname    (ifname),
        m_vlan    (vlan),
        m_vlanid    (vlanid),
        m_slotid    (slotid)
    {
    }

    L2sysGlobalStaticMacManagedObject::~L2sysGlobalStaticMacManagedObject ()
    {
    }

    string  L2sysGlobalStaticMacManagedObject::getClassName()
    {
        return ("L2sysGlobalStaticMacManagedObject");
    }

    void  L2sysGlobalStaticMacManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_Mac,"Mac"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_forward,"forward"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_interface_type,"interface_type"));
        addPersistableAttribute (new AttributeString(&m_ifname,"ifname"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_vlan,"vlan"));
        addPersistableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
        addPersistableAttribute (new AttributeUI32(&m_slotid,"slotid"));
		

		setUserTagForAttribute("Mac", mac_address_table_mac_address);
		setUserTagForAttribute("forward", mac_address_table_forward);
		setUserTagForAttribute("interface_type", mac_address_table_interface_type);
		setUserTagForAttribute("ifname", mac_address_table_interface_name);
		setUserTagForAttribute("vlan", mac_address_table_vlan);
		setUserTagForAttribute("vlanid", mac_address_table_vlanid);

    }

    void  L2sysGlobalStaticMacManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_Mac,"Mac"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_forward,"forward"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_interface_type,"interface_type"));
        addPersistableAttributeForCreate  (new AttributeString(&m_ifname,"ifname"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_vlan,"vlan"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_vlanid,"vlanid"));
		addPersistableAttributeForCreate  (new AttributeUI32(&m_slotid,"slotid"));

        vector<string > keyName;
        keyName.push_back ("Mac");
        setUserDefinedKeyCombination (keyName);
        keyName.push_back ("forward");
        setUserDefinedKeyCombination (keyName);
        keyName.push_back ("interface_type");
        setUserDefinedKeyCombination (keyName);
        keyName.push_back ("ifname");
        setUserDefinedKeyCombination (keyName);
        keyName.push_back ("vlan");
        setUserDefinedKeyCombination (keyName);
        keyName.push_back ("vlanid");
        setUserDefinedKeyCombination (keyName);
    }

    void  L2sysGlobalStaticMacManagedObject::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysGlobalStaticMacManagedObject::getMac() const
    {
        return (m_Mac);
    }

    void  L2sysGlobalStaticMacManagedObject::setForward(const UI32 &forward)
    {
        m_forward  =  forward;
    }

    UI32  L2sysGlobalStaticMacManagedObject::getForward() const
    {
        return (m_forward);
    }

    void  L2sysGlobalStaticMacManagedObject::setInterface_type(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysGlobalStaticMacManagedObject::getInterface_type() const
    {
        return (m_interface_type);
    }

    void  L2sysGlobalStaticMacManagedObject::setIfname(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  L2sysGlobalStaticMacManagedObject::getIfname() const
    {
        return (m_ifname);
    }

    void  L2sysGlobalStaticMacManagedObject::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  L2sysGlobalStaticMacManagedObject::getVlan() const
    {
        return (m_vlan);
    }

    void  L2sysGlobalStaticMacManagedObject::setVlanid(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  L2sysGlobalStaticMacManagedObject::getVlanid() const
    {
        return (m_vlanid);
    }
	
	void  L2sysGlobalStaticMacManagedObject::setSlotid(const UI32 &slotid)
    {
        m_slotid  =  slotid;
    }

    UI32  L2sysGlobalStaticMacManagedObject::getSlotid() const
    {
        return (m_slotid);
    }


}
