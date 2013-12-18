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

#include "Qos/Global/CeeGlobalCreatePriorityGroupMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    CeeGlobalCreatePriorityGroupMessage::CeeGlobalCreatePriorityGroupMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALCREATEPRIORITYGROUP)
    {
    }

    CeeGlobalCreatePriorityGroupMessage::CeeGlobalCreatePriorityGroupMessage (const string &ceeMapName,const string &pgId,const SI32 &weight,const SI32 &pfc)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALCREATEPRIORITYGROUP),
        m_ceeMapName    (ceeMapName),
        m_pgId    (pgId),
        m_weight    (weight),
        m_pfc    (pfc)
    {
    }

    CeeGlobalCreatePriorityGroupMessage::~CeeGlobalCreatePriorityGroupMessage ()
    {
    }

    void  CeeGlobalCreatePriorityGroupMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeString(&m_pgId,"pgId"));
        addSerializableAttribute (new AttributeSI32(&m_weight,"weight"));
        addSerializableAttribute (new AttributeSI32(&m_pfc,"pfc"));
    }

    void  CeeGlobalCreatePriorityGroupMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  CeeGlobalCreatePriorityGroupMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  CeeGlobalCreatePriorityGroupMessage::setPgId(const string &pgId)
    {
        m_pgId  =  pgId;
    }

    string  CeeGlobalCreatePriorityGroupMessage::getPgId() const
    {
        return (m_pgId);
    }

    void  CeeGlobalCreatePriorityGroupMessage::setWeight(const SI32 &weight)
    {
        m_weight  =  weight;
    }

    SI32  CeeGlobalCreatePriorityGroupMessage::getWeight() const
    {
        return (m_weight);
    }

    void  CeeGlobalCreatePriorityGroupMessage::setPfc(const SI32 &pfc)
    {
        m_pfc  =  pfc;
    }

    SI32  CeeGlobalCreatePriorityGroupMessage::getPfc() const
    {
        return (m_pfc);
    }

}
