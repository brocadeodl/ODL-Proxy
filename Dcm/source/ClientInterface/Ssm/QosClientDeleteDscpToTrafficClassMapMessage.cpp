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

#include "ClientInterface/Ssm/QosClientDeleteDscpToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"


namespace DcmNs
{

    QosClientDeleteDscpToTrafficClassMapMessage::QosClientDeleteDscpToTrafficClassMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEDSCPTOTRAFFICCLASSMAP)
    {
    }

    QosClientDeleteDscpToTrafficClassMapMessage::QosClientDeleteDscpToTrafficClassMapMessage (const string &dscpTrafficClassMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEDSCPTOTRAFFICCLASSMAP),
        m_dscpTrafficClassMapName    (dscpTrafficClassMapName)
    {
    }

    QosClientDeleteDscpToTrafficClassMapMessage::~QosClientDeleteDscpToTrafficClassMapMessage ()
    {
    }

    void  QosClientDeleteDscpToTrafficClassMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpTrafficClassMapName,"dscpTrafficClassMapName"));
    }

    void  QosClientDeleteDscpToTrafficClassMapMessage::setDscpTrafficClassMapName(const string &dscpTrafficClassMapName)
    {
        m_dscpTrafficClassMapName  =  dscpTrafficClassMapName;
    }

    string  QosClientDeleteDscpToTrafficClassMapMessage::getDscpTrafficClassMapName() const
    {
        return (m_dscpTrafficClassMapName);
    }

    const void *QosClientDeleteDscpToTrafficClassMapMessage::getCStructureForInputs ()
    {
		QosDscpTrafficClassMapMsg_t *ptr;
        char *pInput = (char*)malloc(sizeof(ptr->dscpTrafficClassMapName));
        strncpy (pInput, m_dscpTrafficClassMapName.c_str (), sizeof(ptr->dscpTrafficClassMapName) - 1);
		pInput[sizeof(ptr->dscpTrafficClassMapName) - 1] = '\0';
        return (pInput);
    }

    void QosClientDeleteDscpToTrafficClassMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
