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

#include "ClientInterface/Mstp/MstpVlanBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "Mstp/Local/MstpUpdateVlanBridgeConfigMessage.h"

namespace DcmNs
{

    MstpVlanBridgeConfigMessage::MstpVlanBridgeConfigMessage ()
        : DcmManagementInterfaceMessage (MSTPVLANBRIDGECONFIG)
    {
		m_id = 0;
		m_priority = 0;
		m_forwardDelay = 0;	
		m_helloTime = 0;
		m_maxAge = 0;
		m_agingTime = 0;
		m_cmdCode = 0;
		m_mode = 0;
    }

    MstpVlanBridgeConfigMessage::MstpVlanBridgeConfigMessage(MstpUpdateVlanBridgeConfigMessage & msg)
        : DcmManagementInterfaceMessage (MSTPVLANBRIDGECONFIG)
    {
		m_id = msg.m_id;
		m_priority = msg.m_priority;
		m_forwardDelay = msg.m_forwardDelay;	
		m_helloTime = msg.m_helloTime;
		m_maxAge = msg.m_maxAge;
		m_agingTime = msg.m_agingTime;
		m_cmdCode = msg.m_cmdCode;
		m_mode = msg.m_mode;
    }

    MstpVlanBridgeConfigMessage::MstpVlanBridgeConfigMessage (const UI32 &id,const UI32 &priority,const UI32 &forwardDelay,const UI32 &helloTime,const UI32 &maxAge, const UI32 &agingTime,const UI32 &mode, const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (MSTPVLANBRIDGECONFIG),
        m_id    (id),
        m_priority    (priority),
        m_forwardDelay    (forwardDelay),
        m_helloTime    (helloTime),
        m_maxAge    (maxAge),
        m_agingTime    (agingTime),
        m_mode    (mode),
        m_cmdCode    (cmdCode)
    {
    }

    MstpVlanBridgeConfigMessage::~MstpVlanBridgeConfigMessage ()
    {
    }

    void  MstpVlanBridgeConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_forwardDelay,"forwardDelay"));
        addSerializableAttribute (new AttributeUI32(&m_helloTime,"helloTime"));
        addSerializableAttribute (new AttributeUI32(&m_maxAge,"maxAge"));
        addSerializableAttribute (new AttributeUI32(&m_agingTime,"agingTime"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  MstpVlanBridgeConfigMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  MstpVlanBridgeConfigMessage::getId() const
    {
        return (m_id);
    }

    void  MstpVlanBridgeConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpVlanBridgeConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpVlanBridgeConfigMessage::setForwardDelay(const UI32 &forwardDelay)
    {
        m_forwardDelay  =  forwardDelay;
    }

    UI32  MstpVlanBridgeConfigMessage::getForwardDelay() const
    {
        return (m_forwardDelay);
    }

    void  MstpVlanBridgeConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  MstpVlanBridgeConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  MstpVlanBridgeConfigMessage::setMaxAge(const UI32 &maxAge)
    {
        m_maxAge  =  maxAge;
    }

    UI32  MstpVlanBridgeConfigMessage::getMaxAge() const
    {
        return (m_maxAge);
    }

    void  MstpVlanBridgeConfigMessage::setAgingTime(const UI32 &agingTime)
    {
        m_agingTime  =  agingTime;
    }

    UI32  MstpVlanBridgeConfigMessage::getAgingTime() const
    {
        return (m_agingTime);
    }

    void  MstpVlanBridgeConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpVlanBridgeConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpVlanBridgeConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  MstpVlanBridgeConfigMessage::getMode() const
    {
        return (m_mode);
    }

	const void *MstpVlanBridgeConfigMessage::getCStructureForInputs() 
	{
		mstp_msg_br_vlan_config *vlan_config = new mstp_msg_br_vlan_config;
		vlan_config->cmd = m_cmdCode;
		vlan_config->vid = m_id;
		vlan_config->hello_time = m_helloTime;
		vlan_config->max_age = m_maxAge;
		vlan_config->aging_time = m_agingTime;
		vlan_config->fwd_delay = m_forwardDelay;
		vlan_config->priority = m_priority;
		return vlan_config;
	}
}
