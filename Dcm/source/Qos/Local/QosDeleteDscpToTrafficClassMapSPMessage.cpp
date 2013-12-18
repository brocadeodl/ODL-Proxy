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
#include "Qos/Local/QosDeleteDscpToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosDeleteDscpToTrafficClassMapSPMessage::QosDeleteDscpToTrafficClassMapSPMessage ()
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSDELETEDSCPTOTRAFFICCLASSMAPSP)
    {
    }

    QosDeleteDscpToTrafficClassMapSPMessage::QosDeleteDscpToTrafficClassMapSPMessage (const string &dscpToTrafficClassMapName)
        : PrismMessage(QosLocalObjectManager::getPrismServiceId(), QOSDELETEDSCPTOTRAFFICCLASSMAPSP),
        m_dscpToTrafficClassMapName    (dscpToTrafficClassMapName)
    {
    }

    QosDeleteDscpToTrafficClassMapSPMessage::~QosDeleteDscpToTrafficClassMapSPMessage ()
    {
    }

    void  QosDeleteDscpToTrafficClassMapSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpToTrafficClassMapName,"dscpToTrafficClassMapName"));
    }

    void  QosDeleteDscpToTrafficClassMapSPMessage::setDscpTrafficClassMapName(const string &dscpToTrafficClassMapName)
    {
        m_dscpToTrafficClassMapName  =  dscpToTrafficClassMapName;
    }

    string  QosDeleteDscpToTrafficClassMapSPMessage::getDscpTrafficClassMapName() const
    {
        return (m_dscpToTrafficClassMapName);
    }

}
