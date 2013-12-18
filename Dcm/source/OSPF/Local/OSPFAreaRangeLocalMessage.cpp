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
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "OSPF/Local/OSPFAreaRangeLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    OSPFAreaRangeLocalMessage::OSPFAreaRangeLocalMessage ()
        : ManagementInterfaceMessage (OSPFLocalObjectManager::getClassName(), OSPFAREARANGELOCAL)
    {
    }


    OSPFAreaRangeLocalMessage::~OSPFAreaRangeLocalMessage ()
    {
    }

    void  OSPFAreaRangeLocalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_opCode, "opCode"));
        addSerializableAttribute (new AttributeString(&m_areaId,"areaId"));
        addSerializableAttribute (new AttributeIpV4Address(&m_ipAddr,"ipAddr"));
        addSerializableAttribute (new AttributeIpV4Address(&m_ipMask,"ipMask"));
        addSerializableAttribute (new AttributeUI32(&m_rangeEffect,"rangeEffect"));
        addSerializableAttribute (new AttributeUI32(&m_rangeCost,"rangeCost"));
    }

	void OSPFAreaRangeLocalMessage::setCmdCode(unsigned int code)
	{
		m_cmdCode = code;
	}

	unsigned int OSPFAreaRangeLocalMessage::getCmdCode()
	{
		return (m_cmdCode);
	}

	void OSPFAreaRangeLocalMessage::setOpCode(const OSPFOpCode &opCode)
	{
		m_opCode = opCode;
	}

	OSPFOpCode OSPFAreaRangeLocalMessage::getOpCode() const
	{
		return (m_opCode);
	}


    void  OSPFAreaRangeLocalMessage::setAreaId(const string &areaId)
    {
        m_areaId  =  areaId;
    }

    string  OSPFAreaRangeLocalMessage::getAreaId() const
    {
        return (m_areaId);
    }

	void  OSPFAreaRangeLocalMessage::setIpAddr(const IpV4Address &ip)
	{
		m_ipAddr = ip;
	}

	IpV4Address  OSPFAreaRangeLocalMessage::getIpAddr()  const
	{
		return (m_ipAddr);
	}

	void  OSPFAreaRangeLocalMessage::setIpMask(const IpV4Address &ip)
	{
		m_ipMask = ip;
	}

	IpV4Address  OSPFAreaRangeLocalMessage::getIpMask()  const
	{
		return (m_ipMask);
	}

	void OSPFAreaRangeLocalMessage::setRangeEffect(const UI32 &effect)
	{
		m_rangeEffect = effect;
	}

	UI32  OSPFAreaRangeLocalMessage::getRangeEffect()  const
	{
		return (m_rangeEffect);
	}
	
	void OSPFAreaRangeLocalMessage::setRangeCost(const UI32 &cost)
	{
		m_rangeCost = cost;
	}

	UI32  OSPFAreaRangeLocalMessage::getRangeCost()  const
	{
		return (m_rangeCost);
	}

	void OSPFAreaRangeLocalMessage::setRangeCostModified(const bool &flag)
	{
		m_rangeCostModified = flag;
	}

	bool  OSPFAreaRangeLocalMessage::isRangeCostModified()  const
	{
		return (m_rangeCostModified);
	}

	void OSPFAreaRangeLocalMessage::setRangeEffectModified(const bool &flag)
	{
		m_rangeEffectModified = flag;
	}

	bool  OSPFAreaRangeLocalMessage::isRangeEffectModified()  const
	{
		return (m_rangeEffectModified);
	}

	void 
    OSPFAreaRangeLocalMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		return;
	}
}
