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

#include "Mstp/Global/MstpGlobalUpdateVlanBridgeConfigMessage.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

namespace DcmNs
{

    MstpGlobalUpdateVlanBridgeConfigMessage::MstpGlobalUpdateVlanBridgeConfigMessage ()
        : ManagementInterfaceMessage (MstpGlobalObjectManager::getClassName (),MSTPGLOBALUPDATEVLANBRIDGECONFIG)
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

    MstpGlobalUpdateVlanBridgeConfigMessage::MstpGlobalUpdateVlanBridgeConfigMessage
(const UI32 &id,const UI32 &priority,const UI32 &forwardDelay,const UI32 &helloTime,const UI32 &maxAge, const UI32 &agingTime, const UI32 &cmdCode, const UI32 &mode)
        : ManagementInterfaceMessage (MstpGlobalObjectManager::getClassName (),MSTPGLOBALUPDATEVLANBRIDGECONFIG),
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

    MstpGlobalUpdateVlanBridgeConfigMessage::~MstpGlobalUpdateVlanBridgeConfigMessage ()
    {
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_forwardDelay,"forwardDelay"));
        addSerializableAttribute (new AttributeUI32(&m_helloTime,"helloTime"));
        addSerializableAttribute (new AttributeUI32(&m_maxAge,"maxAge"));
        addSerializableAttribute (new AttributeUI32(&m_agingTime,"agingTime"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  MstpGlobalUpdateVlanBridgeConfigMessage::getId() const
    {
        return (m_id);
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpGlobalUpdateVlanBridgeConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setForwardDelay(const UI32 &forwardDelay)
    {
        m_forwardDelay  =  forwardDelay;
    }

    UI32  MstpGlobalUpdateVlanBridgeConfigMessage::getForwardDelay() const
    {
        return (m_forwardDelay);
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  MstpGlobalUpdateVlanBridgeConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setMaxAge(const UI32 &maxAge)
    {
        m_maxAge  =  maxAge;
    }

    UI32  MstpGlobalUpdateVlanBridgeConfigMessage::getMaxAge() const
    {
        return (m_maxAge);
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setAgingTime(const UI32 &agingTime)
    {
        m_agingTime  =  agingTime;
    }

    UI32  MstpGlobalUpdateVlanBridgeConfigMessage::getAgingTime() const
    {
        return (m_agingTime);
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpGlobalUpdateVlanBridgeConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  MstpGlobalUpdateVlanBridgeConfigMessage::getMode() const
    {
        return (m_mode);
    }

    void  MstpGlobalUpdateVlanBridgeConfigMessage::printMsg()
    {
        trace(TRACE_LEVEL_DEBUG, string("MstpGlobalUpdateVlanBridgeConfigMessage::printMsg()"));
        trace(TRACE_LEVEL_DEBUG, string("m_id:				")+toString(m_id));
        trace(TRACE_LEVEL_DEBUG, string("m_priority:		")+toString(m_priority));
        trace(TRACE_LEVEL_DEBUG, string("m_forwardDelay:	")+toString(m_forwardDelay));
        trace(TRACE_LEVEL_DEBUG, string("m_helloTime:		")+toString(m_helloTime));
        trace(TRACE_LEVEL_DEBUG, string("m_maxAge:         ")+toString(m_maxAge));
        trace(TRACE_LEVEL_DEBUG, string("m_agingTime:		")+toString(m_agingTime));
        trace(TRACE_LEVEL_DEBUG, string("m_mode:			")+toString(m_mode));
        trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:			")+toString(m_cmdCode));
    }

}
