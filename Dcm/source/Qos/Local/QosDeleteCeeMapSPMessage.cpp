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

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosDeleteCeeMapSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosDeleteCeeMapSPMessage::QosDeleteCeeMapSPMessage ()
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (),QOSDELETECEEMAPSP)
    {
    }

    QosDeleteCeeMapSPMessage::QosDeleteCeeMapSPMessage (const string &ceeMapName)
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (),QOSDELETECEEMAPSP),
        m_ceeMapName    (ceeMapName)
    {
    }

    QosDeleteCeeMapSPMessage::~QosDeleteCeeMapSPMessage ()
    {
    }

    void  QosDeleteCeeMapSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
    }

    void  QosDeleteCeeMapSPMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosDeleteCeeMapSPMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

}
