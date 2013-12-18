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

#include "ClientInterface/Ssm/QosClientCreatePriorityGroupMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

namespace DcmNs
{

    QosClientCreatePriorityGroupMessage::QosClientCreatePriorityGroupMessage ()
        : DcmManagementInterfaceMessage (QOSCLIENTCREATEPRIORITYGROUP)
    {
    }

    QosClientCreatePriorityGroupMessage::QosClientCreatePriorityGroupMessage (const string &ceeMapName,const string &pgId,const SI32 &weight,const SI32 &pfc)
        : DcmManagementInterfaceMessage (QOSCLIENTCREATEPRIORITYGROUP),
        m_ceeMapName    (ceeMapName),
        m_pgId    (pgId),
        m_weight    (weight),
        m_pfc    (pfc)
    {
    }

    QosClientCreatePriorityGroupMessage::~QosClientCreatePriorityGroupMessage ()
    {
    }

    void  QosClientCreatePriorityGroupMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeString(&m_pgId,"pgId"));
        addSerializableAttribute (new AttributeSI32(&m_weight,"weight"));
        addSerializableAttribute (new AttributeSI32(&m_pfc,"pfc"));
        addSerializableAttribute (new AttributeSI32(&m_configReplay,"configReplay"));
    }

    void  QosClientCreatePriorityGroupMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosClientCreatePriorityGroupMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosClientCreatePriorityGroupMessage::setPgId(const string &pgId)
    {
        m_pgId  =  pgId;
    }

    string  QosClientCreatePriorityGroupMessage::getPgId() const
    {
        return (m_pgId);
    }

    void  QosClientCreatePriorityGroupMessage::setWeight(const SI32 &weight)
    {
        m_weight  =  weight;
    }

    SI32  QosClientCreatePriorityGroupMessage::getWeight() const
    {
        return (m_weight);
    }

    void  QosClientCreatePriorityGroupMessage::setPfc(const SI32 &pfc)
    {
        m_pfc  =  pfc;
    }

    SI32  QosClientCreatePriorityGroupMessage::getPfc() const
    {
        return (m_pfc);
    }

    void  QosClientCreatePriorityGroupMessage::setConfigReplay(const SI32 &configReplay)
    {
        m_configReplay = configReplay;
    }

    SI32 QosClientCreatePriorityGroupMessage::getConfigReplay() const
    {
        return (m_configReplay);
    }

    const void *QosClientCreatePriorityGroupMessage::getCStructureForInputs ()
    {
        QosPriorityGroupMsg_t *pInput = (QosPriorityGroupMsg_t *)calloc (1, sizeof (QosPriorityGroupMsg_t));

        if ( NULL == pInput )  {
            return pInput;
        }

        strncpy (pInput->ceeMapName, m_ceeMapName.c_str (), MAP_NAME_MAX_STR-1);
        pInput->ceeMapName[MAP_NAME_MAX_STR-1] = '\0';
		strncpy (pInput->pgid, m_pgId.c_str (), 8);
        pInput->weight = m_weight;
        pInput->pfc = m_pfc;
        pInput->configReplay = (int)m_configReplay;

        return (pInput);
    }

    void QosClientCreatePriorityGroupMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }
}

