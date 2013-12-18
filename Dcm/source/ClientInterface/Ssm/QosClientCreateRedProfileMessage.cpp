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

#include "ClientInterface/Ssm/QosClientCreateRedProfileMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreateRedProfileMessage::QosClientCreateRedProfileMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEREDPROFILE)
    {
    }

    QosClientCreateRedProfileMessage::QosClientCreateRedProfileMessage (const SI32 &profileId, const SI32 &minThreshold, const SI32 &maxThreshold, const SI32 &dropProbability)
        : DcmManagementInterfaceMessage(QOSCLIENTCREATEREDPROFILE),
		m_profileId (profileId),
		m_minThreshold (minThreshold),
		m_maxThreshold (maxThreshold),
		m_dropProbability (dropProbability)
    {
    }

    QosClientCreateRedProfileMessage::~QosClientCreateRedProfileMessage ()
    {
    }

    void  QosClientCreateRedProfileMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeSI32(&m_minThreshold,"minThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_maxThreshold,"maxThreshold"));
        addSerializableAttribute (new AttributeSI32(&m_dropProbability,"dropProbability"));
    }

    void  QosClientCreateRedProfileMessage::setProfileId(const SI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    SI32  QosClientCreateRedProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  QosClientCreateRedProfileMessage::setMinThreshold(const SI32 &minThreshold)
    {
        m_minThreshold  =  minThreshold;
    }

    SI32  QosClientCreateRedProfileMessage::getMinThreshold() const
    {
        return (m_minThreshold);
    }

    void  QosClientCreateRedProfileMessage::setMaxThreshold(const SI32 &maxThreshold)
    {
        m_maxThreshold  =  maxThreshold;
    }

    SI32  QosClientCreateRedProfileMessage::getMaxThreshold() const
    {
        return (m_maxThreshold);
    }

    void  QosClientCreateRedProfileMessage::setDropProbability(const SI32 &dropProbability)
    {
        m_dropProbability  =  dropProbability;
    }

    SI32  QosClientCreateRedProfileMessage::getDropProbability() const
    {
        return (m_dropProbability);
    }

    const void *QosClientCreateRedProfileMessage::getCStructureForInputs ()
    {
        QosRedProfileIdsMsg_t *pInput = (QosRedProfileIdsMsg_t *)malloc (sizeof (QosRedProfileIdsMsg_t));

		pInput->profileId = (int)m_profileId;
		pInput->minThreshold = (int)m_minThreshold;
		pInput->maxThreshold = (int)m_maxThreshold;
		pInput->dropProbability = (int)m_dropProbability;

        return (pInput);
    }

    void QosClientCreateRedProfileMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
