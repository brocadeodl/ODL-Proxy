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
#include "Qos/Local/QosCreatePriorityGroupSPMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosCreatePriorityGroupSPMessage::QosCreatePriorityGroupSPMessage ()
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (),QOSCREATEPRIORITYGROUPSP)
    {
    }

    QosCreatePriorityGroupSPMessage::QosCreatePriorityGroupSPMessage (const string &ceeMapName,const string &pgId,const SI32 &weight,const SI32 &pfc)
        : PrismMessage (QosLocalObjectManager::getPrismServiceId (),QOSCREATEPRIORITYGROUPSP),
        m_ceeMapName    (ceeMapName),
        m_pgId    (pgId),
        m_weight    (weight),
        m_pfc    (pfc)
    {
    }

    QosCreatePriorityGroupSPMessage::~QosCreatePriorityGroupSPMessage ()
    {
    }

    void  QosCreatePriorityGroupSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeString(&m_pgId,"pgId"));
        addSerializableAttribute (new AttributeSI32(&m_weight,"weight"));
        addSerializableAttribute (new AttributeSI32(&m_pfc,"pfc"));
    }

    void  QosCreatePriorityGroupSPMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosCreatePriorityGroupSPMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosCreatePriorityGroupSPMessage::setPgId(const string &pgId)
    {
        m_pgId  =  pgId;
    }

    string  QosCreatePriorityGroupSPMessage::getPgId() const
    {
        return (m_pgId);
    }

    void  QosCreatePriorityGroupSPMessage::setWeight(const SI32 &weight)
    {
        m_weight  =  weight;
    }

    SI32  QosCreatePriorityGroupSPMessage::getWeight() const
    {
        return (m_weight);
    }

    void  QosCreatePriorityGroupSPMessage::setPfc(const SI32 &pfc)
    {
        m_pfc  =  pfc;
    }

    SI32  QosCreatePriorityGroupSPMessage::getPfc() const
    {
        return (m_pfc);
    }

}
