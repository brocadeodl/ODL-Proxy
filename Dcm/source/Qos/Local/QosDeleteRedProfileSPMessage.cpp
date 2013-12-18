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
#include "Qos/Local/QosDeleteRedProfileSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosDeleteRedProfileSPMessage::QosDeleteRedProfileSPMessage ()
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSDELETEREDPROFILESP)
    {
    }

    QosDeleteRedProfileSPMessage::QosDeleteRedProfileSPMessage (const SI32 &profileId)
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSDELETEREDPROFILESP),
        m_profileId    (profileId)
    {
    }

    QosDeleteRedProfileSPMessage::~QosDeleteRedProfileSPMessage ()
    {
    }

    void  QosDeleteRedProfileSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_profileId,"profileId"));
    }

    void  QosDeleteRedProfileSPMessage::setProfileId(const SI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    SI32  QosDeleteRedProfileSPMessage::getProfileId() const
    {
        return (m_profileId);
    }

}
