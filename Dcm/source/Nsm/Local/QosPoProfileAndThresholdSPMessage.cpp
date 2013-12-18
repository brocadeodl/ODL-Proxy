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
#include "Nsm/Local/QosPoProfileAndThresholdSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    QosPoProfileAndThresholdSPMessage::QosPoProfileAndThresholdSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),QOSPOPROFILEANDTHRESHOLDSP)
    {
		m_OpCode = 0;
		m_cmdCode = 0;
		m_cos = 0;
		m_redProfile = 0;
		m_tailDrop = 0;
		m_poId = 0;
    }

    QosPoProfileAndThresholdSPMessage::QosPoProfileAndThresholdSPMessage (const UI32 &cos,const UI32 &red_profile,const UI32 &tail_drop)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),QOSPOPROFILEANDTHRESHOLDSP),
        m_cos    (cos),
        m_redProfile    (red_profile),
        m_tailDrop    (tail_drop)
    {
		m_poId = 0;
		m_OpCode = 0;
		m_cmdCode = 0;
    }

    QosPoProfileAndThresholdSPMessage::~QosPoProfileAndThresholdSPMessage ()
    {
    }

    void  QosPoProfileAndThresholdSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_cos,"cos"));
        addSerializableAttribute (new AttributeUI32(&m_redProfile,"redProfile"));
        addSerializableAttribute (new AttributeUI32(&m_tailDrop,"tailDrop"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_OpCode,"OpCode"));
    }

    void  QosPoProfileAndThresholdSPMessage::setCos(const UI32 &cos)
    {
        m_cos  =  cos;
    }

    UI32  QosPoProfileAndThresholdSPMessage::getCos() const
    {
        return (m_cos);
    }

    void  QosPoProfileAndThresholdSPMessage::setRedProfile(const UI32 &red_profile)
    {
        m_redProfile  =  red_profile;
    }

    UI32  QosPoProfileAndThresholdSPMessage::getRedProfile() const
    {
        return (m_redProfile);
    }

    void  QosPoProfileAndThresholdSPMessage::setTailDrop(const UI32 &tail_drop)
    {
        m_tailDrop  =  tail_drop;
    }

    UI32  QosPoProfileAndThresholdSPMessage::getTailDrop() const
    {
        return (m_tailDrop);
    }

    void  QosPoProfileAndThresholdSPMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  QosPoProfileAndThresholdSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  QosPoProfileAndThresholdSPMessage::setOpCode(const SI32 &OpCode)
    {
        m_OpCode  =  OpCode;
    }

    SI32  QosPoProfileAndThresholdSPMessage::getOpCode() const
    {
        return (m_OpCode);
    }

	 void  QosPoProfileAndThresholdSPMessage::setPoId(const UI32 &poId)
    {
        m_poId =  poId;
    }

    UI32  QosPoProfileAndThresholdSPMessage::getPoId() const
    {
        return (m_poId);
    }


}
