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
 *   Author : dchung                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgPortCfgCreateMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgPortCfgCreateMessage::PortCfgPortCfgCreateMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGCREATE),
		m_switch_port	 (-1),
		m_portType      ("TE"),
		m_switchPortType      ("TE"),
		m_dispId		("NONE"),
		m_slot			(-1),
		m_phyPort		(-1),
		m_fcoePort		(-1),
		m_vlan			(-1),
		m_ifIndex		(-1)
    {
    }

    PortCfgPortCfgCreateMessage::PortCfgPortCfgCreateMessage (const string &id,const bool &isIdCompete)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGCREATE),
        m_id    (id),
        m_isIdCompete    (isIdCompete),
		m_switch_port	 (-1),
		m_portType      ("TE"),
		m_switchPortType      ("TE"),
		m_dispId		("NONE"),
		m_slot			(-1),
		m_phyPort		(-1),
		m_fcoePort		(-1),
		m_vlan			(-1),
		m_ifIndex		(-1)
    {
	}

	PortCfgPortCfgCreateMessage::PortCfgPortCfgCreateMessage (const string &id,const bool &isIdCompete, const int &switch_port, const string &switchPortType, const int &ifIndex)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGCREATE),
        m_id    (id),
        m_isIdCompete    (isIdCompete),
		m_switch_port	 (switch_port),
		m_portType      ("TE"),
		m_switchPortType      (switchPortType),
		m_dispId		("NONE"),
		m_slot			(-1),
		m_phyPort		(-1),
		m_fcoePort		(-1),
		m_vlan			(-1),
		m_ifIndex		(ifIndex)

    {
    }

	 PortCfgPortCfgCreateMessage::PortCfgPortCfgCreateMessage (const string &id,const bool &isIdCompete, const int &switch_port, const string &portType, const string &switchPortType, const int &slot, const int &phyPort, const int &fcoePort, const int &vlan, const int &ifIndex)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGCREATE),
        m_id    (id),
        m_isIdCompete    (isIdCompete),
		m_switch_port	 (switch_port),
	 	m_portType      (portType),
		m_switchPortType      (switchPortType),
		m_dispId		("NONE"),
		m_slot			(slot),
		m_phyPort		(phyPort),
		m_fcoePort		(fcoePort),
		m_vlan			(vlan),
		m_ifIndex		(ifIndex)
    {
    }
	

    PortCfgPortCfgCreateMessage::~PortCfgPortCfgCreateMessage ()
    {
    }

    void  PortCfgPortCfgCreateMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeBool(&m_isIdCompete,"isIdCompete"));
        addSerializableAttribute (new AttributeSI32(&m_switch_port,"switchport"));
		addSerializableAttribute (new AttributeString(&m_portType,"portType"));
		addSerializableAttribute (new AttributeString(&m_switchPortType,"switchPortType"));
		addSerializableAttribute (new AttributeString(&m_dispId,"dispId"));
		addSerializableAttribute (new AttributeSI32(&m_slot,"slot"));
		addSerializableAttribute (new AttributeSI32(&m_phyPort,"phyPort"));
		addSerializableAttribute (new AttributeSI32(&m_fcoePort,"fcoePort"));
		addSerializableAttribute (new AttributeSI32(&m_vlan,"vlan"));
		addSerializableAttribute (new AttributeSI32(&m_ifIndex,"ifIndex"));
    }

    void  PortCfgPortCfgCreateMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  PortCfgPortCfgCreateMessage::getId() const
    {
        return (m_id);
    }

    void  PortCfgPortCfgCreateMessage::setIsIdCompete(const bool &isIdCompete)
    {
        m_isIdCompete  =  isIdCompete;
    }

    bool  PortCfgPortCfgCreateMessage::getIsIdCompete() const
    {
        return (m_isIdCompete);
    }
	void  PortCfgPortCfgCreateMessage::setPortType(const string &portType)
	{
		 m_portType  =  portType;
	}

	string  PortCfgPortCfgCreateMessage::getPortType() const
	{
		return (m_portType);
	}

	void  PortCfgPortCfgCreateMessage::setSwitchPortType(const string &switchPortType)
	{
		 m_switchPortType  =  switchPortType;
	}

	string  PortCfgPortCfgCreateMessage::getSwitchPortType() const
	{
		return (m_switchPortType);
	}

	void  PortCfgPortCfgCreateMessage::setDispId(const string &dispId)
	{
		 m_dispId  =  dispId;
	}

	string  PortCfgPortCfgCreateMessage::getDispId() const
	{
		return (m_dispId);
	}

	void  PortCfgPortCfgCreateMessage::setSlot(const int &slot)
	{
		m_slot = slot;
	}

	int  PortCfgPortCfgCreateMessage::getSlot() const
	{
		return (m_slot);
	}

	void  PortCfgPortCfgCreateMessage::setPhyPort(const int &phyPort)
	{
		m_phyPort = phyPort;
	}

	int  PortCfgPortCfgCreateMessage::getPhyPort() const
	{
		return (m_phyPort);
	}

	void  PortCfgPortCfgCreateMessage::setFcoePort(const int &fcoePort)
	{
		m_fcoePort = fcoePort;
	}

	int  PortCfgPortCfgCreateMessage::getFcoePort() const
	{
		return (m_fcoePort);
	}

	void  PortCfgPortCfgCreateMessage::setVlan(const int &vlan)
	{
		m_vlan = vlan;
	}

	int  PortCfgPortCfgCreateMessage::getVlan() const
	{
		return (m_vlan);
	}
		void  PortCfgPortCfgCreateMessage::setIfIndex(const int &ifIndex)
	{
		m_ifIndex = ifIndex;
	}

	int  PortCfgPortCfgCreateMessage::getIfIndex() const
	{
		return (m_ifIndex);
    }

}
