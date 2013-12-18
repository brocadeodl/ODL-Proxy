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
 **************************************************************************/

#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/OSPF/OSPFAreaRangeMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"


namespace DcmNs
{

    OSPFAreaRangeMessage::OSPFAreaRangeMessage ()
        : DcmManagementInterfaceMessage (OSPFAREARANGE)
    {
    }

    OSPFAreaRangeMessage::~OSPFAreaRangeMessage ()
    {
    }

    void  OSPFAreaRangeMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_areaId, "areaId"));
        addSerializableAttribute (new AttributeString(&m_vrf, "vrf"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ipAddr, "rangeAddress"));
		addSerializableAttribute (new AttributeIpV4Address(&m_ipMask, "rangeMask"));
        addSerializableAttribute (new AttributeUI32(&m_rangeEffect, "rangeEffect"));
        addSerializableAttribute (new AttributeUI32(&m_rangeCost, "rangeCost"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_opCode), "opCode"));
    }

	void OSPFAreaRangeMessage::setOpCode(const OSPFOpCode &opCode)
	{
		m_opCode = opCode;
	}

	OSPFOpCode OSPFAreaRangeMessage::getOpCode() const
	{
		return (m_opCode);
	}

	void OSPFAreaRangeMessage::setCmdCode(unsigned int code)
	{
		m_cmdCode = code;
	}

	unsigned int OSPFAreaRangeMessage::getCmdCode()
	{
		return (m_cmdCode);
	}

    void  OSPFAreaRangeMessage::setAreaId(const string &areaId)
    {
        m_areaId  =  areaId;
    }

    string  OSPFAreaRangeMessage::getAreaId() const
    {
        return (m_areaId);
    }

	void  OSPFAreaRangeMessage::setIpAddr(const IpV4Address &ip)
	{
		m_ipAddr = ip;
	}

	IpV4Address  OSPFAreaRangeMessage::getIpAddr()  const
	{
		return (m_ipAddr);
	}

	void  OSPFAreaRangeMessage::setIpMask(const IpV4Address &ip)
	{
		m_ipMask = ip;
	}

	IpV4Address  OSPFAreaRangeMessage::getIpMask()  const
	{
		return (m_ipMask);
	}

	void OSPFAreaRangeMessage::setRangeEffect(const UI32 &effect)
	{
		m_rangeEffect = effect;
	}

	UI32  OSPFAreaRangeMessage::getRangeEffect()  const
	{
		return (m_rangeEffect);
	}

	void OSPFAreaRangeMessage::setRangeCost(const UI32 &cost)
	{
		m_rangeCost = cost;
	}

	UI32  OSPFAreaRangeMessage::getRangeCost()  const
	{
		return (m_rangeCost);
	}

	void OSPFAreaRangeMessage::setVRF(const string &vrf)
	{
		m_vrf = vrf;
	}

	string OSPFAreaRangeMessage::getVRF() const
	{
		return (m_vrf);
	}

    void OSPFAreaRangeMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

	// Convert info from C++ struct OSPFAreaRangeMessage populated in the CPWorker to a C struct for backend
    const void *OSPFAreaRangeMessage::getCStructureForInputs ()
    {
        ospf_area_config_in_msg_t *msg = (ospf_area_config_in_msg_t *) calloc (1, sizeof (ospf_area_config_in_msg_t));
		string ipstr;

		msg->cmdCode = (ospf_area_config_cmd_code_t)m_cmdCode;
		msg->opCode  = m_opCode;

		if (m_areaId.length() < 16) {
			strncpy(msg->area_id, m_areaId.c_str(), m_areaId.length());
			msg->area_id[m_areaId.length()] = '\0';
		}

		ipstr = m_ipAddr.toString();
		strncpy(msg->range_ip, ipstr.c_str(), ipstr.length());
		msg->range_ip[ipstr.length()] = '\0';

		ipstr = m_ipMask.toString();
		strncpy(msg->range_mask, ipstr.c_str(), ipstr.length());
		msg->range_mask[ipstr.length()] = '\0';

		msg->range_effect = m_rangeEffect;
		msg->range_cost = m_rangeCost;

		strcpy(msg->m_vrf ,m_vrf.c_str());

        return msg;
    }

	// Add C struct returned from backend into buffer so CPWorker can process it
	void OSPFAreaRangeMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}

