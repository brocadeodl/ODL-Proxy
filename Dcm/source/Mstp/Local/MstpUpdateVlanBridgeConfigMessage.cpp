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

#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpUpdateVlanBridgeConfigMessage.h"
#include "Mstp/Local/MstpTypes.h"
#include "Mstp/Global/MstpGlobalUpdateVlanBridgeConfigMessage.h"

namespace DcmNs
{

    MstpUpdateVlanBridgeConfigMessage::MstpUpdateVlanBridgeConfigMessage ()
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEVLANBRIDGECONFIG)
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

    MstpUpdateVlanBridgeConfigMessage::MstpUpdateVlanBridgeConfigMessage(MstpGlobalUpdateVlanBridgeConfigMessage & global)
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEVLANBRIDGECONFIG)
    {
		m_id = global.m_id;
		m_priority = global.m_priority;
		m_forwardDelay = global.m_forwardDelay;	
		m_helloTime = global.m_helloTime;
		m_maxAge = global.m_maxAge;
		m_agingTime = global.m_agingTime;
		m_cmdCode = global.m_cmdCode;
		m_mode = global.m_mode;
    }

    MstpUpdateVlanBridgeConfigMessage::MstpUpdateVlanBridgeConfigMessage (const UI32 &id,const UI32 &priority,const UI32 &forwardDelay,const UI32 &helloTime,const UI32 &maxAge, const UI32 &agingTime,const UI32 &cmdCode, const UI32 &mode)
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEVLANBRIDGECONFIG),
        m_id    (id),
        m_priority    (priority),
        m_forwardDelay    (forwardDelay),
        m_helloTime    (helloTime),
        m_maxAge    (maxAge),
        m_agingTime    (agingTime),
        m_cmdCode    (cmdCode),
        m_mode    (mode)
    {
    }

    MstpUpdateVlanBridgeConfigMessage::~MstpUpdateVlanBridgeConfigMessage ()
    {
    }

    void  MstpUpdateVlanBridgeConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_forwardDelay,"forwardDelay"));
        addSerializableAttribute (new AttributeUI32(&m_helloTime,"helloTime"));
        addSerializableAttribute (new AttributeUI32(&m_maxAge,"maxAge"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeUI32(&m_agingTime,"agingTime"));
    }

    void  MstpUpdateVlanBridgeConfigMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  MstpUpdateVlanBridgeConfigMessage::getId() const
    {
        return (m_id);
    }

    void  MstpUpdateVlanBridgeConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpUpdateVlanBridgeConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpUpdateVlanBridgeConfigMessage::setForwardDelay(const UI32 &forwardDelay)
    {
        m_forwardDelay  =  forwardDelay;
    }

    UI32  MstpUpdateVlanBridgeConfigMessage::getForwardDelay() const
    {
        return (m_forwardDelay);
    }

    void  MstpUpdateVlanBridgeConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  MstpUpdateVlanBridgeConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  MstpUpdateVlanBridgeConfigMessage::setMaxAge(const UI32 &maxAge)
    {
        m_maxAge  =  maxAge;
    }

    UI32  MstpUpdateVlanBridgeConfigMessage::getMaxAge() const
    {
        return (m_maxAge);
    }

    void  MstpUpdateVlanBridgeConfigMessage::setAgingTime(const UI32 &agingTime)
    {
        m_agingTime  =  agingTime;
    }

    UI32  MstpUpdateVlanBridgeConfigMessage::getAgingTime() const
    {
        return (m_agingTime);
    }

    void  MstpUpdateVlanBridgeConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpUpdateVlanBridgeConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpUpdateVlanBridgeConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  MstpUpdateVlanBridgeConfigMessage::getMode() const
    {
        return (m_mode);
    }
}
