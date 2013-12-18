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

#include "ClientInterface/Ssm/QosClientDeletePriorityGroupMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientDeletePriorityGroupMessage::QosClientDeletePriorityGroupMessage ()
        : DcmManagementInterfaceMessage (QOSCLIENTDELETEPRIORITYGROUP)
    {
    }

    QosClientDeletePriorityGroupMessage::QosClientDeletePriorityGroupMessage (const string &ceeMapName,const string &pgId)
        : DcmManagementInterfaceMessage (QOSCLIENTDELETEPRIORITYGROUP),
        m_ceeMapName    (ceeMapName),
        m_pgId    (pgId)
    {
    }

    QosClientDeletePriorityGroupMessage::~QosClientDeletePriorityGroupMessage ()
    {
    }

    void  QosClientDeletePriorityGroupMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeString(&m_pgId,"pgId"));
    }

    void  QosClientDeletePriorityGroupMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosClientDeletePriorityGroupMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosClientDeletePriorityGroupMessage::setPgId(const string &pgId)
    {
        m_pgId  =  pgId;
    }

    string  QosClientDeletePriorityGroupMessage::getPgId() const
    {
        return (m_pgId);
    }

    const void *QosClientDeletePriorityGroupMessage::getCStructureForInputs ()
    {
        QosPriorityGroupMsg_t *pInput = (QosPriorityGroupMsg_t *)calloc (1, sizeof (QosPriorityGroupMsg_t));

        if ( NULL == pInput ) {
            return pInput;
        }
        strncpy (pInput->ceeMapName, m_ceeMapName.c_str (), MAP_NAME_MAX_STR-1);
        pInput->ceeMapName[MAP_NAME_MAX_STR-1] = '\0';
        strncpy (pInput->pgid, m_pgId.c_str (), 8);
		pInput->pgid[7] = '\0';
        pInput->weight = 0;
        pInput->pfc = false;

        return (pInput);
    }

    void QosClientDeletePriorityGroupMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }

}
