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

#include "ClientInterface/Ssm/QosClientDeleteDscpToDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"


namespace DcmNs
{

    QosClientDeleteDscpToDscpMutationMapMessage::QosClientDeleteDscpToDscpMutationMapMessage ()
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEDSCPTODSCPMUTATIONMAP)
    {
    }

    QosClientDeleteDscpToDscpMutationMapMessage::QosClientDeleteDscpToDscpMutationMapMessage (const string &dscpMutationMapName)
        : DcmManagementInterfaceMessage(QOSCLIENTDELETEDSCPTODSCPMUTATIONMAP),
        m_dscpMutationMapName    (dscpMutationMapName)
    {
    }

    QosClientDeleteDscpToDscpMutationMapMessage::~QosClientDeleteDscpToDscpMutationMapMessage ()
    {
    }

    void  QosClientDeleteDscpToDscpMutationMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_dscpMutationMapName,"dscpMutationMapName"));
    }

    void  QosClientDeleteDscpToDscpMutationMapMessage::setDscpMutationMapName(const string &dscpMutationMapName)
    {
        m_dscpMutationMapName  =  dscpMutationMapName;
    }

    string  QosClientDeleteDscpToDscpMutationMapMessage::getDscpMutationMapName() const
    {
        return (m_dscpMutationMapName);
    }

    const void *QosClientDeleteDscpToDscpMutationMapMessage::getCStructureForInputs ()
    {
		QosDscpMutationMapMsg_t *ptr;
        char *pInput = (char*)malloc(sizeof(ptr->dscpMutationMapName));
        strncpy (pInput, m_dscpMutationMapName.c_str (), sizeof(ptr->dscpMutationMapName) - 1);
		pInput[sizeof(ptr->dscpMutationMapName) - 1] = '\0';
        return (pInput);
    }

    void QosClientDeleteDscpToDscpMutationMapMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}
