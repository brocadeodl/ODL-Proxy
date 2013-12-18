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
 *   .Author : sudheend                                                     *
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdatePoInstanceStpConfigSPMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Global/NsmGlobalUpdatePoInstanceStpConfigMessage.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

namespace DcmNs
{

    NsmUpdatePoInstanceStpConfigSPMessage::NsmUpdatePoInstanceStpConfigSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMUPDATEPOINSTANCESTPCONFIGSP)
    {
		m_ifName = "";
		m_ifIndex = 0;
		m_priority = 0;
		m_cost = 0;
		m_cmdCode = 0;
		m_instanceId = 0;
		m_guard = MSTPPORTNOGUARDROOT;
		m_restrictedRole = false;
		m_restrictedTcn = false;
    }

    NsmUpdatePoInstanceStpConfigSPMessage::NsmUpdatePoInstanceStpConfigSPMessage(NsmGlobalUpdatePoInstanceStpConfigMessage &message)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMUPDATEPOINSTANCESTPCONFIGSP)
	{
		m_ifName = message.m_ifName;
		m_ifIndex = 0;
		m_instanceId = message.m_instanceId;
		m_priority = message.m_priority;
		m_cost = message.m_cost;
		m_cmdCode = message.m_cmdCode;
		m_guard = MSTPPORTNOGUARDROOT;
		m_restrictedRole = false;
		m_restrictedTcn = false;

	}

    NsmUpdatePoInstanceStpConfigSPMessage::NsmUpdatePoInstanceStpConfigSPMessage (const string &ifName,const UI8 &instanceId,const UI32 &priority,const UI32 &cost,const UI32 &cmdCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMUPDATEPOINSTANCESTPCONFIGSP),
        m_ifName    (ifName),
        m_instanceId    (instanceId),
        m_priority    (priority),
        m_cost    (cost),
        m_cmdCode    (cmdCode)
    {
		m_guard = MSTPPORTNOGUARDROOT;
		m_ifIndex = 0;
		m_restrictedRole = false;
		m_restrictedTcn = false;
    }

    NsmUpdatePoInstanceStpConfigSPMessage::~NsmUpdatePoInstanceStpConfigSPMessage ()
    {
    }

    void  NsmUpdatePoInstanceStpConfigSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_cost,"ifIndex"));
        addSerializableAttribute (new AttributeUI8(&m_instanceId,"instanceId"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_cost,"cost"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
    }

    void  NsmUpdatePoInstanceStpConfigSPMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmUpdatePoInstanceStpConfigSPMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmUpdatePoInstanceStpConfigSPMessage::setInstanceId(const UI8 &instanceId)
    {
        m_instanceId  =  instanceId;
    }

    UI8  NsmUpdatePoInstanceStpConfigSPMessage::getInstanceId() const
    {
        return (m_instanceId);
    }

    void  NsmUpdatePoInstanceStpConfigSPMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  NsmUpdatePoInstanceStpConfigSPMessage::getPriority() const
    {
        return (m_priority);
    }

    void  NsmUpdatePoInstanceStpConfigSPMessage::setCost(const UI32 &cost)
    {
        m_cost  =  cost;
    }

    UI32  NsmUpdatePoInstanceStpConfigSPMessage::getCost() const
    {
        return (m_cost);
    }

   void  NsmUpdatePoInstanceStpConfigSPMessage::setGuard(const MstpPortGuardRootEnum &guard)
    {
        m_guard  =  guard;
    }

    MstpPortGuardRootEnum  NsmUpdatePoInstanceStpConfigSPMessage::getGuard() const
    {
        return (m_guard);
    }
  void  NsmUpdatePoInstanceStpConfigSPMessage::setRestrictedRole(const bool &restrictedRole)
    {
        m_restrictedRole  =  restrictedRole;
    }

    bool  NsmUpdatePoInstanceStpConfigSPMessage::getRestrictedRole() const
    {
        return (m_restrictedRole);
    }

    void  NsmUpdatePoInstanceStpConfigSPMessage::setRestrictedTcn(const bool &restrictedTcn)
    {
        m_restrictedTcn  =  restrictedTcn;
    }

    bool  NsmUpdatePoInstanceStpConfigSPMessage::getRestrictedTcn() const
    {
        return (m_restrictedTcn);
    }


    void  NsmUpdatePoInstanceStpConfigSPMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmUpdatePoInstanceStpConfigSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmUpdatePoInstanceStpConfigSPMessage::printMsg()
    {
	    trace(TRACE_LEVEL_DEBUG, string("NsmGlobalUpdatePoStpConfigMessage::printMsg()"));
	    trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:		")+toString(m_cmdCode));
	    trace(TRACE_LEVEL_DEBUG, string("m_ifName:		")+m_ifName);
	    trace(TRACE_LEVEL_DEBUG, string("m_ifIndex:		")+toString(m_ifIndex));
	    trace(TRACE_LEVEL_DEBUG, string("m_instanceId:		")+toString(UI32(m_instanceId)));

	    trace(TRACE_LEVEL_DEBUG, string("m_priority:	")+toString((UI32 )m_priority));
	    trace(TRACE_LEVEL_DEBUG, string("m_cost:		")+toString(m_cost));
    }
}
