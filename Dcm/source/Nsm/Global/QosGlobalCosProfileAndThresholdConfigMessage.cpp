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
 *   Author : Sudha Balodia                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/QosGlobalCosProfileAndThresholdConfigMessage.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    QosGlobalCosProfileAndThresholdConfigMessage::QosGlobalCosProfileAndThresholdConfigMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCOSPROFILEANDTHRESHOLDCONFIG)
    {
		m_opCode = 0;
		m_cmdCode = 0;
		m_cos = 0;
		m_redProfile = 0;
		m_tailDrop = 0;
		m_poId = 0;
    }

    QosGlobalCosProfileAndThresholdConfigMessage::QosGlobalCosProfileAndThresholdConfigMessage (const UI32 &cos,const UI32 &red_profile,const UI32 &tail_drop)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCOSPROFILEANDTHRESHOLDCONFIG),
        m_cos    (cos),
        m_redProfile    (red_profile),
        m_tailDrop    (tail_drop)
    {
		m_opCode = 0;
		m_cmdCode = 0;
		m_poId = 0;
    }

    QosGlobalCosProfileAndThresholdConfigMessage::~QosGlobalCosProfileAndThresholdConfigMessage ()
    {
    }

    void  QosGlobalCosProfileAndThresholdConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_cos,"cos"));
        addSerializableAttribute (new AttributeUI32(&m_redProfile,"redProfile"));
        addSerializableAttribute (new AttributeUI32(&m_tailDrop,"tailDrop"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_opCode,"opCode"));
    }

    void  QosGlobalCosProfileAndThresholdConfigMessage::setCos(const UI32 &cos)
    {
        m_cos  =  cos;
    }

    UI32  QosGlobalCosProfileAndThresholdConfigMessage::getCos() const
    {
        return (m_cos);
    }

    void  QosGlobalCosProfileAndThresholdConfigMessage::setRedProfile(const UI32 &red_profile)
    {
        m_redProfile  =  red_profile;
    }

    UI32  QosGlobalCosProfileAndThresholdConfigMessage::getRedProfile() const
    {
        return (m_redProfile);
    }

    void  QosGlobalCosProfileAndThresholdConfigMessage::setTailDrop(const UI32 &tail_drop)
    {
        m_tailDrop  =  tail_drop;
    }

    UI32  QosGlobalCosProfileAndThresholdConfigMessage::getTailDrop() const
    {
        return (m_tailDrop);
    }

    void  QosGlobalCosProfileAndThresholdConfigMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  QosGlobalCosProfileAndThresholdConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  QosGlobalCosProfileAndThresholdConfigMessage::setOpCode(const SI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    SI32  QosGlobalCosProfileAndThresholdConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

	 void  QosGlobalCosProfileAndThresholdConfigMessage::setPoId(const UI32 &poId)
    {
        m_poId =  poId;
    }

    UI32  QosGlobalCosProfileAndThresholdConfigMessage::getPoId() const
    {
        return (m_poId);
    }


}
