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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmQosCosTailDropConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmQosCosTailDropConfigMessage::NsmQosCosTailDropConfigMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMQOSCOSTHRESHOLDCONFIG)
    {
    }

    NsmQosCosTailDropConfigMessage::NsmQosCosTailDropConfigMessage (const UI32 &cos0Th, const UI32 &cos1Th, const UI32 &cos2Th, const UI32 &cos3Th, const UI32 &cos4Th, const UI32 &cos5Th, const UI32 &cos6Th, const UI32 &cos7Th)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMQOSCOSTHRESHOLDCONFIG),
		m_cos0_threshold (cos0Th),
		m_cos1_threshold (cos1Th),
		m_cos2_threshold (cos2Th),
		m_cos3_threshold (cos3Th),
		m_cos4_threshold (cos4Th),
		m_cos5_threshold (cos5Th),
		m_cos6_threshold (cos6Th),
		m_cos7_threshold (cos7Th)
    {
		m_OpCode = 0;
		m_cmdCode = 0;
    }

    NsmQosCosTailDropConfigMessage::~NsmQosCosTailDropConfigMessage ()
    {
    }

    void  NsmQosCosTailDropConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_OpCode,"OpCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
		addSerializableAttribute (new AttributeUI32(&m_cos0_threshold, "cos0_threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos1_threshold, "cos1_threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos2_threshold, "cos2_threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos3_threshold, "cos3_threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos4_threshold, "cos4_threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos5_threshold, "cos5_threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos6_threshold, "cos6_threshold"));
		addSerializableAttribute (new AttributeUI32(&m_cos7_threshold, "cos7_threshold"));
    }

    void  NsmQosCosTailDropConfigMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmQosCosTailDropConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmQosCosTailDropConfigMessage::setOpCode(const SI32 &OpCode)
    {
        m_OpCode  =  OpCode;
    }

    SI32  NsmQosCosTailDropConfigMessage::getOpCode() const
    {
        return (m_OpCode);
    }

	void NsmQosCosTailDropConfigMessage::setIfType(const UI32 &ifType)
	{
		m_ifType = ifType;
	}
	
	UI32 NsmQosCosTailDropConfigMessage::getIfType() const
	{
		return (m_ifType);
	}
	
    void  NsmQosCosTailDropConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmQosCosTailDropConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

	void NsmQosCosTailDropConfigMessage::setCos0Threshold(const UI32 &cos0Th)
	{
		m_cos0_threshold = cos0Th;
	}
	UI32 NsmQosCosTailDropConfigMessage::getCos0Threshold() const
	{
		return (m_cos0_threshold);
	}
	void NsmQosCosTailDropConfigMessage::setCos1Threshold(const UI32 &cos1Th)
	{
		m_cos1_threshold = cos1Th;
	}
	UI32 NsmQosCosTailDropConfigMessage::getCos1Threshold() const
	{
		return (m_cos1_threshold);
	}
	void NsmQosCosTailDropConfigMessage::setCos2Threshold(const UI32 &cos2Th)
	{
		m_cos2_threshold = cos2Th;
	}
	UI32 NsmQosCosTailDropConfigMessage::getCos2Threshold() const
	{
		return (m_cos2_threshold);
	}
	void NsmQosCosTailDropConfigMessage::setCos3Threshold(const UI32 &cos3Th)
	{
		m_cos3_threshold = cos3Th;
	}
	UI32 NsmQosCosTailDropConfigMessage::getCos3Threshold() const
	{
		return (m_cos3_threshold);
	}
	void NsmQosCosTailDropConfigMessage::setCos4Threshold(const UI32 &cos4Th)
	{
		m_cos4_threshold = cos4Th;
	}
	UI32 NsmQosCosTailDropConfigMessage::getCos4Threshold() const
	{
		return (m_cos4_threshold);
	}
	void NsmQosCosTailDropConfigMessage::setCos5Threshold(const UI32 &cos5Th)
	{
		m_cos5_threshold = cos5Th;
	}
	UI32 NsmQosCosTailDropConfigMessage::getCos5Threshold() const
	{
		return (m_cos5_threshold);
	}
	void NsmQosCosTailDropConfigMessage::setCos6Threshold(const UI32 &cos6Th)
	{
		m_cos6_threshold = cos6Th;
	}
	UI32 NsmQosCosTailDropConfigMessage::getCos6Threshold() const
	{
		return (m_cos6_threshold);
	}
	void NsmQosCosTailDropConfigMessage::setCos7Threshold(const UI32 &cos7Th)
	{
		m_cos7_threshold = cos7Th;
	}
	UI32 NsmQosCosTailDropConfigMessage::getCos7Threshold() const
	{
		return (m_cos7_threshold);
	}
}
