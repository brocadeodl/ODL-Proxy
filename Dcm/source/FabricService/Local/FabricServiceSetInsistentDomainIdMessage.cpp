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
 *   Author : krangara                                                     *
 **************************************************************************/

#include "FabricService/Local/FabricServiceLocalObjectManager.h"
#include "FabricService/Local/FabricServiceSetInsistentDomainIdMessage.h"
#include "FabricService/Local/FabricServiceTypes.h"

namespace DcmNs
{

    FabricServiceSetInsistentDomainIdMessage::FabricServiceSetInsistentDomainIdMessage ()
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICESETINSISTENTDOMAINID)
    {
    }

    FabricServiceSetInsistentDomainIdMessage::FabricServiceSetInsistentDomainIdMessage (const UI32 &insistent_domainId)
        : PrismMessage (FabricServiceLocalObjectManager::getPrismServiceId (), FABRICSERVICESETINSISTENTDOMAINID),
        m_insistent_domainId    (insistent_domainId)
    {
    }

    FabricServiceSetInsistentDomainIdMessage::~FabricServiceSetInsistentDomainIdMessage ()
    {
    }

    void  FabricServiceSetInsistentDomainIdMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_insistent_domainId,"insistent_domainId"));
    }

    void  FabricServiceSetInsistentDomainIdMessage::setInsistent_domainId(const UI32 &insistent_domainId)
    {
        m_insistent_domainId  =  insistent_domainId;
    }

    UI32  FabricServiceSetInsistentDomainIdMessage::getInsistent_domainId() const
    {
        return (m_insistent_domainId);
    }

}
