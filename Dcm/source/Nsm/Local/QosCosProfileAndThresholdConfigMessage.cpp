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
 *   Author : Sudha Balodia                                                *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/QosCosProfileAndThresholdConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    QosCosProfileAndThresholdConfigMessage::QosCosProfileAndThresholdConfigMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),QOSCOSPROFILEANDTHRESHOLDCONFIG) {
		m_OpCode = 0;
		m_cos = 0;
		m_cmdCode = 0;
		m_redProfile = 0;
		m_tailDrop = 0;
    }

    QosCosProfileAndThresholdConfigMessage::QosCosProfileAndThresholdConfigMessage (const UI32 &cos,const UI32 &redProfile, const UI32 &tailDrop)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),QOSCOSPROFILEANDTHRESHOLDCONFIG),
        m_cos    (cos),
        m_redProfile    (redProfile),
        m_tailDrop    (tailDrop) {
		m_OpCode = 0;
		m_cmdCode = 0;
    }

    QosCosProfileAndThresholdConfigMessage::~QosCosProfileAndThresholdConfigMessage ()
    {
    }

    void  QosCosProfileAndThresholdConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_cos,"cos"));
        addSerializableAttribute (new AttributeUI32(&m_redProfile,"redProfile"));
        addSerializableAttribute (new AttributeUI32(&m_tailDrop,"tailDropThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_OpCode,"OpCode"));
        addSerializableAttribute (new AttributeSI32(&m_ifType, "ifType"));
    }

    void  QosCosProfileAndThresholdConfigMessage::setCos(const UI32 &cos)
    {
        m_cos  = cos;
    }

    UI32  QosCosProfileAndThresholdConfigMessage::getCos() const
    {
        return (m_cos);
    }

    void  QosCosProfileAndThresholdConfigMessage::setRedProfile(const UI32 &redProfile)
    {
        m_redProfile  = redProfile;
    }

    UI32  QosCosProfileAndThresholdConfigMessage::getRedProfile() const
    {
        return (m_redProfile);
    }

    void  QosCosProfileAndThresholdConfigMessage::setTailDrop(const UI32 &tailDrop)
    {
        m_tailDrop  =  tailDrop;
    }

    UI32  QosCosProfileAndThresholdConfigMessage::getTailDrop() const
    {
        return (m_tailDrop);
    }

    void  QosCosProfileAndThresholdConfigMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  QosCosProfileAndThresholdConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  QosCosProfileAndThresholdConfigMessage::setOpCode(const SI32 &OpCode)
    {
        m_OpCode  =  OpCode;
    }

    SI32  QosCosProfileAndThresholdConfigMessage::getOpCode() const
    {
        return (m_OpCode);
    }

    void  QosCosProfileAndThresholdConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  QosCosProfileAndThresholdConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

	void QosCosProfileAndThresholdConfigMessage::setIfType(const SI32 &ifType)
	{
		m_ifType = ifType;
	}

	SI32 QosCosProfileAndThresholdConfigMessage::getIfType() const
	{
		return (m_ifType);
	}
}
