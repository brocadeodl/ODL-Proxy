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
 *   Author : sbusch                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgPortCfgPortTranslateMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{
    // ************* PortCfgPortCfgPortTranslateMessage ************

    PortCfgPortCfgPortTranslateMessage::PortCfgPortCfgPortTranslateMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName ()
									  ,PORTCFGPORTCFGPORTTRANSLATE)
    {
		m_switchPort = -1;
		m_rbridge = -1;
		m_id = "";
		m_type = "";
		m_phyPortId = "";
		m_phyPortType = "";
    }

    PortCfgPortCfgPortTranslateMessage::PortCfgPortCfgPortTranslateMessage (const string &id,
																			const SI32 &switchPort, const SI32 &rbridge)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName ()
									  ,PORTCFGPORTCFGPORTTRANSLATE),
        m_id    (id),
        m_switchPort    (switchPort),
		m_rbridge    (rbridge)
    {
		m_type = "";
		m_phyPortId = "";
		m_phyPortType = "";
    }

    PortCfgPortCfgPortTranslateMessage::PortCfgPortCfgPortTranslateMessage (const SI32 &switchPort, const SI32 &rbridge)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName ()
									  ,PORTCFGPORTCFGPORTTRANSLATE),
        m_switchPort    (switchPort),
		m_rbridge    (rbridge)
    {
		m_id = "";
		m_type = "";
		m_phyPortId = "";
		m_phyPortType = "";
    }

    PortCfgPortCfgPortTranslateMessage::PortCfgPortCfgPortTranslateMessage (const string &id,const string &type)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGPORTTRANSLATE),
        m_id    (id),
        m_type    (type)
    {
		m_switchPort = -1;
		m_rbridge = -1;
		m_phyPortId = "";
		m_phyPortType = "";
    }

    PortCfgPortCfgPortTranslateMessage::PortCfgPortCfgPortTranslateMessage (const string &id,const string &type,const string &phyPortId,const string &phyPortType,const SI32 &switchPort,const SI32 &rbridge)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGPORTCFGPORTTRANSLATE),
        m_id    (id),
        m_type    (type),
        m_phyPortId    (phyPortId),
        m_phyPortType    (phyPortType),
        m_switchPort    (switchPort),
        m_rbridge    (rbridge)
    {
    }

    PortCfgPortCfgPortTranslateMessage::~PortCfgPortCfgPortTranslateMessage ()
    {
    }

    void  PortCfgPortCfgPortTranslateMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_type,"type"));
        addSerializableAttribute (new AttributeString(&m_phyPortId,"phyPortId"));
        addSerializableAttribute (new AttributeString(&m_phyPortType,"phyPortType"));
        addSerializableAttribute (new AttributeSI32(&m_switchPort,"switchPort"));
		addSerializableAttribute (new AttributeSI32(&m_rbridge,"rbridge"));
    }

    void  PortCfgPortCfgPortTranslateMessage::setId(const string &id)
    {
        m_id = id;
    }

    string  PortCfgPortCfgPortTranslateMessage::getId() const
    {
        return (m_id);
	}

    void  PortCfgPortCfgPortTranslateMessage::setType(const string &type)
    {
        m_type  =  type;
    }

    string  PortCfgPortCfgPortTranslateMessage::getType() const
    {
        return (m_type);
    }

    void  PortCfgPortCfgPortTranslateMessage::setPhyPortId(const string &phyPortId)
    {
        m_phyPortId  =  phyPortId;
    }

    string  PortCfgPortCfgPortTranslateMessage::getPhyPortId() const
    {
        return (m_phyPortId);
    }

    void  PortCfgPortCfgPortTranslateMessage::setPhyPortType(const string &phyPortType)
    {
        m_phyPortType  =  phyPortType;
    }

    string  PortCfgPortCfgPortTranslateMessage::getPhyPortType() const
    {
        return (m_phyPortType);
    }

    void  PortCfgPortCfgPortTranslateMessage::setSwitchPort(const SI32 &switchPort)
    {
        m_switchPort = switchPort;
    }

    SI32  PortCfgPortCfgPortTranslateMessage::getSwitchPort() const
    {
        return (m_switchPort);
    }

	void  PortCfgPortCfgPortTranslateMessage::setRbridge(const SI32 &rbridge)
    {
        m_rbridge = rbridge;
    }

    SI32  PortCfgPortCfgPortTranslateMessage::getRbridge() const
    {
        return (m_rbridge);
    }

	void  PortCfgPortCfgPortTranslateMessage::reset()
	{
		m_rbridge = -1;
		m_switchPort = -1;
		m_id = "";
		m_type = "";
		m_phyPortId = "";
		m_phyPortType = "";
	}

    bool  PortCfgPortCfgPortTranslateMessage::isDISet() const
    {
        return (m_switchPort != -1 && m_rbridge != -1);
    }

    bool  PortCfgPortCfgPortTranslateMessage::isIdSet() const
    {
        return (m_id.length() != 0 && m_type.length() != 0);
    }

    bool  PortCfgPortCfgPortTranslateMessage::isPhyPortSet() const
    {
        return (m_id.length() != 0 && m_type.length() != 0);
    }
}
