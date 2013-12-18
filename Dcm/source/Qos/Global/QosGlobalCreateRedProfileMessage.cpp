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

#include "Qos/Global/QosGlobalCreateRedProfileMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalCreateRedProfileMessage::QosGlobalCreateRedProfileMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATEREDPROFILE)
    {
    }

    QosGlobalCreateRedProfileMessage::QosGlobalCreateRedProfileMessage (const SI32 &profileId,const SI32 &minThreshold,const SI32 &maxThreshold,const SI32 &dropProbability)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATEREDPROFILE),
        m_profileId    (profileId),
        m_minThreshold    (minThreshold),
        m_maxThreshold    (maxThreshold),
        m_dropProbability    (dropProbability)
    {
    }

    QosGlobalCreateRedProfileMessage::~QosGlobalCreateRedProfileMessage ()
    {
    }

    void  QosGlobalCreateRedProfileMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeSI32(&m_minThreshold,"minThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_maxThreshold,"maxThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_dropProbability,"dropProbability"));
    }

    void  QosGlobalCreateRedProfileMessage::setProfileId(const SI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    SI32  QosGlobalCreateRedProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  QosGlobalCreateRedProfileMessage::setMinThreshold(const SI32 &minThreshold)
    {
        m_minThreshold  =  minThreshold;
    }

    SI32  QosGlobalCreateRedProfileMessage::getMinThreshold() const
    {
        return (m_minThreshold);
    }

    void  QosGlobalCreateRedProfileMessage::setMaxThreshold(const SI32 &maxThreshold)
    {
        m_maxThreshold  =  maxThreshold;
    }

    SI32  QosGlobalCreateRedProfileMessage::getMaxThreshold() const
    {
        return (m_maxThreshold);
    }

    void  QosGlobalCreateRedProfileMessage::setDropProbability(const SI32 &dropProbability)
    {
        m_dropProbability  =  dropProbability;
    }

    SI32  QosGlobalCreateRedProfileMessage::getDropProbability() const
    {
        return (m_dropProbability);
    }

}
