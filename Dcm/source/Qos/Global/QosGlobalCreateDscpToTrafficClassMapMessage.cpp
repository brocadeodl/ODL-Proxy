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

#include "Qos/Global/QosGlobalCreateDscpToTrafficClassMapMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    QosGlobalCreateDscpToTrafficClassMapMessage::QosGlobalCreateDscpToTrafficClassMapMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATEDSCPTOTRAFFICCLASSMAP)
    {
    }

    QosGlobalCreateDscpToTrafficClassMapMessage::QosGlobalCreateDscpToTrafficClassMapMessage (const string &dscpToTrafficClassMapName)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),QOSGLOBALCREATEDSCPTOTRAFFICCLASSMAP),
        m_dscpToTrafficClassMapName    (dscpToTrafficClassMapName)
    {
    }

    QosGlobalCreateDscpToTrafficClassMapMessage::~QosGlobalCreateDscpToTrafficClassMapMessage ()
    {
    }

    void  QosGlobalCreateDscpToTrafficClassMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpToTrafficClassMapName,"dscpToTrafficClassMapName"));
    }

    void  QosGlobalCreateDscpToTrafficClassMapMessage::setDscpToTrafficClassMapName(const string &dscpToTrafficClassMapName)
    {
        m_dscpToTrafficClassMapName  =  dscpToTrafficClassMapName;
    }

    string  QosGlobalCreateDscpToTrafficClassMapMessage::getDscpToTrafficClassMapName() const
    {
        return (m_dscpToTrafficClassMapName);
    }

}
