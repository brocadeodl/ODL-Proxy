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
 *   Author : nbellari                                                     *
 **************************************************************************/

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosCreateRedProfileSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosCreateRedProfileSPMessage::QosCreateRedProfileSPMessage ()
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSCREATEREDPROFILESP)
    {
    }

    QosCreateRedProfileSPMessage::QosCreateRedProfileSPMessage (const SI32 &profileId,const SI32 &minThreshold,const SI32 &maxThreshold,const SI32 &dropProbability)
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSCREATEREDPROFILESP),
        m_profileId    (profileId),
        m_minThreshold    (minThreshold),
        m_maxThreshold    (maxThreshold),
        m_dropProbability    (dropProbability)
    {
    }

    QosCreateRedProfileSPMessage::~QosCreateRedProfileSPMessage ()
    {
    }

    void  QosCreateRedProfileSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeSI32(&m_minThreshold,"minThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_maxThreshold,"maxThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_dropProbability,"dropProbability"));
    }

    void  QosCreateRedProfileSPMessage::setProfileId(const SI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    SI32  QosCreateRedProfileSPMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  QosCreateRedProfileSPMessage::setMinThreshold(const SI32 &minThreshold)
    {
        m_minThreshold  =  minThreshold;
    }

    SI32  QosCreateRedProfileSPMessage::getMinThreshold() const
    {
        return (m_minThreshold);
    }

    void  QosCreateRedProfileSPMessage::setMaxThreshold(const SI32 &maxThreshold)
    {
        m_maxThreshold  =  maxThreshold;
    }

    SI32  QosCreateRedProfileSPMessage::getMaxThreshold() const
    {
        return (m_maxThreshold);
    }

    void  QosCreateRedProfileSPMessage::setDropProbability(const SI32 &dropProbability)
    {
        m_dropProbability  =  dropProbability;
    }

    SI32  QosCreateRedProfileSPMessage::getDropProbability() const
    {
        return (m_dropProbability);
    }

}
