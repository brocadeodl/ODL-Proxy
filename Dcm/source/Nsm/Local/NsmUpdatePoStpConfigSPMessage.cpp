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

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdatePoStpConfigSPMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Global/NsmGlobalUpdatePoStpConfigMessage.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

namespace DcmNs
{

    NsmUpdatePoStpConfigSPMessage::NsmUpdatePoStpConfigSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMUPDATEPOSTPCONFIGSP)
    {
	    m_ifName = "";
	    m_ifIndex = 0;
	    m_priority = 0;
	    m_helloTime  = 0;
	    m_cost = 0;
	    m_cmdCode = 0;
		m_autoEdge = false;
        m_bpduGuardEnable = false;
        m_bpduGuard = false;
        m_bpduFilter = false;
        m_edgePort = true;
        m_guard = MSTPPORTNOGUARDROOT;
		m_linkShared = MSTPPORTLINKTYPEP2P;
        m_mac = MSTPPORTPVSTBPDUMACBRCD;
        m_portFast = false;
        m_priority = 0;
        m_restrictedRole = true;
        m_restrictedTcn = true;
        m_stpEnable = false;
		m_portId = 0;
    }

    NsmUpdatePoStpConfigSPMessage::NsmUpdatePoStpConfigSPMessage (const string &ifName,const UI16 &priority,const UI32 &helloTime,const UI32 &cost,const UI32 &cmdCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId() ,NSMUPDATEPOSTPCONFIGSP),
        m_ifName    (ifName),
        m_priority    (priority),
        m_helloTime    (helloTime),
        m_cost    (cost),
        m_portId ( 0 ),
        m_cmdCode    (cmdCode)
    {
			m_autoEdge = true;
			m_bpduFilter = false;
			m_bpduGuardEnable = false;
			m_bpduGuard = false;
			m_edgePort = true;
			m_guard = MSTPPORTNOGUARDROOT;
			m_ifIndex = 0;
			m_linkShared = MSTPPORTLINKTYPEP2P; 
			m_mac = MSTPPORTPVSTBPDUMACBRCD; 
			m_portFast = false;
			m_restrictedRole = true;
			m_restrictedTcn = true;
			m_stpEnable = false;
 
    }

    NsmUpdatePoStpConfigSPMessage::NsmUpdatePoStpConfigSPMessage (NsmGlobalUpdatePoStpConfigMessage &globalMsg)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMUPDATEPOSTPCONFIGSP)
    {
        m_ifName = globalMsg.m_ifName;
        m_ifIndex = 0;
        m_priority  = globalMsg.m_priority;
        m_helloTime  = globalMsg.m_helloTime;
        m_cost  = globalMsg.m_cost;
        m_cmdCode  = globalMsg.m_cmdCode;
		m_autoEdge = true;
			m_bpduFilter = false;
			m_bpduGuardEnable = globalMsg.m_bpduGuardEnable;
			m_bpduGuard = false;
			m_edgePort = true;
			m_guard = MSTPPORTNOGUARDROOT;
			m_linkShared = MSTPPORTLINKTYPEP2P; 
			m_mac = MSTPPORTPVSTBPDUMACBRCD; 
			m_portFast = false;
			m_restrictedRole = true;
			m_restrictedTcn = true;
			m_stpEnable = false;
			m_portId = 0;
    }

    NsmUpdatePoStpConfigSPMessage::~NsmUpdatePoStpConfigSPMessage ()
    {
    }

    void  NsmUpdatePoStpConfigSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI16(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_helloTime,"helloTime"));
        addSerializableAttribute (new AttributeUI32(&m_cost,"cost"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_portId,"portId"));
    }

    void  NsmUpdatePoStpConfigSPMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmUpdatePoStpConfigSPMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmUpdatePoStpConfigSPMessage::setIfIndex(const UI32 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI32  NsmUpdatePoStpConfigSPMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  NsmUpdatePoStpConfigSPMessage::setPriority(const UI16 &priority)
    {
        m_priority  =  priority;
    }

    UI16  NsmUpdatePoStpConfigSPMessage::getPriority() const
    {
        return (m_priority);
    }

    void  NsmUpdatePoStpConfigSPMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  NsmUpdatePoStpConfigSPMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  NsmUpdatePoStpConfigSPMessage::setCost(const UI32 &cost)
    {
        m_cost  =  cost;
    }

    UI32  NsmUpdatePoStpConfigSPMessage::getCost() const
    {
        return (m_cost);
    }



    void  NsmUpdatePoStpConfigSPMessage::setRestrictedRole(const bool &restrictedRole)
    {
        m_restrictedRole  =  restrictedRole;
    }

    bool  NsmUpdatePoStpConfigSPMessage::getRestrictedRole() const
    {
        return (m_restrictedRole);
    }

    void  NsmUpdatePoStpConfigSPMessage::setRestrictedTcn(const bool &restrictedTcn)
    {
        m_restrictedTcn  =  restrictedTcn;
    }


    bool  NsmUpdatePoStpConfigSPMessage::getRestrictedTcn() const
    {
        return (m_restrictedTcn);
    }

    void  NsmUpdatePoStpConfigSPMessage::setAutoEdge(const bool &autoEdge)
    {
        m_autoEdge  =  autoEdge;
    }

    bool  NsmUpdatePoStpConfigSPMessage::getAutoEdge() const
    {
        return (m_autoEdge);
    }

    void  NsmUpdatePoStpConfigSPMessage::setPortFast(const bool &portFast)
    {
        m_portFast  =  portFast;
    }

    bool  NsmUpdatePoStpConfigSPMessage::getPortFast() const
    {
        return (m_portFast);
    }

    void  NsmUpdatePoStpConfigSPMessage::setEdgePort(const bool &edgePort)
    {
        m_edgePort  =  edgePort;
    }

    bool  NsmUpdatePoStpConfigSPMessage::getEdgePort() const
    {
        return (m_edgePort);
    }

    void  NsmUpdatePoStpConfigSPMessage::setBpduGuardEnable(const bool &bpduGuardEnable)
    {
        m_bpduGuardEnable  =  bpduGuardEnable;
    }

    bool  NsmUpdatePoStpConfigSPMessage::getBpduGuardEnable() const
    {
        return (m_bpduGuardEnable);
    }

    void  NsmUpdatePoStpConfigSPMessage::setBpduGuard(const bool &bpduGuard)
    {
        m_bpduGuard  =  bpduGuard;
    }

    bool  NsmUpdatePoStpConfigSPMessage::getBpduGuard() const
    {
        return (m_bpduGuard);
    }

    void  NsmUpdatePoStpConfigSPMessage::setBpduFilter(const bool &bpduFilter)
    {
        m_bpduFilter  =  bpduFilter;
    }

    bool  NsmUpdatePoStpConfigSPMessage::getBpduFilter() const
    {
        return (m_bpduFilter);
    }

    void  NsmUpdatePoStpConfigSPMessage::setGuard(const MstpPortGuardRootEnum &guard)
    {
        m_guard  =  guard;
    }

    MstpPortGuardRootEnum  NsmUpdatePoStpConfigSPMessage::getGuard() const
    {
        return (m_guard);
    }

    void  NsmUpdatePoStpConfigSPMessage::setLinkShared(const MstpPortLinkTypeEnum &linkShared)
    {
        m_linkShared  =  linkShared;
    }

    MstpPortLinkTypeEnum  NsmUpdatePoStpConfigSPMessage::getLinkShared() const
    {
        return (m_linkShared);
    }

    void  NsmUpdatePoStpConfigSPMessage::setStpEnable(const bool &stpEnable)
    {
        m_stpEnable  =  stpEnable;
    }

    bool  NsmUpdatePoStpConfigSPMessage::getStpEnable() const
    {
        return (m_stpEnable);
    }

	void NsmUpdatePoStpConfigSPMessage::setBpduMac(const MstpPortPvstBpDuMac &mac)
	{
		m_mac = mac;
	}

	MstpPortPvstBpDuMac NsmUpdatePoStpConfigSPMessage::getBpduMac()  const
	{
		return m_mac;
	}

    void  NsmUpdatePoStpConfigSPMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmUpdatePoStpConfigSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void NsmUpdatePoStpConfigSPMessage::setPortId(const UI32 portId)
    {
        m_portId = portId;
    }

    UI32 NsmUpdatePoStpConfigSPMessage::getPortId() const
    {
        return m_portId;
    }

    void  NsmUpdatePoStpConfigSPMessage::printMsg()
    {
	    trace(TRACE_LEVEL_DEBUG, string("NsmGlobalUpdatePoStpConfigMessage::printMsg()"));
	    trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:		")+toString(m_cmdCode));
	    trace(TRACE_LEVEL_DEBUG, string("m_ifName:		")+m_ifName);
	    trace(TRACE_LEVEL_DEBUG, string("m_priority:	")+toString((UI32 )m_priority));
	    trace(TRACE_LEVEL_DEBUG, string("m_helloTime:	")+toString(m_helloTime));
	    trace(TRACE_LEVEL_DEBUG, string("m_cost:		")+toString(m_cost));
	    trace(TRACE_LEVEL_DEBUG, string("m_portId:      ")+toString(m_portId));
    }
}
