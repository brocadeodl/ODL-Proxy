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

#include "Nsm/Global/NsmGlobalUpdatePoStpConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"

namespace DcmNs
{

    NsmGlobalUpdatePoStpConfigMessage::NsmGlobalUpdatePoStpConfigMessage ()
        : NsmPortStpConfigMessage (NsmGlobalObjectManager::getClassName(), NSMGLOBALUPDATEPOSTPCONFIG)
        , m_bpduGuardEnable ( false )
        , m_helloTime       ( 0 )
    	, m_bpduDropDirection	(BPDU_DROP_DISABLED)
	{
	}

    NsmGlobalUpdatePoStpConfigMessage::NsmGlobalUpdatePoStpConfigMessage (const string &ifName,const UI16 &priority,const UI32 &helloTime,const UI32 &cost,const UI32 &cmdCode)
        : NsmPortStpConfigMessage (NsmGlobalObjectManager::getClassName(), NSMGLOBALUPDATEPOSTPCONFIG, cmdCode, IF_TYPE_PO, ifName, cost, priority)
        , m_bpduGuardEnable ( false )
        , m_helloTime       ( helloTime )
    	, m_bpduDropDirection(BPDU_DROP_DISABLED)
    {
    }

    NsmGlobalUpdatePoStpConfigMessage::~NsmGlobalUpdatePoStpConfigMessage ()
    {
    }

    void  NsmGlobalUpdatePoStpConfigMessage::setupAttributesForSerialization()
    {
        NsmPortStpConfigMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeUI32(&m_bpduGuardEnable,"bpduGuardEnable"));
        addSerializableAttribute (new AttributeUI32(&m_helloTime,"helloTime"));
        addSerializableAttribute (new AttributeUI8(&m_bpduDropDirection,"bpduDropDirection"));
    }

    void  NsmGlobalUpdatePoStpConfigMessage::setIfName(const string &ifName)
    {
        setInterfaceInfo(IF_TYPE_PO, ifName);
    }

    void  NsmGlobalUpdatePoStpConfigMessage::setBpduGuardEnable(const bool &bpduGuardEnable)
    {
        m_bpduGuardEnable  =  bpduGuardEnable;
    }

    bool  NsmGlobalUpdatePoStpConfigMessage::getBpduGuardEnable() const
    {
        return (m_bpduGuardEnable);
    }

    void  NsmGlobalUpdatePoStpConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  NsmGlobalUpdatePoStpConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  NsmGlobalUpdatePoStpConfigMessage::setBpduDropDirection(const UI8 direction)
    {
        m_bpduDropDirection  =  direction;
    }

    UI8	  NsmGlobalUpdatePoStpConfigMessage::getBpduDropDirection() const
    {
        return (m_bpduDropDirection);
    }

    void  NsmGlobalUpdatePoStpConfigMessage::printMsg()
    {
	    trace(TRACE_LEVEL_DEBUG, string("NsmGlobalUpdatePoStpConfigMessage::printMsg()"));
	    trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:		")+toString(m_cmdCode));
	    trace(TRACE_LEVEL_DEBUG, string("m_ifName:		")+m_ifName);
	    trace(TRACE_LEVEL_DEBUG, string("m_priority:	")+toString((UI32 )m_priority));
	    trace(TRACE_LEVEL_DEBUG, string("m_helloTime:	")+toString(m_helloTime));
	    trace(TRACE_LEVEL_DEBUG, string("m_cost:		")+toString(m_cost));
	    trace(TRACE_LEVEL_DEBUG, string("m_bpduDropDirection:		")+toString(m_bpduDropDirection));
    }


}
