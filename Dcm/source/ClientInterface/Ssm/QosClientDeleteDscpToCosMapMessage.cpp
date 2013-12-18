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

#include "ClientInterface/Ssm/QosClientDeleteDscpToCosMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"


namespace DcmNs
{

    QosClientDeleteDscpToCosMapMessage::QosClientDeleteDscpToCosMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEDSCPTOCOSMAP)
    {
    }

    QosClientDeleteDscpToCosMapMessage::QosClientDeleteDscpToCosMapMessage (const string &dscpCosMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEDSCPTOCOSMAP),
        m_dscpCosMapName    (dscpCosMapName)
    {
    }

    QosClientDeleteDscpToCosMapMessage::~QosClientDeleteDscpToCosMapMessage ()
    {
    }

    void  QosClientDeleteDscpToCosMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpCosMapName,"dscpCosMapName"));
    }

    void  QosClientDeleteDscpToCosMapMessage::setDscpCosMapName(const string &dscpCosMapName)
    {
        m_dscpCosMapName  =  dscpCosMapName;
    }

    string  QosClientDeleteDscpToCosMapMessage::getDscpCosMapName() const
    {
        return (m_dscpCosMapName);
    }

    const void *QosClientDeleteDscpToCosMapMessage::getCStructureForInputs ()
    {
		QosDscpCosMapMsg_t *ptr;
        char *pInput = (char*)malloc(sizeof(ptr->dscpCosMapName));
        strncpy (pInput, m_dscpCosMapName.c_str (), sizeof(ptr->dscpCosMapName) - 1);
		pInput[sizeof(ptr->dscpCosMapName) - 1] = '\0';
        return (pInput);
    }

    void QosClientDeleteDscpToCosMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
