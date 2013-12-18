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
 *   Author : hraza                                                        *
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "OSPF/Local/OSPFPermitDenyRedistributeLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    OSPFPermitDenyRedistributeLocalMessage::OSPFPermitDenyRedistributeLocalMessage ()
        : PrismMessage (OSPFLocalObjectManager::getPrismServiceId (),OSPFPERMITDENYCONFIGLOCAL)
    {
            m_opCode = 0;
            m_cmdCode = 0;
            m_index = 0;
            m_permitDenyOption = OSPF_PERMIT_DENY_OPTION_NONE;
            m_routeOption = OSPF_ROUTE_OPTION_NONE;
            m_subNet.fromString("0.0.0.0");
            m_mask.fromString("0.0.0.0");
            m_matchMetric = 0;
			m_setMetric = 0;
    }

    OSPFPermitDenyRedistributeLocalMessage::OSPFPermitDenyRedistributeLocalMessage (const UI8 &opCode,const UI8 &cmdCode,const UI32 &index, const OSPFPermitDenyOption &permitDenyOption, const OSPFRouteOption &routeOption, const IpV4Address &subNet, const IpV4Address &mask, const UI32 &matchMetric, const UI32 &setMetric)
        : PrismMessage (OSPFLocalObjectManager::getPrismServiceId (),OSPFPERMITDENYCONFIGLOCAL),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
	m_index    (index),
	m_permitDenyOption    (permitDenyOption),
	m_routeOption (routeOption),
	m_subNet    (subNet),
	m_mask    (mask),
	m_matchMetric    (matchMetric),
	m_setMetric    (setMetric)

    {
    }

    OSPFPermitDenyRedistributeLocalMessage::~OSPFPermitDenyRedistributeLocalMessage ()
    {
    }

    void  OSPFPermitDenyRedistributeLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_index,"index"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_permitDenyOption,"permitDenyOption"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_routeOption,"routeOption"));
        addSerializableAttribute (new AttributeIpV4Address(&m_subNet,"subNet"));
        addSerializableAttribute (new AttributeIpV4Address(&m_mask,"mask"));
        addSerializableAttribute (new AttributeUI32(&m_matchMetric,"matchMetric"));
        addSerializableAttribute (new AttributeUI32(&m_setMetric,"setMetric"));

    }

    void  OSPFPermitDenyRedistributeLocalMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  OSPFPermitDenyRedistributeLocalMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  OSPFPermitDenyRedistributeLocalMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  OSPFPermitDenyRedistributeLocalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  OSPFPermitDenyRedistributeLocalMessage::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  OSPFPermitDenyRedistributeLocalMessage::getIndex() const
    {
        return (m_index);
    }

    void OSPFPermitDenyRedistributeLocalMessage::setPermitDenyOption(const OSPFPermitDenyOption &permitDenyOption)
    {
        m_permitDenyOption = permitDenyOption;
    }

    OSPFPermitDenyOption OSPFPermitDenyRedistributeLocalMessage::getPermitDenyOption()  const
    {
        return (m_permitDenyOption);
    }

    void OSPFPermitDenyRedistributeLocalMessage::setRouteOption(const OSPFRouteOption &routeOption)
    {
        m_routeOption = routeOption;
    }

    OSPFRouteOption OSPFPermitDenyRedistributeLocalMessage::getRouteOption() const
    {
        return (m_routeOption);
    }

    void  OSPFPermitDenyRedistributeLocalMessage::setSubNet(const IpV4Address &subNet)
    {
        m_subNet  =  subNet;
    }

    IpV4Address  OSPFPermitDenyRedistributeLocalMessage::getSubNet() const
    {
        return (m_subNet);
    }

    void  OSPFPermitDenyRedistributeLocalMessage::setMask(const IpV4Address &mask)
    {
        m_mask  =  mask;
    }

    IpV4Address  OSPFPermitDenyRedistributeLocalMessage::getMask() const
    {
        return (m_mask);
    }

    void  OSPFPermitDenyRedistributeLocalMessage::setMatchMetric(const UI32 &matchMetric)
    {
        m_matchMetric  =  matchMetric;
    }

    UI32  OSPFPermitDenyRedistributeLocalMessage::getMatchMetric() const
    {
        return (m_matchMetric);
    }

    void  OSPFPermitDenyRedistributeLocalMessage::setSetMetric(const UI32 &setMetric)
    {
        m_setMetric  =  setMetric;
    }

    UI32  OSPFPermitDenyRedistributeLocalMessage::getSetMetric() const
    {
        return (m_setMetric);
    }

    void  OSPFPermitDenyRedistributeLocalMessage::dumpMessage() const
	{
            trace (TRACE_LEVEL_DEVEL, string("m_opCode : ") + m_opCode);
            trace (TRACE_LEVEL_DEVEL, string("m_cmdCode : ") + m_cmdCode);
            trace (TRACE_LEVEL_DEVEL, string("m_index : ") + m_index);
            trace (TRACE_LEVEL_DEVEL, string("m_permitDenyOption : ") + m_permitDenyOption);
            trace (TRACE_LEVEL_DEVEL, string("m_routeOption : ") + m_routeOption);
            trace (TRACE_LEVEL_DEVEL, string("m_subNet : ") + m_subNet.toString());
            trace (TRACE_LEVEL_DEVEL, string("m_mask : ") + m_mask.toString());
            trace (TRACE_LEVEL_DEVEL, string("m_matchMetric : ") + m_matchMetric);
            trace (TRACE_LEVEL_DEVEL, string("m_setMetric : ") + m_setMetric);
	}

}
