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

#include "Qos/Global/CeeGlobalDeletePriorityGroupMessage.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalTypes.h"

namespace DcmNs
{

    CeeGlobalDeletePriorityGroupMessage::CeeGlobalDeletePriorityGroupMessage ()
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALDELETEPRIORITYGROUP)
    {
    }

    CeeGlobalDeletePriorityGroupMessage::CeeGlobalDeletePriorityGroupMessage (const string &ceeMapName,const string &pgId)
        : ManagementInterfaceMessage (QosGlobalObjectManager::getClassName (),CEEGLOBALDELETEPRIORITYGROUP),
        m_ceeMapName    (ceeMapName),
        m_pgId    (pgId)
    {
    }

    CeeGlobalDeletePriorityGroupMessage::~CeeGlobalDeletePriorityGroupMessage ()
    {
    }

    void  CeeGlobalDeletePriorityGroupMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeString(&m_pgId,"pgId"));
    }

    void  CeeGlobalDeletePriorityGroupMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  CeeGlobalDeletePriorityGroupMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  CeeGlobalDeletePriorityGroupMessage::setPgId(const string &pgId)
    {
        m_pgId  =  pgId;
    }

    string  CeeGlobalDeletePriorityGroupMessage::getPgId() const
    {
        return (m_pgId);
    }
}
