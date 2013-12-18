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

#include "ClientInterface/OSPF/OSPFPermitDenyRedistributeMessage.h"
#include "OSPF/Local/OSPFPermitDenyRedistributeLocalMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    OSPFPermitDenyRedistributeMessage::OSPFPermitDenyRedistributeMessage ()
        : DcmManagementInterfaceMessage (OSPFPERMITDENYCONFIG)
    {
    }

    OSPFPermitDenyRedistributeMessage::OSPFPermitDenyRedistributeMessage (const UI8 &opCode,const UI8 &cmdCode,const UI32 &index, const OSPFPermitDenyOption &permitDenyOption, const OSPFRouteOption &routeOption, const IpV4Address &subNet, const IpV4Address &mask, const UI32 &matchMetric, const UI32 &setMetric)
        : DcmManagementInterfaceMessage (OSPFPERMITDENYCONFIG),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
	m_index    (index),
	m_permitDenyOption    (permitDenyOption),
	m_routeOption (routeOption),
	m_subNet    (subNet),
	m_mask    (mask),
	m_matchMetric    (matchMetric),
	m_setMetric (setMetric)

    {
    }

    OSPFPermitDenyRedistributeMessage::~OSPFPermitDenyRedistributeMessage ()
    {
    }

    void  OSPFPermitDenyRedistributeMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
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

    void  OSPFPermitDenyRedistributeMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  OSPFPermitDenyRedistributeMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  OSPFPermitDenyRedistributeMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  OSPFPermitDenyRedistributeMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  OSPFPermitDenyRedistributeMessage::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  OSPFPermitDenyRedistributeMessage::getIndex() const
    {
        return (m_index);
    }

    void OSPFPermitDenyRedistributeMessage::setPermitDenyOption(const OSPFPermitDenyOption &permitDenyOption)
    {
        m_permitDenyOption = permitDenyOption;
    }

    OSPFPermitDenyOption OSPFPermitDenyRedistributeMessage::getPermitDenyOption()  const
    {
        return (m_permitDenyOption);
    }

    void OSPFPermitDenyRedistributeMessage::setRouteOption(const OSPFRouteOption &routeOption)
    {
        m_routeOption = routeOption;
    }

    OSPFRouteOption OSPFPermitDenyRedistributeMessage::getRouteOption() const
    {
        return (m_routeOption);
    }

    void  OSPFPermitDenyRedistributeMessage::setSubNet(const IpV4Address &subNet)
    {
        m_subNet  =  subNet;
    }

    IpV4Address  OSPFPermitDenyRedistributeMessage::getSubNet() const
    {
        return (m_subNet);
    }

    void  OSPFPermitDenyRedistributeMessage::setMask(const IpV4Address &mask)
    {
        m_mask  =  mask;
    }

    IpV4Address  OSPFPermitDenyRedistributeMessage::getMask() const
    {
        return (m_mask);
    }

    void  OSPFPermitDenyRedistributeMessage::setMatchMetric(const UI32 &matchMetric)
    {
        m_matchMetric  =  matchMetric;
    }

    UI32  OSPFPermitDenyRedistributeMessage::getMatchMetric() const
    {
        return (m_matchMetric);
    }

    void  OSPFPermitDenyRedistributeMessage::setSetMetric(const UI32 &setMetric)
    {
        m_setMetric  =  setMetric;
    }

    UI32  OSPFPermitDenyRedistributeMessage::getSetMetric() const
    {
        return (m_setMetric);
    }

    const void *OSPFPermitDenyRedistributeMessage::getCStructureForInputs ()
    {
        OSPFPermitDenyRedistributeConfigMsg_t *pInput = new OSPFPermitDenyRedistributeConfigMsg_t;
        pInput->opCode = m_opCode;
        pInput->cmdCode = m_cmdCode;
		pInput->m_index = m_index;
		pInput->m_permitDenyOption = m_permitDenyOption;
		pInput->m_routeOption = m_routeOption;

		if(m_subNet.toString() != string("0.0.0.0"))
			strcpy(pInput->m_subNet ,(m_subNet.toString()).c_str());
		else
			pInput->m_subNet[0] = '\0';

		if(m_mask.toString() != string("0.0.0.0"))
			strcpy(pInput->m_mask ,(m_mask.toString()).c_str());
		else
			pInput->m_mask[0] = '\0';

		pInput->m_matchMetric = m_matchMetric;
		pInput->m_setMetric = m_setMetric;
        return (pInput);
    }

    void OSPFPermitDenyRedistributeMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

	void OSPFPermitDenyRedistributeMessage::updateFromLocalMessage (OSPFPermitDenyRedistributeLocalMessage *pMsg)
	{
            m_opCode = pMsg->m_opCode;
            m_cmdCode = pMsg->m_cmdCode;
            m_index = pMsg->m_index;
            m_permitDenyOption = pMsg->m_permitDenyOption;
            m_routeOption = pMsg->m_routeOption;
            m_subNet = pMsg->m_subNet;
            m_mask = pMsg->m_mask;
            m_matchMetric = pMsg->m_matchMetric;
            m_setMetric = pMsg->m_setMetric;
	}
}
