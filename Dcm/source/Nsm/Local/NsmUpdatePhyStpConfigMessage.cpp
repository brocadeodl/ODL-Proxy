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
#include "Nsm/Local/NsmUpdatePhyStpConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"


namespace DcmNs
{

    NsmUpdatePhyStpConfigMessage::NsmUpdatePhyStpConfigMessage ()
        : NsmPortStpConfigMessage (NsmLocalObjectManager::getClassName (),NSMUPDATEPHYSTPCONFIG)
        , m_helloTime       ( 0 )
        , m_restrictedRole  ( true  )
        , m_restrictedTcn   ( true  )
        , m_autoEdge        ( false )
        , m_portFast        ( false )
        , m_edgePort        ( true  )
        , m_bpduGuardEnable ( false )
        , m_bpduGuard       ( false )
        , m_bpduFilter      ( false )
        , m_guard           ( MSTPPORTNOGUARDROOT )
        , m_linkShared      ( MSTPPORTLINKTYPEP2P )
        , m_mac             ( MSTPPORTPVSTBPDUMACBRCD )
        , m_stpEnable       ( false )
    	, m_bpduDropDirection(BPDU_DROP_DISABLED)
    {
    }

    NsmUpdatePhyStpConfigMessage::~NsmUpdatePhyStpConfigMessage ()
    {
    }

    void  NsmUpdatePhyStpConfigMessage::setupAttributesForSerialization()
    {
        NsmPortStpConfigMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeUI32(&m_helloTime,"helloTime"));
        addSerializableAttribute (new AttributeUI8(&m_bpduDropDirection,"bpduDropDirection"));
    }

    void  NsmUpdatePhyStpConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  NsmUpdatePhyStpConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  NsmUpdatePhyStpConfigMessage::setRestrictedRole(const bool &restrictedRole)
    {
        m_restrictedRole  =  restrictedRole;
    }

    bool  NsmUpdatePhyStpConfigMessage::getRestrictedRole() const
    {
        return (m_restrictedRole);
    }

    void  NsmUpdatePhyStpConfigMessage::setRestrictedTcn(const bool &restrictedTcn)
    {
        m_restrictedTcn  =  restrictedTcn;
    }

    bool  NsmUpdatePhyStpConfigMessage::getRestrictedTcn() const
    {
        return (m_restrictedTcn);
    }

    void  NsmUpdatePhyStpConfigMessage::setAutoEdge(const bool &autoEdge)
    {
        m_autoEdge  =  autoEdge;
    }

    bool  NsmUpdatePhyStpConfigMessage::getAutoEdge() const
    {
        return (m_autoEdge);
    }

    void  NsmUpdatePhyStpConfigMessage::setPortFast(const bool &portFast)
    {
        m_portFast  =  portFast;
    }

    bool  NsmUpdatePhyStpConfigMessage::getPortFast() const
    {
        return (m_portFast);
    }

    void  NsmUpdatePhyStpConfigMessage::setEdgePort(const bool &edgePort)
    {
        m_edgePort  =  edgePort;
    }

    bool  NsmUpdatePhyStpConfigMessage::getEdgePort() const
    {
        return (m_edgePort);
    }

    void  NsmUpdatePhyStpConfigMessage::setBpduGuardEnable(const bool &bpduGuardEnable)
    {
        m_bpduGuardEnable  =  bpduGuardEnable;
    }

    bool  NsmUpdatePhyStpConfigMessage::getBpduGuardEnable() const
    {
        return (m_bpduGuardEnable);
    }

    void  NsmUpdatePhyStpConfigMessage::setBpduGuard(const bool &bpduGuard)
    {
        m_bpduGuard  =  bpduGuard;
    }

    bool  NsmUpdatePhyStpConfigMessage::getBpduGuard() const
    {
        return (m_bpduGuard);
    }

    void  NsmUpdatePhyStpConfigMessage::setBpduFilter(const bool &bpduFilter)
    {
        m_bpduFilter  =  bpduFilter;
    }

    bool  NsmUpdatePhyStpConfigMessage::getBpduFilter() const
    {
        return (m_bpduFilter);
    }

    void  NsmUpdatePhyStpConfigMessage::setGuard(const MstpPortGuardRootEnum &guard)
    {
        m_guard  =  guard;
    }

    MstpPortGuardRootEnum  NsmUpdatePhyStpConfigMessage::getGuard() const
    {
        return (m_guard);
    }

    void  NsmUpdatePhyStpConfigMessage::setLinkShared(const MstpPortLinkTypeEnum &linkShared)
    {
        m_linkShared  =  linkShared;
    }

    MstpPortLinkTypeEnum  NsmUpdatePhyStpConfigMessage::getLinkShared() const
    {
        return (m_linkShared);
    }

    void  NsmUpdatePhyStpConfigMessage::setStpEnable(const bool &stpEnable)
    {
        m_stpEnable  =  stpEnable;
    }

    bool  NsmUpdatePhyStpConfigMessage::getStpEnable() const
    {
        return (m_stpEnable);
    }

	void NsmUpdatePhyStpConfigMessage::setBpduMac(const MstpPortPvstBpDuMac &mac)
	{
		m_mac = mac;
	}

	MstpPortPvstBpDuMac NsmUpdatePhyStpConfigMessage::getBpduMac()  const
	{
		return m_mac;
	}

	void NsmUpdatePhyStpConfigMessage::setBpduDropDirection(const UI8 direction)
	{
		m_bpduDropDirection = direction;
	}

	UI8 NsmUpdatePhyStpConfigMessage::getBpduDropDirection()  const
	{
		return m_bpduDropDirection;
	}
 

    void  NsmUpdatePhyStpConfigMessage::printMsg()
    {
            trace(TRACE_LEVEL_DEBUG, string("NsmUpdatePhyStpConfigMessage::printMsg()"));
            trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:         ")+toString(m_cmdCode));
            trace(TRACE_LEVEL_DEBUG, string("m_ifName:          ")+m_ifName);
            trace(TRACE_LEVEL_DEBUG, string("m_priority:        ")+toString((UI32 )m_priority));
            trace(TRACE_LEVEL_DEBUG, string("m_helloTime:       ")+toString(m_helloTime));
            trace(TRACE_LEVEL_DEBUG, string("m_cost:            ")+toString(m_cost));
            trace(TRACE_LEVEL_DEBUG, string("m_bpduDropDirection:            ")+toString(m_bpduDropDirection));
    }

}
