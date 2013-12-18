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

#include "ClientInterface/Ssm/QosClientDeleteRedProfileMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"


namespace DcmNs
{

    QosClientDeleteRedProfileMessage::QosClientDeleteRedProfileMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEREDPROFILE)
    {
    }

    QosClientDeleteRedProfileMessage::QosClientDeleteRedProfileMessage (const SI32 &profileId)
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEREDPROFILE),
        m_profileId (profileId)
    {
    }

    QosClientDeleteRedProfileMessage::~QosClientDeleteRedProfileMessage ()
    {
    }

    void  QosClientDeleteRedProfileMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_profileId,"profileId"));
    }

    void  QosClientDeleteRedProfileMessage::setProfileId(const SI32 &profileId)
    {
        m_profileId =  profileId;
    }

    SI32 QosClientDeleteRedProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

    const void *QosClientDeleteRedProfileMessage::getCStructureForInputs ()
    {
        SI32 *pInput = (SI32*)malloc(sizeof(SI32));
		*pInput = m_profileId;
        return (pInput);
    }

    void QosClientDeleteRedProfileMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
