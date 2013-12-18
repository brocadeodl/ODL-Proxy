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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                                       *
 **************************************************************************/
#include "vcs.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Nsm/Local/Dhcp/DhcpRelInterfaceLocalManagedObject.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Dhcp/DhcpRelIntfClientConfigMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

    DhcpRelInterfaceLocalManagedObject::DhcpRelInterfaceLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (DhcpRelInterfaceLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
		m_serverAddress ("0.0.0.0"),
        m_ifName  (""),
        m_ifType  (IF_TYPE_INVALID),
        m_vlanId  (0)
	{
    }

    DhcpRelInterfaceLocalManagedObject::~DhcpRelInterfaceLocalManagedObject ()
    {
    }

    string  DhcpRelInterfaceLocalManagedObject::getClassName()
    {
        return ("DhcpRelInterfaceLocalManagedObject");
    }

    void  DhcpRelInterfaceLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
 		addPersistableAttribute (new AttributeIpV4Address(&m_serverAddress,"serverAddress"));
    	addPersistableAttribute (new AttributeString(&m_ifName,"ifName"));
    	addPersistableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    	addPersistableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));

    	setUserTagForAttribute("serverAddress", dhcp_relay_ip_addr);
    }

    void  DhcpRelInterfaceLocalManagedObject::setupAttributesForCreate()
    {
		DcmLocalManagedObject::setupAttributesForCreate ();
    	addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_serverAddress,"serverAddress"));
    	addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName"));
    	addPersistableAttributeForCreate  (new AttributeUI32(&m_ifType,"ifType"));
    	addPersistableAttributeForCreate  (new AttributeUI32(&m_vlanId,"vlanId"));

		DhcpRelInterfaceLocalManagedObject::setupKeys();
    }

	void DhcpRelInterfaceLocalManagedObject::setupKeys()
	{
    	vector<string> keyName;
    	keyName.push_back ("serverAddress");
    	setUserDefinedKeyCombination (keyName);
	}

	void  DhcpRelInterfaceLocalManagedObject::setServerAddress(const IpV4Address &serverAddress)
	{
    	m_serverAddress  =  serverAddress;
	}

	IpV4Address  DhcpRelInterfaceLocalManagedObject::getServerAddress() const
	{
    	return (m_serverAddress);
	}

	void  DhcpRelInterfaceLocalManagedObject::setIfType(const UI32 &ifType)
	{
    	m_ifType  =  ifType;
	}

	UI32  DhcpRelInterfaceLocalManagedObject::getIfType() const
	{
    	return (m_ifType);
	}

	void  DhcpRelInterfaceLocalManagedObject::setIfName(const string &ifName)
	{
    	m_ifName  =  ifName;
	}

	string  DhcpRelInterfaceLocalManagedObject::getIfName() const
	{
    	return (m_ifName);
	}

	void  DhcpRelInterfaceLocalManagedObject::setVlanId(const UI32 &vlanId)
	{
    	m_vlanId  =  vlanId;
	}

	UI32  DhcpRelInterfaceLocalManagedObject::getVlanId() const
	{
    	return (m_vlanId);
	}


}
