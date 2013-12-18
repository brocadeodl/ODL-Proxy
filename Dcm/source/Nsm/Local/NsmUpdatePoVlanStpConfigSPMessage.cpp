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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdatePoVlanStpConfigSPMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Global/NsmGlobalUpdatePoVlanStpConfigMessage.h"

namespace DcmNs
{

    NsmUpdatePoVlanStpConfigSPMessage::NsmUpdatePoVlanStpConfigSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMUPDATEPOVLANSTPCONFIGSP)
    {
		m_ifName = "";
		m_priority = 0;
		m_cost = 0;
		m_cmdCode = 0;
		m_vid = 0;
    }

    NsmUpdatePoVlanStpConfigSPMessage::NsmUpdatePoVlanStpConfigSPMessage(NsmGlobalUpdatePoVlanStpConfigMessage &message)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMUPDATEPOVLANSTPCONFIGSP)
	{
		m_ifName = message.m_ifName;
		m_vid = message.m_vid;
		m_priority = message.m_priority;
		m_cost = message.m_cost;
		m_cmdCode = message.m_cmdCode;
	}

    NsmUpdatePoVlanStpConfigSPMessage::NsmUpdatePoVlanStpConfigSPMessage (const string &ifName,const UI32 &vid,const UI16 &priority,const UI32 &cost,const UI32 &cmdCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMUPDATEPOVLANSTPCONFIGSP),
        m_ifName    (ifName),
        m_vid    (vid),
        m_priority    (priority),
        m_cost    (cost),
        m_cmdCode    (cmdCode)
    {
    }

    NsmUpdatePoVlanStpConfigSPMessage::~NsmUpdatePoVlanStpConfigSPMessage ()
    {
    }

    void  NsmUpdatePoVlanStpConfigSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_vid,"vid"));
        addSerializableAttribute (new AttributeUI16(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_cost,"cost"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmUpdatePoVlanStpConfigSPMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmUpdatePoVlanStpConfigSPMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmUpdatePoVlanStpConfigSPMessage::setVid(const UI32 &vid)
    {
        m_vid  =  vid;
    }

    UI32  NsmUpdatePoVlanStpConfigSPMessage::getVid() const
    {
        return (m_vid);
    }

    void  NsmUpdatePoVlanStpConfigSPMessage::setPriority(const UI16 &priority)
    {
        m_priority  =  priority;
    }

    UI16  NsmUpdatePoVlanStpConfigSPMessage::getPriority() const
    {
        return (m_priority);
    }

    void  NsmUpdatePoVlanStpConfigSPMessage::setCost(const UI32 &cost)
    {
        m_cost  =  cost;
    }

    UI32  NsmUpdatePoVlanStpConfigSPMessage::getCost() const
    {
        return (m_cost);
    }

    void  NsmUpdatePoVlanStpConfigSPMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmUpdatePoVlanStpConfigSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
