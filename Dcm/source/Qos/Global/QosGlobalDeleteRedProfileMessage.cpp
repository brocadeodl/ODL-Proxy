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

#include "Qos/Global/QosGlobalDeleteRedProfileMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalDeleteRedProfileMessage::QosGlobalDeleteRedProfileMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALDELETEREDPROFILE)
    {
    }

    QosGlobalDeleteRedProfileMessage::QosGlobalDeleteRedProfileMessage (const SI32 &profileId)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALDELETEREDPROFILE),
        m_profileId    (profileId)
    {
    }

    QosGlobalDeleteRedProfileMessage::~QosGlobalDeleteRedProfileMessage ()
    {
    }

    void  QosGlobalDeleteRedProfileMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_profileId,"profileId"));
    }

    void  QosGlobalDeleteRedProfileMessage::setProfileId(const SI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    SI32  QosGlobalDeleteRedProfileMessage::getProfileId() const
    {
        return (m_profileId);
    }

}
